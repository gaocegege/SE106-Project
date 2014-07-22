#include "graph.h"
#include <string>
#include <fstream>

using namespace std;

OLGraph::OLGraph()//rewrite version
{
}

//ensure that the file available
void OLGraph::load()
{
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	if (ifs.peek() == EOF)
	{
		ofstream ofs(LIKEFILE, ios::out | ios::binary);
	}
}

//useless
void OLGraph::dump()
{
	return;
}

void OLGraph::setPo(long pointer)
{
	this->pointer = pointer;
}

//contains the arc(this->pointer, pointer);
bool OLGraph::contains(long pointer)
{
	OLGVNode buf_node;
	OLGArc buf_arc;
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ifs.seekg(this->pointer);
	ifs.read((char *)&buf_node, sizeof(buf_node));//get the node
	if (buf_node.firstout == -1)
		return false;
	else
	{
		ifs.seekg(buf_node.firstout);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
		while (buf_arc.tlink != -1)
		{
			if (buf_arc.headvex == pointer)
				return true;
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
		}
		if (buf_arc.headvex == pointer)
			return true;
		return false;
	}
}

//add new vex
long OLGraph::addVex(char *x)
{
	OLGVNode buf;
	long pointerToLike = 0;
	strcpy(buf.username, x);
	buf.firstin = buf.firstout = -1;
	ofstream ofs(LIKEFILE, ios::in | ios::out | ios::binary);
	ofs.seekp(0, ios::end);
	pointerToLike = ofs.tellp();
	if (pointerToLike == -1)
		pointerToLike = 0;
	ofs.write((char *)&buf, sizeof(buf));
	return pointerToLike;
}

//add the arc//constant time
void OLGraph::InsertArc(long pointer)//error//not sort as index//solved
{
	OLGArc p;
	OLGVNode q,q1;
	p.headvex = pointer;
	p.tailvex = this->pointer;
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ifs.seekg(this->pointer);
	ifs.read((char *)&q, sizeof(q));
	ifs.seekg(pointer);
	ifs.read((char *)&q1, sizeof(q1));
	p.tlink = q.firstout;
	p.hlink = q1.firstin;
	ofstream ofs(LIKEFILE, ios::in | ios::out | ios::binary);
	ofs.seekp(0, ios::end);
	q.firstout = ofs.tellp();
	q1.firstin = ofs.tellp();
	ofs.write((char *)&p, sizeof(p));
	ofs.seekp(this->pointer);
	ofs.write((char *)&q, sizeof(q));
	ofs.seekp(pointer);
	ofs.write((char *)&q1, sizeof(q1));
}

//delete the arc
void OLGraph::DeleteArc(long po1, long pointer)
{
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ofstream ofs(LIKEFILE, ios::in | ios::binary);
	OLGVNode buf_node;
	OLGArc buf_arc;
	OLGArc buf0;
	ifs.seekg(po1);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	ifs.seekg(buf_node.firstout);
	ifs.read((char *)&buf_arc, sizeof(buf_arc));
	if (buf_arc.headvex == pointer)
	{
		buf_node.firstout = buf_arc.tlink;
		ofs.seekp(po1);
		ofs.write((char *)&buf_node, sizeof(buf_node));
	}
	else
	{
		//if (buf_node.firstout != -1)
		//{
		long buf_l = buf_node.firstout;
		if (buf_arc.tlink != -1)
		{
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf0, sizeof(buf0));
		}
		while (buf_arc.tlink != -1)
		{
			if (buf0.headvex != pointer)
			{
				buf_l = buf_arc.tlink;
				ifs.seekg(buf_arc.tlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
				if (buf_arc.tlink != -1)
				{
					ifs.seekg(buf_arc.tlink);
					ifs.read((char *)&buf0, sizeof(buf0));
				}
			}
			else
				break;
		}
		if (buf0.headvex == pointer)
		{
			buf_arc.tlink = buf0.tlink;
			//UPDATE
			ofs.seekp(buf_l);//buf_l points to 96
			ofs.write((char *)&buf_arc, sizeof(buf_arc));
		}
		//maybe wrong↗//no
		//}
	}

	//deal with the head node
	ifs.seekg(pointer);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	ifs.seekg(buf_node.firstin);
	ifs.read((char *)&buf_arc, sizeof(buf_arc));
	if (buf_arc.tailvex == po1)
	{
		buf_node.firstin = buf_arc.hlink;
		ofs.seekp(pointer);
		ofs.write((char *)&buf_node, sizeof(buf_node));
	}
	else
	{
		if (buf_node.firstout != -1)
		{
			ifs.seekg(buf_node.firstin);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			long buf_l = buf_arc.hlink;
			if (buf_l != -1)
			{
				ifs.seekg(buf_l);
				ifs.read((char *)&buf0, sizeof(buf0));
			}
			while (buf_arc.hlink != -1)
			{
				if (buf0.tailvex != pointer)
				{
					buf_l = buf_arc.hlink;
					ifs.seekg(buf_arc.hlink);
					ifs.read((char *)&buf_arc, sizeof(buf_arc));
					if (buf_arc.hlink != -1)
					{
						ifs.seekg(buf_arc.hlink);
						ifs.read((char *)&buf0, sizeof(buf0));
					}
				}
				else
					break;
			}
			if (buf0.tailvex == pointer)
			{
				buf_arc.hlink = buf0.hlink;
				//UPDATE
				ofs.seekp(buf_l);
				ofs.write((char *)&buf_arc, sizeof(buf_arc));
			}
			//maybe wrong↗//no
		}
	}
}

//two likeones will bug//solved
void OLGraph::DeleteArc(long pointer)
{
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ofstream ofs(LIKEFILE, ios::in | ios::binary);
	OLGVNode buf_node;
	OLGArc buf_arc;
	OLGArc buf0;
	ifs.seekg(this->pointer);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	ifs.seekg(buf_node.firstout);
	ifs.read((char *)&buf_arc, sizeof(buf_arc));
	if (buf_arc.headvex == pointer)
	{
		buf_node.firstout = buf_arc.tlink;
		ofs.seekp(this->pointer);
		ofs.write((char *)&buf_node, sizeof(buf_node));
	}
	else
	{
		//if (buf_node.firstout != -1)
		//{
		long buf_l = buf_node.firstout;
		if (buf_arc.tlink != -1)
		{
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf0, sizeof(buf0));
		}
		while (buf_arc.tlink != -1)
		{
			if (buf0.headvex != pointer)
			{
				buf_l = buf_arc.tlink;
				ifs.seekg(buf_arc.tlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
				if (buf_arc.tlink != -1)
				{
					ifs.seekg(buf_arc.tlink);
					ifs.read((char *)&buf0, sizeof(buf0));
				}
			}
			else
				break;
		}
		if (buf0.headvex == pointer)
		{
			buf_arc.tlink = buf0.tlink;
			//UPDATE
			ofs.seekp(buf_l);//buf_l points to 96
			ofs.write((char *)&buf_arc, sizeof(buf_arc));
		}
		//maybe wrong↗
		//}
	}

	//deal with the head node
	ifs.seekg(pointer);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	ifs.seekg(buf_node.firstin);
	ifs.read((char *)&buf_arc, sizeof(buf_arc));
	if (buf_arc.tailvex == this->pointer)
	{
		buf_node.firstin = buf_arc.hlink;
		ofs.seekp(pointer);
		ofs.write((char *)&buf_node, sizeof(buf_node));
	}
	else
	{
		if (buf_node.firstout != -1)
		{
			ifs.seekg(buf_node.firstin);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			long buf_l = buf_arc.hlink;
			if (buf_l != -1)
			{
				ifs.seekg(buf_l);
				ifs.read((char *)&buf0, sizeof(buf0));
			}
			while (buf_arc.hlink != -1)
			{
				if (buf0.tailvex != pointer)
				{
					buf_l = buf_arc.hlink;
					ifs.seekg(buf_arc.hlink);
					ifs.read((char *)&buf_arc, sizeof(buf_arc));
					if (buf_arc.hlink != -1)
					{
						ifs.seekg(buf_arc.hlink);
						ifs.read((char *)&buf0, sizeof(buf0));
					}
				}
				else
					break;
			}
			if (buf0.tailvex == pointer)
			{
				buf_arc.hlink = buf0.hlink;
				//UPDATE
				ofs.seekp(buf_l);
				ofs.write((char *)&buf_arc, sizeof(buf_arc));
			}
			//maybe wrong↗
		}
	}
}

//delete the vex and the arc(s)
void OLGraph::DeleteVex()
{
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ofstream ofs(LIKEFILE, ios::in | ios::binary);
	OLGVNode buf_node;
	OLGArc buf_arc;
	OLGArc buf0;
	ifs.seekg(this->pointer);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	if (buf_node.firstout != -1)
	{
		ifs.seekg(buf_node.firstout);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
		while (buf_arc.tlink != -1)
		{
			DeleteArc(buf_arc.headvex);
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
		}
		DeleteArc(buf_arc.headvex);
	}

	if (buf_node.firstin != -1)
	{
		ifs.seekg(buf_node.firstin);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
		while (buf_arc.hlink != -1)
		{
			DeleteArc(buf_arc.tailvex, this->pointer);
			ifs.seekg(buf_arc.hlink);//loop
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
		}
		DeleteArc(buf_arc.tailvex, this->pointer);
	}
}