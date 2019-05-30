#pragma once
#include "iBus.h"
#include "lee.h"
#include "libusb.h"

namespace srb {
	class UsbToSrb : public iBus
	{
	private :
		libusb_context* mainCTX = null;
		libusb_device * mainDEV = null;
		libusb_device_handle *mainDH = null;

//------------------------------about access--------------------------------------------

		UsbAccess* accesses[256] = {null};
		uint8 point_in = 0;
		uint8 point_send = 0;
		uint8 point_out = 0;
		bool accessSend(uint8 point);
		bool accessRecv();


//----------------------------private method-------------------------------------------
		int initUsbSrb(libusb_device * initDEV, libusb_device_handle * initDH);

	public:
		bool isOpen();
		UsbToSrb();
		~UsbToSrb();
		void closeUsb();
		int openUsbByName(const char* name);
		void testSend(uint8 *senddatas, uint8 *recvdatas);	
		bool addAccess(UsbAccess * access_pa)  override;
		int doAccess() override;
		int getAccessQueueLen()override {
			return (int)(point_in - point_out);
		}


	};
}
