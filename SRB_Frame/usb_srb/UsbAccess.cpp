
#include "srb_usb_bus.in.h"
#include "UsbAccess.h"

using namespace lee8871_support;
namespace srb {
	namespace usb_bus {

		UsbAccess::UsbAccess(iAccesser* n) {
			owner = n;
			_status = eAccessStatus::Initing;
		}
		UsbAccess::~UsbAccess() {
			if (usb_send_pkg != nullptr) {
				delete usb_send_pkg;
		}
			if (usb_recv_pkg != nullptr) {
				delete usb_recv_pkg;
			}

		}
		UsbAccess * UsbAccess::newAccess(iAccesser * owner)		{
			UsbAccess * acs = new UsbAccess(owner);
			if (acs == nullptr) {
				return nullptr;
			}			
			acs->usb_send_pkg = new sUsbToSrbPkg;
			if (acs->usb_send_pkg == nullptr) {
				delete acs;
				return nullptr;
			}
			acs->_send_pkg = &(acs->usb_send_pkg->pkg);
			return acs;
		}
		
		int UsbAccess::getUsbSendPkg(sUsbToSrbPkg** pkg, int* len) {
			if (_status != eAccessStatus::WaitSend) {
				return fail;
			}
			(*len) = 3 + usb_send_pkg->pkg.bfc.length;
			usb_send_pkg->addr = owner->Addr();
			*pkg = usb_send_pkg;
			return done;
		}

		int UsbAccess::setUsbRecvPkg(sUsbToSrbPkg * pkg, int len) {
			//check my status
			if (_status != eAccessStatus::SendWaitRecv) {
				return fail;
			}
			if (usb_recv_pkg != nullptr) {
				delete usb_recv_pkg;
			}
			usb_recv_pkg = pkg;
			if (len >= 3) {//check receive done
				if ((len == usb_recv_pkg->pkg.bfc.length + 3) && (usb_recv_pkg->err < 0x10)) {
					_recv_pkg = &(usb_recv_pkg->pkg);
					_status = eAccessStatus::RecvedDone;return done;
				}
			}
			else if (len == 2) {//check error
				switch (usb_recv_pkg->err) {
				case USB_ERR_BROADCAST:
					if (owner->Addr() == SC_BROADCAST) {
						_status = eAccessStatus::RecvedDone;return done;
					}
					break;
				case USB_ERR_BUS_TIMEOUT:
					_status = eAccessStatus::BusTimeOut;return done;
					break;
				default:
					break;
				}
			}
			_status = eAccessStatus::RecvedBadPkg;return done;
		}
	}
}
