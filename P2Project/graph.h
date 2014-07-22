#ifndef GRAPH_H
#define GRAPH_H

#include "base.h"

typedef struct
{
	long tailvex;
	long headvex;
	long hlink;
	long tlink;
}OLGArc;

typedef struct
{
	char username[LENGTHOFID];
	long firstin;
	long firstout;
}OLGVNode;

class OLGraph
{
private:
	long pointer;
public:
	/*friend class Interface;*/
	void DeleteArc(long po1, long pointer);
	OLGraph();
	void load();
	void dump();
	long addVex(char *x);
	bool contains(long pointer);
	void DeleteVex();
	void InsertArc(long pointer);
	void DeleteArc(long pointer);
	void setPo(long pointer);
};

#endif