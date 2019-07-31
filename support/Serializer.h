#pragma once
#include "lee.h"

namespace srb {
	
	template<typename tTo>
	class iSerializer {
	protected:
		static constexpr int NAME_LEN = 20;
	public:
		tTo name[NAME_LEN];
		void* value_prt;
		int len;
		
		virtual int set(const tTo* buf) = 0;
		virtual int get(tTo* buf, int len) = 0;
	};


	template<typename tFrom,typename tTo>
	class Serializer:public iSerializer<tTo> {
	public:
		Serializer(const tTo* n, tFrom* value_prt, int len=1);
		int set(const tTo* buf) override;
		int get(tTo* buf, int len)override;
	};

	template<typename tFrom, typename tTo>
	Serializer<tFrom, tTo>::Serializer(const tTo * n, tFrom * value_prt, int len)
		:value_prt(value_prt), len(len) {
		for (int i = 0;i < 20;i++) {
			name[i] = n[i];
			if (n[i] == 0) {
				break;
			}
		}
	}





};