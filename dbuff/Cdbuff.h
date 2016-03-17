#pragma once

#include <windows.h>

//#define dbg
#define dbg2

#ifdef dbg
#define debugstr(ss) printf("%s\n",ss)
#else
#define debugstr(ss)
#endif

#ifdef dbg2
#define debugstr2(ss,a,b,c) printf(ss,a,b,c)
#define debugstr3(ss) printf(ss)
#else
#define debugstr2(ss)
#define debugstr3(ss)
#endif

#define buff_size	(10)
struct cBuff{
	char buff[buff_size];
	int wi,ri;
	bool free;
	char id;

	cBuff(void):wi(0),ri(0),free(true){}

	bool write(char d){
		bool res = false;
		if(free){
			if(wi<buff_size){
				debugstr2("w buff_%d [%d]=%d\n",id,wi,d);
				buff[wi++] = d;
				res = true;
				debugstr("  write ok");
			} else {
				debugstr("  write is full");
				free = false;
			}
		}
		return res;
	}
	char read(char *d){
		char res = 0;
		if(!free) {
			if(ri<buff_size) {
				debugstr2("     r buff_%d [%d]=%d\n",id,ri,buff[ri]);
				*d = buff[ri++];
				res = 1;
				debugstr("  read ok");
			} else {
				debugstr("  read empty/switch");
			}
		} else {
			res = 2;
			debugstr("  read empty/write");
		}

		return res;
	}
	void discard(void){
		wi = 0; ri = 0; free = true;
	}
};

class cDBuff
{
public:
	cBuff buff[2];
	char wi, ri;

	HANDLE evRead, evWrite;

	cDBuff(void){
		debugstr("cdbuff created");
		evRead = CreateEvent(0,TRUE,0,0);
		evWrite = CreateEvent(0,0,0,0);
		wi = 0;
		ri = 0;
		buff[0].id = 0;
		buff[1].id = 1;

	}
	~cDBuff(void){}

	char swi(void){ return wi == 0 ? 1 : 0; }
	char sri(void){	return ri == 0 ? 1 : 0;	}

	void write(char d) {
		if(!buff[wi].write(d)) {
			if(!buff[swi()].write(d)) {
				debugstr(" all buffers full, waiting");
				WaitForSingleObject(evWrite,INFINITE);
				write(d);
			} else {
				debugstr(" write switch buffers ------ ");
				wi = swi();
				debugstr3("er");
				SetEvent(evRead);
			}
		} else {
			debugstr(" write 1 success");
		}
	}

	char read(void) {
		WaitForSingleObject(evRead,INFINITE);
		char res = 0;
		switch(buff[ri].read(&res))
		{
		case 0:
			// should switch buffers
			debugstr(" read switch buffers +++++++ ");
			buff[ri].discard();
			ri = sri();
			debugstr3("ew");
			SetEvent(evWrite);
			res = read();
			break;
		case 1:
			// read successful
			debugstr(" read successful");
			break;
		case 2:
			// buffer not yet ready
			debugstr(" nothing to read");
			ResetEvent(evRead);
			res = read();
			break;
		}
		return res;
	}
};

