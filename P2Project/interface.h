#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include "hash.h"
#include "user.h"
#include "graph.h"

using namespace std;

class Interface
{
public:
	Interface(bool saveflag);
	~Interface();
	void beginTowork();

	//for test
	void load();			//load before the program
	void login();			//login
	bool login(Information &info);
	void pushMessage(Message meg);
	void register_t(Information &newuser);
	void quitDo_offline();	//save after the quit
	bool likeSomeone(char *buf);		//like someone
	void changeName();
	void cancelLike(char *x);
	void deleteMessage(int number);
private:
	bool quitFlag,logFlag,saveflag;
	string cmd_off, cmd_on;
	HashTable<RecordNode, SaveForm> *hash;
	HashTable<RecordNode_name, SaveForm_name> *hash_name;
	HashTable<RecordNode_bir, SaveForm_bir> *hash_bir;
	HashTable<RecordNode_tele, SaveForm_tele> *hash_tele;
	HashTable<RecordNode_add, SaveForm_add> *hash_add;
	OLGraph *relationship;
	vector<Information> recommandPeople;
	User user;

	void getInput_offline();
	void getInput_online();
	void usage_offline();
	void usage_online();
	void commandExec_offline();
	void commandExec_online();
	void display_offline();
	void display_online();

	void register_m();		//register
	void deleteUser();
	void renewUser();

	void changeInfo();		//change something
	void searchSomeone();	//search
	void multipleSearch();	//multiple search
	void likeSomeone();		//like
	void cancelLike();
	void people();			//watch who like me and I like who
	void recommand();

	void watchSomeone();
	void shareMessage();	//share messages
	void pushMessage();		//push messages
	void showMessages();	//show the messages
	void showMyMessages();
	void showAllMessages();
	void deleteMessage();

	void onlineWork();		//online
	void work();

	void printInfo(Information &x);
	void clear();
};

#endif