#pragma once
#include "srb_usb_bus_heads.h"
#include "iBus.h"
#include "libusb.h"
#include <mutex> 
#include <fstream>

namespace srb {
	namespace usb_bus {
		class UsbToSrb : public iBus		{
		private:
			libusb_context* mainCTX = null;
			libusb_device * mainDEV = null;
			libusb_device_handle *mainDH = null;

			//------------------------------about access--------------------------------------------
			UsbAccess* acs_queue[256] = { null };
			uint8 point_in = 0;
			uint8 point_send = 0;
			uint8 point_out = 0;
			int accessSend(uint8 point);
			int accessRecv();

			//----------------------------private method-------------------------------------------
			int initUsbSrb(libusb_device * initDEV, libusb_device_handle * initDH);
			std::mutex access_lock;
			std::ofstream  recordSTM;

		public:
			bool isOpen();
			UsbToSrb();
			~UsbToSrb();
			void closeUsb();
			int openUsbByName(const char* name);
			void testSend(uint8 *senddatas, uint8 *recvdatas);

			iAccess*  newAccess(iAccesser* sender_node)  override;
			int doAccess() override;
			int getAccessQueueLen()override;





		};
	}
}
//‘∆µÕ—‡π˝«· ∫∆∫∆◊ﬂº≤∑Á
//ª∆ªË«„÷Ë”Í æ¯“πœ÷À´–«