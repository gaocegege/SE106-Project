#include <string>
#include "base.h"
using namespace std;

/*
	The form to save the index 
*/

struct RecordNode
{
	char username[LENGTHOFID];
	int pointer;
	bool operator==(const RecordNode& x)
	{
		return  this->username == x.username;
	}
};

struct SaveForm
{
	RecordNode record;
	long pointerToNext;
};

struct RecordNode_name
{
	char username[LENGTHOFNAME];
	int pointer;
	bool operator==(const RecordNode_name& x)
	{
		return  this->username == x.username;
	}
};

struct SaveForm_name
{
	RecordNode_name record;
	long pointerToNext;
};

struct RecordNode_bir
{
	char username[LENGTHOFBIR];
	int pointer;
	bool operator==(const RecordNode_bir& x)
	{
		return  this->username == x.username;
	}
};

struct SaveForm_bir
{
	RecordNode_bir record;
	long pointerToNext;
};

struct RecordNode_tele
{
	char username[LENGTHOFTELEPHONE];
	int pointer;
	bool operator==(const RecordNode_tele& x)
	{
		return  this->username == x.username;
	}
};

struct SaveForm_tele
{
	RecordNode_tele record;
	long pointerToNext;
};

struct RecordNode_add
{
	char username[LENGTHOFADDRESS];
	int pointer;
	bool operator==(const RecordNode_add& x)
	{
		return  this->username == x.username;
	}
};

struct SaveForm_add
{
	RecordNode_add record;
	long pointerToNext;
};