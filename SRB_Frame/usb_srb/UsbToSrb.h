#pragma once
#include "srb_usb_bus_heads.h"
#include "iBus.h"
#include "libusb.h"
#include <mutex> 

namespace srb {
	namespace usb_bus {
		class UsbToSrb : public iBus		{
		private:
			libusb_context* mainCTX = null;
			libusb_device * mainDEV = null;
			libusb_device_handle *mainDH = null;

			//------------------------------about access--------------------------------------------
			UsbAccess* accesses[256] = { null };
			uint8 point_in = 0;
			uint8 point_send = 0;
			uint8 point_out = 0;
			int accessSend(uint8 point);
			int accessRecv();

			//----------------------------private method-------------------------------------------
			int initUsbSrb(libusb_device * initDEV, libusb_device_handle * initDH);
			std::mutex access_lock;

		public:
			bool isOpen();
			UsbToSrb();
			~UsbToSrb();
			void closeUsb();
			int openUsbByName(const char* name);
			void testSend(uint8 *senddatas, uint8 *recvdatas);

			///overrite acceess port
			iAccess*  newAccess(BaseNode* sender_node)  override;
			int doAccess() override;

			inline int getAccessQueueLen()override {
				return (int)(point_in - point_out);
			}


		};
	}
}
//‘∆µÕ—‡π˝«· ∫∆∫∆◊ﬂº≤∑Á
//ª∆ªË«„÷Ë”Í æ¯“πœ÷À´–«