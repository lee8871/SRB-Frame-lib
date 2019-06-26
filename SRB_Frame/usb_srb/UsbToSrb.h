#pragma once
#include "srb_usb_bus_heads.h"
#include "iBus.h"
#include <memory>
namespace srb {
	class StreamJsonWriter;
	namespace usb_bus {
		class UsbToSrb : public iBus		{
		private:
			class Impl;
			std::unique_ptr<Impl> pimpl;
		public:
			UsbToSrb();
			~UsbToSrb();

			bool isOpen();
			int closeUsb();
			int openUsbByName(const char* name);

			iAccess*  newAccess(iAccesser* owner)  override;
			int doAccess() override;
			int getAccessQueueLen()override;
		};
	}
}
//‘∆µÕ—‡π˝«· ∫∆∫∆◊ﬂº≤∑Á
//ª∆ªË«„÷Ë”Í æ¯“πœ÷À´–«