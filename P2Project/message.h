#ifndef MESSAGE_H
#define MESSAGE_H

#include "base.h"
#include <time.h>

struct Message
{
	char message[LENGTHOFMESSAGE];
	char username[LENGTHOFID];
	struct tm localTime;
	long pointerToNext;
	long pointerToS;
	long pointerTo0;
	bool isShared;
	int sharedTimes;

	bool earlierThan(Message &x);//whether is pushed earlier

	void printMegTime();
};

#endif