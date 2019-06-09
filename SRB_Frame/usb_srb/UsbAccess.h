#pragma once
#include "srb_usb_bus_heads.h"
#include "iAccess.h"
namespace srb {
	namespace usb_bus {
		class UsbAccess : public iAccess {
		protected:
			sUsbToSrbPkg* usb_send_pkg = null;
			sUsbToSrbPkg* usb_recv_pkg = null;
		public:
			int getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
			int setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
			int timeoutAccess();
			eAccessStatus getStatus();
			UsbAccess(iAccesser*);
			~UsbAccess();
		};
	}
}
