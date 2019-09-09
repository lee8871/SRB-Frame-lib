#pragma once
#include "iAccess.h"
namespace srb {
	namespace usb_bus {
		class UsbAccess : public iAccess {
		private:
			sUsbToSrbPkg* usb_send_pkg = nullptr;
			sUsbToSrbPkg* usb_recv_pkg = nullptr;
			UsbAccess(iAccesser*);
		public:
			~UsbAccess();
			int getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
			int setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
			static UsbAccess* newAccess(iAccesser*);
		};
	}
}
