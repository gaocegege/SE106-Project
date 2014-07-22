#ifndef INFORMATION_H
#define INFORMATION_H

#include <stdlib.h>
#include <string>

//the form to save the information
struct Information
{
	char identification[LENGTHOFID];
	char password[LENGTHOFPWD];
	char name[LENGTHOFNAME];
	char teleNumber[LENGTHOFTELEPHONE];
	char address[LENGTHOFADDRESS];
	bool gender;
	char birthday[LENGTHOFBIR];
	long pointer;
	long pointerToLike;
	int indexNumber;
	bool isAvailable;
};

typedef Information *Userinfo;

#endif