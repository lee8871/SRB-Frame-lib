#pragma once
namespace srb {
	class iAccess;
	class iAccesser;
	class AccessRecorder;
	class iBus{//这是SRB接口类
	private:
		AccessRecorder * access_recoeder = nullptr;
	protected:
		int accessDone(iAccess*);
	public:	
		//
		virtual iAccess*  newAccess(iAccesser*) = 0;
		virtual int loadAccess(iAccess* acs) = 0;
		virtual int doAccess() = 0;
		virtual int getAccessQueueLen() = 0;
	};
}
