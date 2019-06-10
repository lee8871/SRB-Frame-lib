#include "UsbAccess.h"

namespace srb {
	namespace usb_bus {
		UsbAccess::UsbAccess(iAccesser* n) {
			owner = n;
			_status = eAccessStatus::NoInit;
			usb_send_pkg = new sUsbToSrbPkg;
			if (usb_send_pkg != null) {
				_status = eAccessStatus::WaitSend;
				_send_pkg = &(usb_send_pkg->pkg);
			}

		}
		int UsbAccess::getUsbSendPkg(sUsbToSrbPkg** pkg, int* len) {
			if (_status != eAccessStatus::WaitSend) {
				return fail;
			}
			(*len) = 3 + usb_send_pkg->pkg.bfc.length;
			usb_send_pkg->addr = owner->Addr();
			*pkg = usb_send_pkg;
			_status = eAccessStatus::SendWaitRecv;
			return done;
		}
		int UsbAccess::setUsbRecvPkg(sUsbToSrbPkg * pkg, int len) {
			//check my status
			if (_status != eAccessStatus::SendWaitRecv) {
				return fail;
			}
			if (usb_recv_pkg != null) {
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
		int UsbAccess::timeoutAccess() {
			if (_status != eAccessStatus::SendWaitRecv) {
				return fail;
			}
			_status = eAccessStatus::BusTimeOut;
			return done;
		}


		eAccessStatus UsbAccess::getStatus() {
			return _status;
		}

		UsbAccess::~UsbAccess() {
			if (usb_send_pkg != null) {
				delete usb_send_pkg;
			}
			if (usb_recv_pkg != null) {
				delete usb_recv_pkg;
			}

		}

	}

}
