#include "UsbToSrb.h"
#include "string.h"
using namespace std;
namespace srb {
	namespace usb_bus {
		UsbToSrb::UsbToSrb()
		{
			int usb_status_flag = libusb_init(&mainCTX);
			if (usb_status_flag < 0)
			{
				throw "libusb can not init!";
			}
		}
		UsbToSrb::~UsbToSrb() {
			closeUsb();
			libusb_exit(mainCTX);
		}
		int UsbToSrb::openUsbByName(const char* name) {
			libusb_device **devs;
			char string[256];
			int usb_device_num;
			closeUsb();

			usb_device_num = libusb_get_device_list(mainCTX, &devs);
			if (usb_device_num < 0) {
				return usb_device_num;
			}
			libusb_device_descriptor dev_desc;
			libusb_device_handle *tempDH = NULL;
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
				libusb_get_string_descriptor_ascii(tempDH, dev_desc.iProduct, (unsigned char*)string, sizeof(string));
				if (0 != strcmp(string, "SRB-USB")) {
					libusb_close(tempDH);
					continue;
				}
				libusb_get_string_descriptor_ascii(tempDH, dev_desc.iSerialNumber, (unsigned char*)string, sizeof(string));
				if (0 != strcmp(string, name)) {
					libusb_close(tempDH);
					continue;
				}
				initUsbSrb(devs[i], tempDH);
				break;
			}
			libusb_free_device_list(devs, 1);
			if (mainDEV != null) {
				return 0;
			}
			else {
				return -1;
			}
		}
		int UsbToSrb::initUsbSrb(libusb_device * initDEV, libusb_device_handle * initDH) {
			mainDH = initDH;
			mainDEV = initDEV;
			libusb_ref_device(mainDEV);
			int rev;
			rev = libusb_set_configuration(mainDH, 1);
			if (LIBUSB_SUCCESS != rev) {
				closeUsb();
				return rev;
			}
			rev = libusb_claim_interface(mainDH, 0);
			if (LIBUSB_SUCCESS != rev) {
				closeUsb();
				return rev;
			}
			return 0;
		}
		void UsbToSrb::closeUsb() {
			if (mainDH != null) {
				libusb_release_interface(mainDH, 0);
				libusb_close(mainDH);
				mainDH = null;
			}
			if (mainDEV != null) {
				libusb_unref_device(mainDEV);
				mainDEV = null;
			}
		}
		bool UsbToSrb::isOpen() {
			return (mainDH != null);
		}
		iAccess*  UsbToSrb::newAccess(BaseNode* sender_node) {
			access_lock.lock();
			if (((point_in + 1) & 0xff) == point_out) {
				access_lock.unlock();return null;
			}
			UsbAccess *acs = new UsbAccess(sender_node);
			if (acs == null) {
				access_lock.unlock();return null;
			}
			if (acs->getStatus() == eAccessStatus::NoInit) {
				access_lock.unlock();delete acs;return null;
			}
			accesses[point_in] = acs;
			point_in++;				
			access_lock.unlock();return acs;
		}
		int UsbToSrb::doAccess() {
			int active_counter = 0;//记录正在交给硬件处理的包的数量,硬件带有双缓冲,可以进行访问的同时接收下一个访问,
			int error_counter = 0;//记录错误的次数,如果错误过多,则退出
			if (isOpen() == false) {
				return -1;
			}
			access_lock.lock();
			point_send = point_out;
			while (1) {
				if ((point_send != point_in) && (active_counter < 2)) {
					//需要发送的情况
					if (true == accessSend(point_send)) {
						active_counter++;
						point_send++;
					}
					else {
						error_counter++;
					}
				}
				else {//需要接收的情况
					while (accesses[point_out]->isStatusFinish()) {
						BaseNode* node = accesses[point_out]->node;
						node->sendDone(accesses[point_out]);
						delete accesses[point_out];
						accesses[point_out] = null;
						point_out++;
						if (point_out == point_in) {

							access_lock.unlock();return done;
						}
					}
					if (accessRecv()) {
						active_counter--;
					}
				}
			}

			access_lock.unlock();return fail;
		}



		bool UsbToSrb::accessSend(uint8 point) {
			UsbAccess *a = accesses[point];
			sUsbToSrbPkg* pkg;
			int length;
			int sent_len;
			a->getUsbSendPkg(&pkg, &length);
			pkg->sno = point;
			if (LIBUSB_SUCCESS != libusb_bulk_transfer(mainDH, (2), pkg->u8, length, &sent_len, 10)) {
				return false;
			}
			return true;
		}
		bool UsbToSrb::accessRecv() {
			sUsbToSrbPkg* pkg = new sUsbToSrbPkg();
			int rcvd_len;
			if (LIBUSB_SUCCESS != libusb_bulk_transfer(mainDH, (1 + 0x80), pkg->u8, 31 + 3, &rcvd_len, 10)) {
				return false;
			}
			uint8 point = pkg->sno;
			if (accesses[point] == null) {
				throw "recv a package to nonexistent node.";
			}
			accesses[point]->setUsbRecvPkg(pkg, rcvd_len);
			return false;
		}
	}
}