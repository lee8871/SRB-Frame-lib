#include <fstream>
#include <iostream>
#include <stdio.h>
#include <memory>

#include "srb.in.h"
#include "StreamJsonWriter.h"
#include "OsSupport.h"
#include "AccessRecorder.h"
#include "iAccess.h"
#include "cLogger.h"

using namespace std;
using namespace lee8871_support;
namespace srb {
	class AccessRecorder::Impl{
	private:
		std::ofstream recordSTM;

		std::unique_ptr<StreamJsonWriter> recordSJW;
		char pathname[256];
		bool is_base_pathname_ready = false;
		bool is_open = false;
		char base_pathname[256];
		int file_name_counter = 0;
		int access_counter = 0;
		int setNextFile(){
			if (is_base_pathname_ready == false) {
				Srb_log.addLog(eLogLevel::erro, "Pathname is not set.");
				is_open = false;
				return fail;
			}
			int len = snprintf(pathname, 256, base_pathname, file_name_counter);	
			if (len >= 256) {
				Srb_log.addLog(eLogLevel::erro, "Pathname is too long <%s> ",pathname);
				is_open = false;
				return fail;
			}
			if (recordSTM.is_open() == true) {
				recordSTM.clear();
				recordSTM.close();
			}
			recordSTM.open(pathname, ios::out | ios::trunc);
			if (recordSTM.is_open() == false) {
				Srb_log.addLog(eLogLevel::erro, "<%s> opening fail.", pathname);
				is_open = false;
				return fail;
			}
			file_name_counter++;
			is_open = true;
			return done;
		}


	public:
		Impl():		
			recordSJW(std::make_unique<StreamJsonWriter>(&recordSTM))
		{
		}

		int setPathname(const char* pathname){
			int point_write = 0, point_read = 0;
			int point_sno = -1;
			int point_dot = -1;
			int point_end = -1;

			while(point_end==-1){
				char ch = pathname[point_read++];
				switch (ch) {
				case '\0':
					point_end = point_write;
					base_pathname[point_write++] = '\0';
					break;
				case '%':
					if (point_sno == -1) {//not get %d
						if (pathname[point_read++] == 'd') {
							point_sno = point_write;//get and location it
							base_pathname[point_write++] = '%';
							base_pathname[point_write++] = 'd';
						}
					}
					break;
				case '.':
					point_dot = point_write;
					base_pathname[point_write++] = '.';
					break;
				default :
					base_pathname[point_write++] = ch;
					break;
				}
				if (point_write > 240) {
					Srb_log.addLog(eLogLevel::erro, "Pathname too long (>240).");
					is_base_pathname_ready = false;
					return fail;
				}
			}
			if (point_sno ==-1) {
				//TODO add %d auto
				Srb_log.addLog(eLogLevel::erro, "There is no @d for sno in str.");
				is_base_pathname_ready = false;
				return fail;
			}
			is_base_pathname_ready = true;
			setNextFile();
			return done;
		}

		int record(iAccess* acs) {
			if (is_open) {
				acs->sendJson(*recordSJW);
				access_counter++;
				if (access_counter > 100000) {
					setNextFile();
					access_counter = 0;
				}
				return done;
			}
			return fail;
		}


	};

	AccessRecorder::AccessRecorder() :pimpl(std::make_unique<Impl>()) {}
	AccessRecorder::~AccessRecorder() = default;

	int AccessRecorder::setPathname(const char * pathname)	{
		return pimpl->setPathname(pathname);
	}

	int AccessRecorder::record(iAccess * acs)
	{
		return pimpl->record(acs);
	}

};