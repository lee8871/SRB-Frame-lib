#include "libusb.h"
#include <mutex> 
#include <fstream>
#include <time.h>  
#include "UsbToSrb.h"
#include "string.h"
#include "StreamJsonWriter.h"

#include "UsbToSrb.h"
#include "UsbAccess.h"
#include "AccessRecorder.h"

#include "OsSupport.h"



using namespace std;
namespace srb {
	namespace usb_bus {


		class UsbToSrb::Impl{
		private:
			UsbToSrb* parent;
			libusb_context* mainCTX = nullptr;
			libusb_device * mainDEV = nullptr;
			libusb_device_handle *mainDH = nullptr;		

			std::mutex access_lock;

			//------------------------------about access--------------------------------------------
			UsbAccess* acs_queue[256] = { nullptr };
			uint8 point_in = 0;
			uint8 point_out = 0;
			int accessSend(uint8 point) {
				UsbAccess *a = acs_queue[point];
				sUsbToSrbPkg* pkg;
				int length;
				int sent_len;
				int rev;
				a->getUsbSendPkg(&pkg, &length);
				pkg->sno = point;
				if (LIBUSB_SUCCESS != (rev=libusb_bulk_transfer(mainDH, (2), pkg->u8, length, &sent_len, 10))) {
					logger.infoPrint('u',"Send pkg error %d",rev);
					return fail;
				}
				if (sent_len != length) {
					logger.infoPrint('u', "USB pkg length is %d but send %d", length, sent_len);
					return fail;
				}
				a->recordSendTime();
				logger.infoPrint('U',"send Pkg len %d", sent_len);
				return done;
			}
			int accessRecv() {
				sUsbToSrbPkg* pkg = new sUsbToSrbPkg();
				int rcvd_len;
				int rev;
				if (LIBUSB_SUCCESS != (rev = libusb_bulk_transfer(mainDH, (1 + 0x80), pkg->u8, 31 + 3, &rcvd_len, 10))) {
					logger.infoPrint('u',"receive pkg error %d",rev);
					delete pkg;
					return fail;
				}
				if (rcvd_len <= 1) {
					if (rcvd_len == 1) {
						logger.infoPrint('u', "recv Pkg length=1(%d)", pkg->u8[0]);
					}
					else {
						logger.infoPrint('u', "recv Pkg length=%d", rcvd_len);
					}
					delete pkg;
					return fail;
				}
				uint8 point = pkg->sno;
				if (acs_queue[point] == nullptr) {
					logger.infoPrint('u', "recv a package to nonexistent node.(addr:%d,sno:%d,len:%d); ",pkg->addr,pkg->sno, rcvd_len);
					delete pkg;
					return fail;
				}
				acs_queue[point]->setUsbRecvPkg(pkg, rcvd_len);
				logger.infoPrint('U',"recv Pkg ");
				return done;
			}
			//----------------------------private method-------------------------------------------
		public:
			Impl(UsbToSrb *p) {
				parent = p;

			}
			~Impl() {
				closeUsb();
			}
			bool isOpen() {
				return (mainDH != nullptr);
			}
			int closeUsb() {
				if (mainDH != nullptr) {
					libusb_release_interface(mainDH, 0);
					libusb_close(mainDH);
					mainDH = nullptr;
				}
				if (mainDEV != nullptr) {
					libusb_unref_device(mainDEV);
					mainDEV = nullptr;
				}
				if(mainCTX!=nullptr){
					libusb_exit(mainCTX);					
					logger.infoPrint('U',"close mainCTX!");
					OsSupport::msSleep(50);
					mainCTX=nullptr;
				}				
				return done;
			}

			int initUsbSrb(libusb_device * device) {
				mainDEV = device;
				int rev;
				rev = libusb_open(mainDEV, &mainDH);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb device open fail, libusb_err=%d.", rev);	
					libusb_close(mainDH);
					closeUsb();	return fail;
				}
				rev = libusb_reset_device(mainDH);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb device reset fail, libusb_err=%d.", rev);
				}
				rev = libusb_set_configuration(mainDH, 1);
				if (LIBUSB_SUCCESS != rev) {					
					logger.errPrint("libusb_set_configuration fail, libusb_err=%d.",rev);
					closeUsb();	return fail;
				}
				rev = libusb_claim_interface(mainDH, 0);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb_claim_interface fail, libusb_err=%d.",rev);
					closeUsb();	return fail;
				}
				return done;
			}

			bool isDeviceNamed(const char* name,libusb_device * device){
				libusb_device_descriptor dev_desc;
				if (libusb_get_device_descriptor(device, &dev_desc) < 0) {
					return false;
				}
				if ((dev_desc.idVendor != 0x16c0) || (dev_desc.idProduct != 0x05dc)) {
					return false;
				}
				libusb_device_handle *device_handle = nullptr;
				int rev = libusb_open(device, &device_handle);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb device open fail, libusb_err=%d.", rev);	
					libusb_close(device_handle);
					return false;
				}
				char str[256];
				rev = libusb_get_string_descriptor_ascii(device_handle, dev_desc.iProduct, (unsigned char*)str, sizeof(str));
				if(rev < 0){					
					logger.errPrint("libusb get string_descriptor.iProduct(%d) fail, libusb_err=%d.", dev_desc.iProduct, rev);						
					libusb_close(device_handle);return false;
				}
				if (0 != strcmp(str, "SRB-USB")){								
					libusb_close(device_handle);return false;
				}
				rev =libusb_get_string_descriptor_ascii(device_handle, dev_desc.iSerialNumber, (unsigned char*)str, sizeof(str));
				if(rev < 0){							
					logger.errPrint("libusb get string_descriptor.iSerialNumber(%d) fail, libusb_err=%d.", dev_desc.iSerialNumber,rev);	
					libusb_close(device_handle);return false;
				}
				if (0 != strcmp(str, name)) {	
					libusb_close(device_handle);return false;
				}
				libusb_close(device_handle);
				return true;
			}

			int selectUsbByName(const char* name, libusb_device ** selected_device){				
				*selected_device = nullptr;
				libusb_device **devices;
				//int rev;
				//char str[64];
				int usb_device_num;
				usb_device_num = libusb_get_device_list(mainCTX, &devices);
				if (usb_device_num < 0) {
					logger.errPrint("Get device list fail, libusb_err=%d.",usb_device_num);
					return fail;
				}				
				for (int i = 0;i < usb_device_num;i++){
					if(true == isDeviceNamed(name,devices[i])){
						libusb_ref_device(devices[i]);
						*selected_device = devices[i];
						break;
					}
				}								
				libusb_free_device_list(devices, 1);
				return done;
			}
			//TODO last_name is just for test reset USB
			const char* last_name;
			int openUsbByName(const char* name) {
				last_name=name;
				closeUsb();
				int usb_status_flag = libusb_init(&mainCTX);
				if (usb_status_flag < 0){
					logger.errPrint("libusb can not init! rev:%d",usb_status_flag);
				}				
				libusb_device * selected_device = nullptr;
				if(fail == selectUsbByName(name, &selected_device))	{
					return fail;
				}
				if(selected_device == nullptr){
					return fail;
				}
				if(fail == initUsbSrb(selected_device)){
					return fail;
				}
				return done;
			}

			int resetUSB(){
				int rev;
				if (mainDH != nullptr) {
					libusb_release_interface(mainDH, 0);
					libusb_close(mainDH);
					mainDH = nullptr;
				}
				rev = libusb_open(mainDEV, &mainDH);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb device open fail, libusb_err=%d.", rev);	
					libusb_close(mainDH);
					closeUsb();	return fail;
				}
				rev = libusb_reset_device(mainDH);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb device reset fail, libusb_err=%d.", rev);
				}
				rev = libusb_set_configuration(mainDH, 1);
				if (LIBUSB_SUCCESS != rev) {					
					logger.errPrint("libusb_set_configuration fail, libusb_err=%d.",rev);
					closeUsb();	return fail;
				}
				rev = libusb_claim_interface(mainDH, 0);
				if (LIBUSB_SUCCESS != rev) {
					logger.errPrint("libusb_claim_interface fail, libusb_err=%d.",rev);
					closeUsb();	return fail;
				}
				return done;

			}


			int lsUsbByName(strlist name_len_64,int len) {
				libusb_device **devs;
				char str[256];
				int usb_device_num;
				closeUsb();

				usb_device_num = libusb_get_device_list(mainCTX, &devs);
				if (usb_device_num < 0) {
					return usb_device_num;
				}
				libusb_device_descriptor dev_desc;
				libusb_device_handle *tempDH = nullptr;
				int scan_counter = 0;
				for (int i = 0;i < usb_device_num;i++)
				{
					if (libusb_get_device_descriptor(devs[i], &dev_desc) < 0) {
						continue;
					}
					if ((dev_desc.idVendor != 0x16c0) || (dev_desc.idProduct != 0x05dc)) {
						continue;
					}
					if (LIBUSB_SUCCESS != libusb_open(devs[i], &tempDH)) {
						continue;
					}
					libusb_get_string_descriptor_ascii(tempDH, dev_desc.iProduct, (unsigned char*)str, sizeof(str));
					if (0 != strcmp(str, "SRB-USB")) {
						libusb_close(tempDH);
						continue;
					}
					libusb_get_string_descriptor_ascii(tempDH, dev_desc.iSerialNumber, (unsigned char*)str, 64);
					libusb_close(tempDH);
					strcpy_s<64>((name_len_64[scan_counter]),str);
					scan_counter++;
					if(scan_counter == len)
					{
						return scan_counter;
					}
				}
				libusb_free_device_list(devs, 1);				
				return scan_counter;
			}


			int access_reset_counter = 0;
			static const int ACCESS_RESET_MAX = 5;
			int loadAccess(iAccess* acs){
				UsbAccess* uacs = dynamic_cast<UsbAccess*>(acs);
				if (uacs == nullptr) { return fail; }
				if (uacs->initDone() == fail) { return fail; };
				access_lock.lock();
				acs_queue[point_in++] = uacs;
				if(point_in == point_out) {
					point_in--;
					acs_queue[point_in] = nullptr;
					access_lock.unlock();
					return fail;
				}
				access_lock.unlock();
				return done;
			};
			int doAccess() {
				int active_counter = 0;//记录正在交给硬件处理的包的数量,硬件带有双缓冲,可以进行访问的同时接收下一个访问,
				int send_error_counter = 0;
				int recv_error_counter = 0;
				if (isOpen() == false) {
					logger.errPrint("Do access before open port.");
					return fail;
				}
				uint8 point_send = 0;
				access_lock.lock();
				point_send = point_out;
				while (1) {
					if ((point_send != point_in) && (active_counter < 2)) {//需要发送的情况
						if (acs_queue[point_send]->Status == eAccessStatus::WaitSend) {//是需要发送的包							
							access_lock.unlock();
							int is_access_send_status = accessSend(point_send);
							access_lock.lock();
							if (done == is_access_send_status) {
								active_counter++;
								point_send++;
							}
							else {
								send_error_counter++;
							}
						}
						else {
							//如果包的类型奇怪则发布一个错误。
							if (acs_queue[point_send]->Status != eAccessStatus::Cancel) {
								logger.errPrint("An access in queue[%d] status %d is sending.", point_send, acs_queue[point_send]->Status );
							}
							point_send++;
						}
					}
					else {//需要接收的情况
					//check if access finish. If finish dequeue and report to accesser
						while (acs_queue[point_out]->isStatusFinish()) {
							UsbAccess* acs = acs_queue[point_out];
							acs_queue[point_out] = nullptr;
							point_out++;
							access_lock.unlock();
							parent->accessDone(acs);
							//TODO acs应该可以在访问后不被销毁，由访问的发起者销毁，但是发起者不知道访问对应的接口类型。
							//delete acs;
							access_lock.lock();
							if (point_out == point_in) {
								access_lock.unlock();return done;
							}
						}
						access_lock.unlock();
						int access_recv_status = accessRecv();
						access_lock.lock();
						if (done == access_recv_status ) {
							active_counter--;
						}
						else {
							recv_error_counter++;
						}
					}
					if((recv_error_counter >=5)||(send_error_counter >=5)){
						//TODO: if fail exit ,we shold do some thing.
						access_lock.unlock();		
						if(access_reset_counter == ACCESS_RESET_MAX){
							logger.crashPrint("Access timeover too many times!");
							return fail;
						}
						else{
							int rev = openUsbByName(last_name);
							if(rev == done){     
								logger.errPrint("Access timeover reset all USB%d done(recv:%d,send:%d), portname is %s"
								,access_reset_counter,recv_error_counter,send_error_counter, last_name);				
							}
							else{
								logger.errPrint("Access timeover reset all USB%d fail(recv:%d,send:%d), portname is %s "
								,access_reset_counter,recv_error_counter,send_error_counter, last_name);
								return fail;
							}
							access_reset_counter++;
						}
						access_lock.lock();
					}
				}
			}
			int getAccessQueueLen(){
				return (int)(point_in - point_out);
			}
		};


		UsbToSrb::UsbToSrb():pimpl(std::make_unique<Impl>(this)){}
		UsbToSrb::~UsbToSrb() = default;

		int UsbToSrb::openUsbByName(const char* name){
			return pimpl->openUsbByName(name);
		}

		int UsbToSrb::lsUsbByName(strlist name_len_64,int len){
			return pimpl->lsUsbByName(name_len_64,len);
		}
		int UsbToSrb::closeUsb() {
			return pimpl->closeUsb();
		}
		bool UsbToSrb::isOpen() {
			return pimpl->isOpen();
		}
		iAccess*  UsbToSrb::newAccess(iAccesser* owner) {			
			UsbAccess *acs = UsbAccess::newAccess(owner);
			return acs;
		}
		int UsbToSrb::loadAccess(iAccess * acs){
			return pimpl->loadAccess(acs);
		}
		int UsbToSrb::doAccess(){
			return pimpl->doAccess();
		}
		int UsbToSrb::getAccessQueueLen() {
			return pimpl->getAccessQueueLen();
		}
	}
}