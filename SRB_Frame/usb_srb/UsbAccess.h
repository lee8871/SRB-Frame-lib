#pragma once
#include "srb_usb_bus_heads.h"
#include "iAccess.h"
namespace srb {
	namespace usb_bus {
		class UsbAccess : public iAccess {
		protected:
			sUsbToSrbPkg* usb_send_pkg = nullptr;
			sUsbToSrbPkg* usb_recv_pkg = nullptr;
			UsbAccess(iAccesser*);
		public:
			~UsbAccess();
			int getUsbSendPkg(sUsbToSrbPkg** pkg, int* len);
			int setUsbRecvPkg(sUsbToSrbPkg* pkg, int len);
			int timeoutAccess();
			int initDone();
			static UsbAccess* newAccess(iAccesser*);
			virtual const char* getType() override;
		};
	}
}
