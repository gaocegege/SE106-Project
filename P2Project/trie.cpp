#include "trie.h"

Trie::Trie()
:num(0)
{
	dic buf;
	memset(&buf, 0, sizeof(buf));
	node.push_back(buf);
}

void Trie::insert(char *str)
{
	int pos = 0, id;
	for (int i = 0; str[i]; i++)
	{
		id = str[i] - '0';
		if (node[pos].next[id] == 0)
			node[pos].next[id] = ++num;

		for (int i = node.size(); i <= num; i++)//ensure have enough node space
		{
			dic buf;
			memset(&buf, 0, sizeof(buf));
			node.push_back(buf);
		}

		pos = node[pos].next[id];
		node[pos].pass++;
	}
	node[pos].val = 1;
	//end
}
//查找匹配的个数
void Trie::search(char *str)
{
	int pos = 0, id;
	for (int i = 0; str[i]; i++)
	{
		id = str[i] - '0';
		if (node[pos].next[id] == 0)
		{
			printf("0\n");
			return;
		}
		pos = node[pos].next[id];
	}
	printf("%d\n", node[pos].pass);
}
//get the order
void Trie::pipei(int pos, char*str, int len)
{
	if (node[pos].val)
	{
		//output
		str[len] = '\0';
		cout << str << endl;//print
	}
	for (int i = 0; i<75; i++)
	{

		if (node[pos].next[i] != 0)
		{
			str[len++] = '0' + i;
			pipei(node[pos].next[i], str, len);
			len--;
		}
	}
}

void Trie::print()
{
	int pos = 0, id;
	int i = 0;
	for (i = 0; i < 75; i++)
	{
		id = i;
		char str[30] = "";
		pos = node[pos].next[id];

		if (node[pos].val)
			cout << str;
		int len = strlen(str);
		for (i = 0; i < 75; i++)
		{//get the order by 0-75
			if (node[pos].next[i] != 0)
			{
				str[len++] = '0' + i;
				pipei(node[pos].next[i], str, len);
				len--; 
			}
		}
	}
}

void Trie::test()
{
	int p, q, i;
	char str[30];

	while (scanf("%d", &p) != EOF)
	{

		for (i = 0; i<p; i++)
		{
			scanf("%s", str);
			insert(str);
		}
		print();
	}
}