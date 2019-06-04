#include "UsbAccess.h"
#include "BaseNode.h"

namespace srb {
	namespace usb_bus {
		UsbAccess::UsbAccess(BaseNode* n) {
			node = n;
			_status = eAccessStatus::NoInit;
			usb_send_pkg = new sUsbToSrbPkg;
			if (usb_send_pkg != null) {
				_status = eAccessStatus::NoSend;
				_send_pkg = &(usb_send_pkg->pkg);
			}

		}
		bool UsbAccess::getUsbSendPkg(sUsbToSrbPkg** pkg, int* len) {
			if (_status != eAccessStatus::NoSend) {
				return false;
			}
			(*len) = 3 + usb_send_pkg->pkg.bfc.length;
			usb_send_pkg->addr = node->getAddr();
			*pkg = usb_send_pkg;
			_status = eAccessStatus::SendWaitRecv;
			return true;
		}
		bool UsbAccess::setUsbRecvPkg(sUsbToSrbPkg * pkg, int len) {
			//check my status
			if (_status != eAccessStatus::SendWaitRecv) {
				return false;
			}
			if (usb_recv_pkg != null) {
				delete usb_recv_pkg;
			}
			usb_recv_pkg = pkg;

			if (len >= 3) {//check receive done
				if ((len == usb_recv_pkg->pkg.bfc.length + 3) && (usb_recv_pkg->err < 0x10)) {
					_recv_pkg = &(usb_recv_pkg->pkg);
					_status = eAccessStatus::RecvedDone;return true;
				}
			}
			else if (len == 2) {//check error
				switch (usb_recv_pkg->err) {
				case USB_ERR_BROADCAST:
					if (node->getAddr() == SC_BROADCAST) {
						_status = eAccessStatus::RecvedDone;return true;
					}
					else {
						_status = eAccessStatus::RecvedBadPkg;return true;
					}
				case USB_ERR_BUS_TIMEOUT:
					_status = eAccessStatus::BusTimeOut;return true;
				default:
					break;
				}
			}
			_status = eAccessStatus::RecvedBadPkg;return true;
		}
		bool UsbAccess::timeoutAccess() {
			if (_status != eAccessStatus::SendWaitRecv) {
				return false;
			}
			_status = eAccessStatus::BusTimeOut;
			return true;
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
