#ifndef HASH_H
#define HASH_H

#include "Index.h"
#include "base.h"
#include <vector>
#include <fstream>
#include <list>

#define H 133333

using namespace std;

template <class T, class S>
class HashTable
{
public:
	HashTable(char *x, int size = H);
	~HashTable();
	T contains(char *x);
	S containsFrom(char *x, S &t);
	void makeEmpty();
	bool insert(T &x);
	void deleteNode(T &x);
	void begin();
	void dump();
private:
	int myhash(T &x);
	int myhash(char *x);
	int currentSize;
	char *fileName;
};

template <class T, class S>
int HashTable<T, S>::myhash(T &x)//BKDRHash
{
	unsigned  int  seed = 131;  //  31 131 1313 13131 131313 etc..
	unsigned  int  hash = 0;
	for (int i = 0; i < LENGTHOFID; i++)
	{
		if (x.username[i] == '\0')
			break;
		hash = hash  *  seed + x.username[i];
	}
	return  (hash & 0x7FFFFFFF) % (H + 1);
}

template <class T, class S>
int HashTable<T, S>::myhash(char *x)
{
	unsigned  int  seed = 131;  //  31 131 1313 13131 131313 etc..
	unsigned  int  hash = 0;
	for (int i = 0; i < LENGTHOFID; i++)
	{
		if (x[i] == '\0')
			break;
		hash = hash  *  seed + x[i];
	}
	return  (hash & 0x7FFFFFFF) % (H + 1);
}

template <class T, class S>
HashTable<T, S>::HashTable(char *x, int size)
: currentSize(size)
{
	this->fileName = new char[strlen(x)];
	strcpy(this->fileName, x);
}

template <class T, class S>
HashTable<T, S>::~HashTable()
{
	free(fileName);
}

//if contains,return it,else return Node whose pointer is -2
template <class T, class S>
T HashTable<T, S>::contains(char *x)
{
	int h = myhash(x);
	S buf;
	T buf_r;
	long buf_p;
	ifstream ifs(fileName, ios::in | ios::binary);
	ofstream ofs(fileName, ios_base::in | ios_base::out | ios::binary);
	ifs.seekg(h*sizeof(S));
	ifs.read((char *)&buf, sizeof(buf));
	if (buf.pointerToNext == -2)
	{
		buf_r.pointer = -2;
		return buf_r;
	}

	while (buf.pointerToNext != -1)
	{
		if (strcmp(buf.record.username, x) == 0)
			return buf.record;
		ifs.seekg(buf.pointerToNext);
		ifs.read((char *)&buf, sizeof(buf));
	}

	if (strcmp(buf.record.username, x) == 0)
		return buf.record;
	buf_r.pointer = -2;
	return buf_r;
}

template <class T, class S>
S HashTable<T, S>::containsFrom(char *x, S &t)
{
	int h = myhash(x);
	S buf;
	T buf_r;
	long buf_p;
	if (t.pointerToNext == -3)
	{
		ifstream ifs(fileName, ios::in | ios::binary);
		ofstream ofs(fileName, ios_base::in | ios_base::out | ios::binary);
		ifs.seekg(h*sizeof(S));
		ifs.read((char *)&buf, sizeof(buf));
		if (buf.pointerToNext == -2)
		{
			return buf;
		}

		while (buf.pointerToNext != -1)
		{
			if (strcmp(buf.record.username, x) == 0)
				return buf;
			ifs.seekg(buf.pointerToNext);
			ifs.read((char *)&buf, sizeof(buf));
		}

		if (strcmp(buf.record.username, x) == 0)
			return buf;
		buf.pointerToNext = -2;
		return buf;
	}

	ifstream ifs(fileName, ios::in | ios::binary);
	ofstream ofs(fileName, ios_base::in | ios_base::out | ios::binary);
	ifs.seekg(t.pointerToNext);
	ifs.read((char *)&buf, sizeof(buf));
	if (buf.pointerToNext == -2)
	{
		return buf;
	}

	while (buf.pointerToNext != -1)
	{
		if (strcmp(buf.record.username, x) == 0)
			return buf;
		ifs.seekg(buf.pointerToNext);
		ifs.read((char *)&buf, sizeof(buf));
	}

	if (strcmp(buf.record.username, x) == 0)
		return buf;
	buf.pointerToNext = -2;
	return buf;
}

template <class T, class S>
void HashTable<T, S>::makeEmpty()
{

}

template <class T, class S>
bool HashTable<T, S>::insert(T &x)
{
	int h = myhash(x);
	S buf;
	long buf_p;
	ifstream ifs(fileName, ios::in | ios::binary);
	ofstream ofs(fileName, ios_base::in | ios_base::out | ios::binary);

	ofs.seekp(0, ios::end);
	ifs.seekg(h*sizeof(buf));
	ifs.read((char *)&buf, sizeof(buf));

	if (buf.pointerToNext == -2)
	{
		ofs.seekp(h*sizeof(buf));
		buf.record = x;
		buf.pointerToNext = -1;
		ofs.write((char *)&buf, sizeof(buf));
		return true;
	}
	else if (buf.pointerToNext == -1)
	{
		buf.pointerToNext = ofs.tellp();
		ofs.seekp(h*sizeof(buf));
		ofs.write((char *)&buf, sizeof(buf));
		ofs.seekp(0, ios::end);//save the x
		buf.pointerToNext = -1;
		buf.record = x;
		ofs.write((char *)&buf, sizeof(buf));
		return true;
	}

	while (buf.pointerToNext != -1)
	{
		buf_p = buf.pointerToNext;
		ifs.seekg(buf.pointerToNext);
		ifs.read((char *)&buf, sizeof(buf));
	}
	buf.pointerToNext = ofs.tellp();//get the new node's position
	ofs.seekp(buf_p);
	ofs.write((char *)&buf, sizeof(buf));//update the node

	ofs.seekp(0, ios::end);//save the x
	buf.pointerToNext = -1;
	buf.record = x;
	ofs.write((char *)&buf, sizeof(buf));
	return true;
}

//not consider about the node that doesn't exist
template <class T, class S>
void HashTable<T, S>::deleteNode(T &x)
{
	int h = myhash(x);
	S buf,buf0;
	long buf_p;
	ifstream ifs(fileName, ios::in | ios::binary);
	ofstream ofs(fileName, ios_base::in | ios_base::out | ios::binary);
	ifs.seekg(h*sizeof(S));
	ifs.read((char *)&buf, sizeof(buf));//First node
	if (strcmp(buf.record.username, x.username) == 0 && buf.record.pointer == x.pointer)//check the first node
	{
		if (buf.pointerToNext == -2)//empty
		{
			memset(&buf, 0, sizeof(buf));
			buf.pointerToNext = -2;
			ofs.seekp(h*(sizeof(S)));
			ofs.write((char *)&buf, sizeof(buf));
		}
		else //next is not empty
		{
			memset(&(buf.record), 0, sizeof(buf.record));
			ofs.seekp(h*(sizeof(S)));
			ofs.write((char *)&buf, sizeof(buf));
		}
	}
	else
	{
		ifs.seekg(buf.pointerToNext);
		ifs.read((char *)&buf0, sizeof(buf0));
		if (strcmp(buf0.record.username, x.username) == 0 && buf0.record.pointer == x.pointer)
		{
			ifs.seekg(h*sizeof(S));
			ifs.read((char *)&buf, sizeof(buf));
			buf.pointerToNext = buf0.pointerToNext;
			ofs.seekp(h*sizeof(S));
			ofs.write((char *)&buf, sizeof(buf));
		}
	}
	while (buf.pointerToNext != -1)
	{
		buf_p = buf.pointerToNext;
		ifs.seekg(buf_p);
		ifs.read((char *)&buf, sizeof(buf));
		if (buf.pointerToNext != -1)
		{
			ifs.seekg(buf.pointerToNext);
			ifs.read((char *)&buf0, sizeof(buf0));
			if (strcmp(buf0.record.username, x.username) == 0 && buf0.record.pointer == x.pointer)
			{
				buf.pointerToNext = buf0.pointerToNext;
				ofs.seekp(buf_p);
				ofs.write((char *)&buf, sizeof(buf));
				return;
			}
		}
		else
		{
			if (strcmp(buf.record.username, x.username) == 0 && buf.record.pointer == x.pointer)
			{
				ifs.seekg(buf_p);
				ifs.read((char *)&buf, sizeof(buf));
				buf.pointerToNext = -1;
				ofs.seekp(buf_p);
				ofs.write((char *)&buf, sizeof(buf));
				return;
			}
		}
	}
}

template <class T, class S>
void HashTable<T, S>::begin()//可以一个链表一个链表的存储读取，可以尝试
{
	S buf;
	ifstream ifs(fileName, ios::in | ios::binary);
	if (ifs.peek() == EOF)
	{
		ofstream ofs(fileName, ios::out | ios::binary);
		for (int i = 0; i < currentSize; i++)
		{
			buf.pointerToNext = -2;
			ofs.write((char *)&buf, sizeof(buf));
		}
		return;
	}
}

template <class T, class S>
void HashTable<T, S>::dump()//bug
{
	//now there is no need to dump the data
	return;
}

#endif