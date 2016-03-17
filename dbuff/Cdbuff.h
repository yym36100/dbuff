#pragma once

#define buff_size	(100)
struct cBuff{
	char buff[buff_size];
	int wi,ri;
	bool free;

	cBuff(void):wi(0),ri(0),free(true){}

	bool write(char d){
		bool res = false;
		if(free) {
			if(wi<buff_size){
				buff[wi++] = d;
				res = true;
			} else {
				free = false;
			}
		}
		return res;
	}
	bool read(char *d){
		bool res = false;
		if(!free) {
			if(ri<buff_size) {
				*d = buff[ri++];
			}
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
	Cdbuff(void){}
	~Cdbuff(void){}
};
