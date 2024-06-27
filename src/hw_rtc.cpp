//
// hw_rtc.cpp
//
////// BEGIN LICENSE NOTICE//////
//
//Baffa-1 HomebrewCPU Minicomputer System Emulator
//
//Copyright(C) 2021 Augusto Baffa, (sol-1.baffasoft.com.br)
//
//This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301, USA.
//
////// END LICENSE NOTICE//////
//
#include <stdio.h>
#include <stdlib.h>
#include "hw_rtc.h"

#include <time.h>
#include <ctime>

#ifdef _MSC_VER    
#include <windows.h>
#else
#include <pthread.h> 
#include "utils.h"
#endif


BAFFA1_DWORD hw_rtc_current_datetime() {

	time_t timer;
	struct tm y2k = { 0 };
	BAFFA1_DWORD seconds;

	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */

	seconds = (BAFFA1_DWORD)difftime(timer, mktime(&y2k));

	return seconds;
}



void hw_rtc_init(struct hw_rtc* hw_rtc) {
	int i = 0;
	for (i = 0; i < 16; i++) {
		hw_rtc->data[i] = 0x00;
	}
}




BAFFA1_BYTE convertToHex(int v) {
	char buffer[16];
#if defined(_MSC_VER) || defined(__MINGW32__)     
	BAFFA1_BYTE ret = (BAFFA1_BYTE)strtol(_itoa(v, buffer, 10), NULL, 16);
#else
	BAFFA1_BYTE ret = (BAFFA1_BYTE)strtol(itoa(v, buffer, 10), NULL, 16);
#endif
	return ret;
}

BAFFA1_BYTE convertToInt(int v) {
	char buffer[16];
#if defined(_MSC_VER) || defined(__MINGW32__)     
	BAFFA1_BYTE ret = (BAFFA1_BYTE)strtol(_itoa(v, buffer, 16), NULL, 10);
#else
	BAFFA1_BYTE ret = (BAFFA1_BYTE)strtol(itoa(v, buffer, 16), NULL, 10);
#endif
	return ret;
}


#ifdef _MSC_VER    
DWORD WINAPI MTRtcThread(LPVOID pParam)
#else
void *MTRtcThread(void *pParam)
#endif
{
	/*
	; RTC SERVICES INTERRUPT
		; RTC I / O bank = FFA0 to FFAF
		; FFA0 to FFA7 is scratch RAM
		; control register at $FFA8[W | R | S | Cal4..Cal0]
		; al = 0..6->get
		; al = 7..D->set
*/
	struct hw_rtc* hw_rtc = (struct hw_rtc*)pParam;
	struct tm * timeinfo;
	time_t rawtime;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	hw_rtc->data[15] = convertToHex(timeinfo->tm_year - 100);
	hw_rtc->data[14] = convertToHex(timeinfo->tm_mon + 1);
	hw_rtc->data[13] = convertToHex(timeinfo->tm_mday);
	hw_rtc->data[12] = convertToHex(timeinfo->tm_wday);

	hw_rtc->data[11] = convertToHex(timeinfo->tm_hour);
	hw_rtc->data[10] = convertToHex(timeinfo->tm_min);
	hw_rtc->data[9] = convertToHex(timeinfo->tm_sec);

	while (true)
	{

		timeinfo->tm_year = convertToInt(hw_rtc->data[15]) + 100;
		timeinfo->tm_mon = convertToInt(hw_rtc->data[14]) - 1;
		timeinfo->tm_mday = convertToInt(hw_rtc->data[13]);

		timeinfo->tm_hour = convertToInt(hw_rtc->data[11]);
		timeinfo->tm_min = convertToInt(hw_rtc->data[10]);
		timeinfo->tm_sec = convertToInt(hw_rtc->data[9]) + 1;

		mktime(timeinfo);

		hw_rtc->data[15] = convertToHex(timeinfo->tm_year - 100);
		hw_rtc->data[14] = convertToHex(timeinfo->tm_mon + 1);
		hw_rtc->data[13] = convertToHex(timeinfo->tm_mday);
		hw_rtc->data[12] = convertToHex(timeinfo->tm_wday);

		hw_rtc->data[11] = convertToHex(timeinfo->tm_hour);
		hw_rtc->data[10] = convertToHex(timeinfo->tm_min);
		hw_rtc->data[9] = convertToHex(timeinfo->tm_sec);

#ifdef _MSC_VER     
		Sleep(1000);
#else
		int milliseconds = 1000;
		struct timespec ts;
		ts.tv_sec = milliseconds / 1000;
		ts.tv_nsec = (milliseconds % 1000) * 1000000;
		nanosleep(&ts, NULL);

#endif
	}

	return 0;
}





void hw_rtc_start_clock(struct hw_rtc* hw_rtc) {

#ifdef _MSC_VER    
	DWORD tid;
	HANDLE myHandle = CreateThread(0, 0, MTRtcThread, hw_rtc, 0, &tid);
#else
	pthread_t tid;
	pthread_create(&tid, NULL, MTRtcThread, (void *)hw_rtc);
#endif

}


void hw_rtc_print(struct hw_rtc* hw_rtc, char *dir, int changed, char *print) {


	int i = 0;
	sprintf(print, ">>> RTC [%s]:", dir);
	for (i = 0; i < 16; i++) {
		if (changed == i)
			sprintf(print + strlen(print), "[%02x", hw_rtc->data[i]);
		else if (changed == i - 1)
			sprintf(print + strlen(print), "]%02x", hw_rtc->data[i]);
		else
			sprintf(print + strlen(print), " %02x", hw_rtc->data[i]);
	}
	if (changed == 15)
		sprintf(print + strlen(print), "]\n");
	else
		sprintf(print + strlen(print), "\n");
}