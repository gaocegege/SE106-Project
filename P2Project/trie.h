#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct dic
{
	int next[75];//size of char 
	int pass;//times
	int val;//flag
};

class Trie
{
public:
	Trie();
	void search(char *str);
	void insert(char *str);
	void print();
	void test();
private:
	vector<dic> node;
	int num;

	void pipei(int pos, char*str, int len);
};

#endif