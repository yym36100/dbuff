// dbuff.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cdbuff.h"
#include <windows.h>


DWORD WINAPI writer(LPVOID lpParameter);
DWORD WINAPI reader(LPVOID lpParameter);

int _tmain(int argc, _TCHAR* argv[]) {
	cDBuff mydbuff;
	HANDLE w,r;

	w = CreateThread(0,0,writer,&mydbuff,CREATE_SUSPENDED,0);
	r = CreateThread(0,0,reader,&mydbuff,CREATE_SUSPENDED,0);

	ResumeThread(w);
	ResumeThread(r);
	while(1);

	return 0;
}

DWORD WINAPI writer(LPVOID lpParameter) {
	cDBuff *p = (cDBuff*)lpParameter;
	unsigned char i=0;
	while(1){
		//printf("adding %d\n",i);
		p->write(i++);
		//putchar('w');
		if(i>100) return 1;
	}
	return 0;
}

DWORD WINAPI reader(LPVOID lpParameter) {
	cDBuff *p = (cDBuff*)lpParameter;
	char d;
	while(1){
		d = p->read();		
		//printf("r %d \n",d);
		Sleep(100);
	}
	return 0;
}


