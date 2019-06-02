#pragma once
#include "lee.h"
#include "iAccess.h"
namespace srb {
	namespace usb_bus {
		class UsbAccess : public iAccess {
		protected:
			sUsbToSrbPkg* usb_send_pkg = null;
			sUsbToSrbPkg* usb_recv_pkg = null;
		public:
			bool getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
			bool setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
			bool timeoutAccess();
			eAccessStatus getStatus();
			UsbAccess(BaseNode*);
			~UsbAccess();
		};
	}
}
