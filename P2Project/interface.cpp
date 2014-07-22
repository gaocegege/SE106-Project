#include "interface.h"
#include <iostream>
#include <time.h>
#include "information.h"
#include "message.h"
#include "trie.h"

using namespace std;

Interface::Interface(bool saveflag)
:cmd_off(""), quitFlag(false), cmd_on(""), saveflag(saveflag)
{
	int buf = 0;
	ifstream ifs(STATEFILE, ios::in | ios::binary);
	if (ifs.peek() == EOF)
	{
		ofstream ofs(STATEFILE, ios::out | ios::binary);
		ofs.write((char *)&buf, sizeof(buf));
		ofs.close();
	}
	ifs.close();
	relationship = new OLGraph();
	hash = new HashTable<RecordNode, SaveForm>(INDEXFILE);
	hash_name = new HashTable<RecordNode_name, SaveForm_name>(NAMEINDEXFILE);
	hash_bir = new HashTable<RecordNode_bir, SaveForm_bir>(BIRINDEXFILE);
	hash_tele = new HashTable<RecordNode_tele, SaveForm_tele>(TELEINDEXFILE);
	hash_add = new HashTable<RecordNode_add, SaveForm_add>(ADDINDEXFILE);
}

Interface::~Interface()
{
	delete hash;
	delete hash_name;
	delete hash_bir;
	delete hash_tele;
	delete hash_add;
	delete relationship;
}

void Interface::getInput_offline()//get the command
{
	cin >> cmd_off;
	commandExec_offline();
}

void Interface::getInput_online()
{
	cin >> cmd_on;
	commandExec_online();
}

void Interface::usage_offline()//output the usage_offline
{
	cout << "usage_offline:\n"//print all the command that the user can input
		<< "-h\tHelp\n"
		<< "-l\tLog in\n"
		<< "-q\tQuit\n"
		<< "-r\tregister\n"
		<< "-n\trenew the user\n"
		/*add more cmd_off supported*/
		<< "\n";
}

void Interface::usage_online()
{
	cout << "usage_online:\n"
		<< "-h\tHelp\n"
		<< "-q\tQuit\n"
		<< "-c\tChange the user info\n"
		<< "-d\tDelete the user\n"
		<< "-p\tPush a message\n"
		<< "-a\tShare a message\n"
		<< "-s\tShow the messages\n"
		<< "-e\tErase a message\n"
		<< "-l\tLike someone\n"
		<< "-w\twatch who like you\n"
		<< "-u\tUnlike the person\n"
		<< "-f\tFind someone\n"
		/*add more cmd_on supported*/
		<< "\n";
}

void Interface::commandExec_offline()//execute the command
{
	if (cmd_off == "-h")
		usage_offline();
	else if (cmd_off == "-q")
	{
		quitFlag = true;
		cout << "Bye Bye!\n";
	}
	else if (cmd_off == "-l")
	{
		login();
	}
	else if (cmd_off == "-r")
	{
		register_m();
	}
	else if (cmd_off == "-n")
	{
		renewUser();
	}
	else
	{
		cout << "The command is not valid.\n\n";
		usage_offline();
		cout << "\n";
	}
}

void Interface::commandExec_online()
{
	if (cmd_on == "-h")
	{
		usage_online();
	}
	else if (cmd_on == "-q")
	{
		logFlag = false;
	}
	else if (cmd_on == "-p")
	{
		pushMessage();
	}
	else if (cmd_on == "-s")
	{
		showMessages();
	}
	else if (cmd_on == "-l")
	{
		likeSomeone();
	}
	else if (cmd_on == "-c")
	{
		changeInfo();
	}
	else if (cmd_on == "-e")
	{
		deleteMessage();
	}
	else if (cmd_on == "-w")
	{
		people();
	}
	else if (cmd_on == "-d")
	{
		deleteUser();
	}
	else if (cmd_on == "-u")
	{
		cancelLike();
	}
	else if (cmd_on == "-f")
	{
		searchSomeone();
	}
	else if (cmd_on == "-a")
	{
		shareMessage();
	}
	/*add more cmd*/
	else
		cout << "command error\n";
}

void Interface::display_offline()//output something
{
	cout << "Welcome to the system. I'm Cece, and I will be your guide in the system.\n"
		<< "Hope you happy:D\n"
		<< "The system allows you to read what your friends push to the system, and you can push,too.\n"
		<< "Let's begin to play!\n\n";
	int userNum = 0;
	ifstream ifs(STATEFILE, ios::in | ios::binary);
	ifs.read((char *)&userNum, sizeof(userNum));
	cout << "Total number of users: " << userNum << endl;
}

void Interface::display_online()
{
	cout << "Login successfully!\n"
		<< "Now you can deal with your information and messages and so on\n";
}

void Interface::register_m()
{
	Information newuser;
	memset(&newuser, 0, sizeof(Information));//set 0
	char test[LENGTHOFPWD];
	int number = 0;
	RecordNode buf;
	RecordNode_name buf_n;
	RecordNode_bir buf_b;

	cout << "please input the new username:(1-20 chars)\n";
	fflush(stdin);
	newuser.identification[LENGTHOFID - 1] = '\0';
	cin >> newuser.identification;
	for (int i = 0; i < strlen(newuser.identification); i++)//low perf
	{
		if (!((newuser.identification[i] >= 48 && newuser.identification[i] <= 57) || (newuser.identification[i] >= 65 && newuser.identification[i] <= 90) || (newuser.identification[i] >= 97 && newuser.identification[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(newuser.identification, 0, sizeof(newuser.identification));
			return;
		}
	}
	cin.clear();

	//check whether the name is in the list, if not, then
	if (hash->contains(newuser.identification).pointer != -2)
	{
		cout << "The username has been used\n";
		return;
	}

	cout << "The username is still available.please input the password(1-32 chars):\n";
	fflush(stdin);
	cin >> newuser.password;
	newuser.password[LENGTHOFPWD - 1] = '\0';
	for (int i = 0; i < strlen(newuser.password); i++)//low perf
	{
		if (!((newuser.password[i] >= 48 && newuser.password[i] <= 57) || (newuser.password[i] >= 65 && newuser.password[i] <= 90) || (newuser.password[i] >= 97 && newuser.password[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(newuser.password, 0, sizeof(newuser.password));
			return;
		}
	}
	cin.clear();

	//if the pwd meet the need, then
	cout << "please input the password again:\n";
	fflush(stdin);
	cin >> test;
	test[LENGTHOFPWD - 1] = '\0';
	for (int i = 0; i < strlen(test); i++)//low perf
	{
		if (!((test[i] >= 48 && test[i] <= 57) || (test[i] >= 65 && test[i] <= 90) || (test[i] >= 97 && test[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			return;
		}
	}
	cin.clear();

	if (strcmp(newuser.password, test) != 0)//should have a loop to determine
	{
		cout << "The password is wrong, the login is failed\n";
		return;
	}

	cout << "Name\n";
	fflush(stdin);
	cin >> newuser.name;
	newuser.name[LENGTHOFNAME - 1] = '\0';
	for (int i = 0; i < strlen(newuser.name); i++)//low perf
	{
		if (!((newuser.name[i] >= 65 && newuser.name[i] <= 90) || (newuser.name[i] >= 97 && newuser.name[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(newuser.name, 0, sizeof(newuser.name));
			return;
		}
	}
	cin.clear();

	cout << "Gender:(1 == man, 0 == woman)\n";
	fflush(stdin);
	cin >> test;
	test[1] = '\0';
	if (test[0] == '1' || strlen(test) == 1)
		newuser.gender = 1;
	else if (test[0] == '0' || strlen(test) == 1)
		newuser.gender = 0;
	else
	{
		cout << "Formal Error\n";
		return;
	}
	cout << "Birthday:(XXXX-XX-XX)\n";
	memset(newuser.birthday, 0,sizeof(newuser.birthday));
	cout << "Year: (XXXX)\n";
	char year[5];
	cin >> year;
	if (year[0] != '1' || (year[1] != '8' && year[1] != '9' && year[1] != '0') || strlen(year) < 4 || (year[2] < '0' || year[2] > '9') || (year[3] < '0' || year[3] > '9'))
	{
		cout << "Formal error\n";
		return;
	}
	strncpy(newuser.birthday, year, 4);
	strcat(newuser.birthday, "-");
	char month[3];
	cout << "Month: (XX)\n";
	cin >> month;
	if (strlen(month) != 2 || (month[0] != '1' && month[0] != '0') || (month[1] < '0' || month[1] > '9'))
	{
		cout << "Formal error\n";
		return;
	}
	strncat(newuser.birthday, month, 2);
	strcat(newuser.birthday, "-");
	char date[3];
	cout << "Date: (xx)\n";
	cin >> date;
	if (strlen(date) != 2 || (date[0] != '1' && date[0] != '0') || (date[1] < '0' || date[1] > '9'))
	{
		cout << "Formal error\n";
		return;
	}
	strncat(newuser.birthday, date, 2);

	ifstream ifs(STATEFILE, ios::in | ios::binary);
	ifs.read((char *)&number, sizeof(number));
	ifs.close();

	ofstream ofs1(STATEFILE, ios::out | ios::binary);
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);

	//get the pointer
	ofs.seekp(0, ios::end);
	buf.pointer = ofs.tellp();//buf = -1~?
	buf_n.pointer = buf.pointer;
	buf_b.pointer = buf.pointer;
	if (buf.pointer == -1)
	{
		buf_n.pointer = 0;
		buf_b.pointer = 0;
		buf.pointer = 0;
	}
	ofs.close();

	//save the data
	ofs.open(INFOFILE, ios::out | ios::binary | ios::app);
	strcpy_s(buf.username, newuser.identification);
	strcpy_s(buf_n.username, newuser.name);
	strcpy_s(buf_b.username, newuser.birthday);
	newuser.pointer = 1;
	newuser.isAvailable = true;
	number++;
	hash->insert(buf);
	hash_name->insert(buf_n);
	hash_bir->insert(buf_b);
	newuser.pointerToLike = relationship->addVex(buf.username);
	ofs1.write((char *)&number, sizeof(number));
	ofs.write((char *)(&newuser), sizeof(Information));
	cout << "Congratulation!\n\n";
}

//don't add the name and bir to the hash index
void Interface::register_t(Information &newuser)//not test
{
	int number = 0;
	RecordNode buf;
	RecordNode_name buf_n;
	RecordNode_bir buf_b;
	if (hash->contains(newuser.identification).pointer != -2)
	{
		return;
	}

	ifstream ifs(STATEFILE, ios::in | ios::binary);
	ifs.read((char *)&number, sizeof(number));
	ifs.close();

	ofstream ofs1(STATEFILE, ios::out | ios::binary);
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);

	//get the pointer
	ofs.seekp(0, ios::end);
	buf.pointer = ofs.tellp();//buf = -1~?
	buf_n.pointer = buf.pointer;//nameIndex
	buf_b.pointer = buf.pointer;//BirIndex
	if (buf.pointer == -1)
		buf.pointer = 0;
	ofs.close();

	//save the data
	ofs.open(INFOFILE, ios::out | ios::binary | ios::app);
	strcpy_s(buf.username, newuser.identification);
	strcpy_s(buf_n.username, newuser.name);
	strcpy_s(buf_b.username, newuser.birthday);
	newuser.pointer = 1;
	newuser.isAvailable = true;
	number++;
	hash->insert(buf);
	hash_name->insert(buf_n);
	hash_bir->insert(buf_b);
	newuser.pointerToLike = relationship->addVex(buf.username);
	ofs1.write((char *)&number, sizeof(number));
	ofs.write((char *)(&newuser), sizeof(Information));
}

void Interface::login()
{
	Information newuser;
	Information ref;
	cout << "Input the username, please.\n";
	fflush(stdin);
	cin.getline(newuser.identification, LENGTHOFID);
	for (int i = 0; i < strlen(newuser.identification); i++)//low perf
	{
		if (!((newuser.identification[i] >= 48 && newuser.identification[i] <= 57) || (newuser.identification[i] >= 65 && newuser.identification[i] <= 90) || (newuser.identification[i] >= 97 && newuser.identification[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(newuser.identification, 0, sizeof(newuser.identification));
			return;
		}
	}
	cin.clear();

	cout << "Please input the password.\n";
	cin.getline(newuser.password, LENGTHOFPWD);
	for (int i = 0; i < strlen(newuser.password); i++)//low perf
	{
		if (!((newuser.password[i] >= 48 && newuser.password[i] <= 57) || (newuser.password[i] >= 65 && newuser.password[i] <= 90) || (newuser.password[i] >= 97 && newuser.password[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(newuser.password, 0, sizeof(newuser.password));
			return;
		}
	}
	cin.clear();

	RecordNode r_newuser = hash->contains(newuser.identification);
	if (r_newuser.pointer == -2)
	{
		cout << "The username is wrong!\n";
		return;
	}

	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_newuser.pointer);
	ifs.read((char *)(&ref), sizeof(Information));
	if (ref.isAvailable == false)//whether is available
	{
		cout << "The user has been deleted\n";
		return;
	}
	user.userInfo = ref;//maybe wrong//no = operator~?
	user.pointerToData = r_newuser.pointer;
	relationship->setPo(ref.pointerToLike);
	//TO DO
	if (strcmp(ref.password, newuser.password) == 0)
	{
		cout << "Successful!\n";
		logFlag = true;
	}
	else
		cout << "The password is wrong~\n";
}

//used for the test mode 
bool Interface::login(Information &info)
{
	Information ref;
	RecordNode r_newuser = hash->contains(info.identification);
	if (r_newuser.pointer == -2)
	{
		cout << "The username is wrong!\n";
		return false;
	}

	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_newuser.pointer);
	ifs.read((char *)(&ref), sizeof(Information));
	if (ref.isAvailable == false)
	{
		cout << "The user has been deleted\n";
		return false;
	}
	user.userInfo = ref;//maybe wrong//no = operator~?
	user.pointerToData = r_newuser.pointer;
	relationship->setPo(ref.pointerToLike);
	//TO DO
	if (strcmp(ref.password, info.password) == 0)
	{
		cout << "Successful!\n";
		return true;
	}
	return false;
}

void Interface::deleteUser()
{
	char buf[LENGTHOFPWD];
	cout << "Please input your password\n";
	cin >> buf;
	if (strcmp(buf, user.userInfo.password) != 0)
	{
		cout << "The pwd is wrong.\n";
		return;
	}
	user.userInfo.isAvailable = false;//set false but don't delete
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);
	ofs.seekp(user.pointerToData);
	ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
	relationship->DeleteVex();
	logFlag = false;
	cout << "Successfully~\n";
}

void Interface::renewUser()
{
	char buf[LENGTHOFID];
	char buf_p[LENGTHOFPWD];
	Information ref;
	cout << "Please input your username\n";
	cin.getline(buf, LENGTHOFID);
	cin.getline(buf, LENGTHOFID);
	for (int i = 0; i < strlen(buf); i++)//low perf
	{
		if (!((buf[i] >= 48 && buf[i] <= 57) || (buf[i] >= 65 && buf[i] <= 90) || (buf[i] >= 97 && buf[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(buf, 0, sizeof(buf));
			return;
		}
	}
	cin.clear();

	RecordNode r_newuser = hash->contains(buf);
	if (r_newuser.pointer == -2)
	{
		cout << "The username is wrong!\n";
		return;
	}

	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_newuser.pointer);
	ifs.read((char *)(&ref), sizeof(Information));

	if (ref.isAvailable == true)
	{
		cout << "The user is still available\n";
	}

	cout << "Please input your password\n";
	cin.getline(buf_p, LENGTHOFPWD);
	for (int i = 0; i < strlen(buf_p); i++)//low perf
	{
		if (!((buf_p[i] >= 48 && buf_p[i] <= 57) || (buf_p[i] >= 65 && buf_p[i] <= 90) || (buf_p[i] >= 97 && buf_p[i] <= 122)))
		{
			cout << "Formal error\n";
			cin.clear();
			memset(buf_p, 0, sizeof(buf_p));
			return;
		}
	}
	cin.clear();
	if (strcmp(buf_p, ref.password) != 0)
	{
		cout << "The pwd is wrong\n";
		return;
	}

	ref.isAvailable = true;//set true
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);
	ofs.seekp(r_newuser.pointer);
	ofs.write((char *)&ref, sizeof(ref));
	cout << "Successfully~\n";
}

void Interface::load()
{
	hash->begin();
	hash_name->begin();
	hash_bir->begin();
	hash_tele->begin();
	hash_add->begin();
	relationship->load();

	ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
	if (ifs.peek() == EOF)
		ofstream ofs(MESSAGEFILE, ios::out | ios::binary);
}

void Interface::quitDo_offline()
{
	//memory to disk
	relationship->dump();
}

void Interface::onlineWork()
{
	display_online();
	usage_online();
	while (1)
	{
		if (logFlag == false)
		{
			cout << "Log out successfully~\n\n";
			usage_offline();
			break;
		}
		getInput_online();
	}
}

void Interface::pushMessage(Message meg)
{
	time_t t = time(NULL);
	meg.localTime = *localtime(&t);
	meg.pointerToNext = 1;
	meg.pointerToS = 1;
	meg.sharedTimes = 0;
	meg.pointerTo0 = 1;
	meg.isShared = 0;

	if (user.userInfo.pointer == 1)
	{
		ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(0, ios::end);
		user.userInfo.pointer = ofs.tellp();//get first
		if (user.userInfo.pointer == -1)
			user.userInfo.pointer = 0;
		ofs.write((char *)&meg, sizeof(meg));
		ofs.close();
		ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(user.pointerToData);
		ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
	}
	else
	{
		ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(0, ios::end);
		meg.pointerToNext = user.userInfo.pointer;
		user.userInfo.pointer = ofs.tellp();
		ofs.write((char *)&meg, sizeof(meg));
		ofs.close();
		ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(user.pointerToData);
		ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
	}

	cout << "Successfully~\nWhat else do you want to do~\n";
}

void Interface::pushMessage()
{
	if (saveflag == true)
	{
		Message meg_buf;
		int number = 0;
		char ch;
		cout << "Now you can push a message no more than 140 chars\n";
		cin.get(ch);
		cin.get(ch);
		while (ch != '\n' && number < LENGTHOFMESSAGE - 1)
		{
			meg_buf.message[number] = ch;
			number++;
			cin.get(ch);
		}
		meg_buf.message[number] = '\0';
		fflush(stdin);

		time_t t = time(NULL);
		meg_buf.localTime = *localtime(&t);
		meg_buf.pointerToNext = 1;
		meg_buf.pointerToS = 1;
		meg_buf.sharedTimes = 0;
		meg_buf.pointerTo0 = 1;
		meg_buf.isShared = 0;
		
		if (user.userInfo.pointer == 1)
		{
			ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
			ofs.seekp(0, ios::end);
			user.userInfo.pointer = ofs.tellp();//get first
			if (user.userInfo.pointer == -1)
			{
				user.userInfo.pointer = 0;
				ofs.seekp(0);
			}
			ofs.write((char *)&meg_buf, sizeof(meg_buf));
			ofs.close();
			ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
			ofs.seekp(user.pointerToData);
			ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
		}
		else
		{
			ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
			ofs.seekp(0, ios::end);
			meg_buf.pointerToNext = user.userInfo.pointer;
			user.userInfo.pointer = ofs.tellp();
			ofs.write((char *)&meg_buf, sizeof(meg_buf));
			ofs.close();
			ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
			ofs.seekp(user.pointerToData);
			ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
		}

		cout << "Successfully~\nWhat else do you want to do~\n";
	}
}

void Interface::watchSomeone()
{
	Message meg;
	Information ref;
	char buf_u[LENGTHOFID];
	cout << "Please input the username\n";
	cin >> buf_u;
	RecordNode r_buf = hash->contains(buf_u);
	if (r_buf.pointer == -2)
	{
		cout << "The username is wrong!\n";
		return;
	}

	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_buf.pointer);
	ifs.read((char *)(&ref), sizeof(Information));
	ifs.close();

	ifs.open(MESSAGEFILE, ios::in | ios::binary);
	ifs.seekg(ref.pointer);
	if (ref.pointer != 1)
		ifs.read((char *)&meg, sizeof(meg));
	else
	{
		cout << "You have no messages\n";
		return;
	}
	int number = 0, sharedTimes = 0;
	while (meg.pointerToNext != 1)
	{
		if (meg.isShared == 1 && meg.pointerToS != 1)//is shared
		{
			cout << number << ".\t";
			meg.printMegTime();
			cout << meg.message << "  Shared from " << meg.username << ": ";
			ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
			Message meg_buf;
			ifs.seekg(meg.pointerToS);
			ifs.read((char *)&meg_buf, sizeof(meg_buf));
			while (meg_buf.pointerToS != 1)
			{
				if (meg_buf.isShared == 1)
				{
					cout << meg_buf.message << "  Shared from " << meg_buf.username << ": ";
					ifs.seekg(meg_buf.pointerToS);//the next
					ifs.read((char *)&meg_buf, sizeof(meg_buf));
				}
				else
					break;
			}
			sharedTimes = meg_buf.sharedTimes;
			cout << meg_buf.message << " ( The message has been shared " << sharedTimes << " times )" << endl;
		}
		else
		{
			cout << number << ".\t";
			meg.printMegTime();
			cout << meg.message << endl;
		}
		ifs.seekg(meg.pointerToNext);
		ifs.read((char *)&meg, sizeof(meg));
		number++;
	}
	//todo
	if (meg.isShared == 1 && meg.pointerToS != 1)//is shared
	{
		cout << number << ".\t";
		meg.printMegTime();
		cout << meg.message << "  Shared from " << meg.username << ": ";
		ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
		Message meg_buf;
		ifs.seekg(meg.pointerToS);
		ifs.read((char *)&meg_buf, sizeof(meg_buf));
		while (meg_buf.pointerToS != 1)
		{
			if (meg_buf.isShared == 1)
			{
				cout << meg_buf.message << "  Shared from " << meg_buf.username << ": ";
				ifs.seekg(meg_buf.pointerToS);//the next
				ifs.read((char *)&meg_buf, sizeof(meg_buf));
			}
			else
				break;
		}
		sharedTimes = meg_buf.sharedTimes;
		cout << meg_buf.message << " ( The message has been shared " << sharedTimes << " times )" << endl;
	}
	else
	{
		cout << number << ".\t";
		meg.printMegTime();
		cout << meg.message << endl;
	}
	
}

void Interface::showMessages()
{
	char cmd;
	cout << "You can \n"
		<< "0. Watch your own essages\n"
		<< "1. Watch all the messages\n"
		<< "2.watch someone's messages\n";
	cin >> cmd;
	switch (cmd)
	{
	case '0':
		showMyMessages();
		break;
	case '1':
		showAllMessages();
		break;
	case '2':
		watchSomeone();
		break;
	default:
		cout << "Error:The cmd is wrong\n";
		break;
	}
}

void Interface::showMyMessages()
{
	Message meg;
	cout << "The Messages:\n";
	ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
	ifs.seekg(user.userInfo.pointer);
	if (user.userInfo.pointer != 1)
		ifs.read((char *)&meg, sizeof(meg));
	else
	{
		cout << "You have no messages\n";
		return;
	}
	int number = 0, sharedTimes = 0;
	while (meg.pointerToNext != 1)
	{
		if (meg.isShared == 1 && meg.pointerToS != 1)//is shared
		{
			cout << number << ".\t";
			meg.printMegTime();
			cout << meg.message << "  Shared from " << meg.username << ": ";
			ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
			Message meg_buf;
			ifs.seekg(meg.pointerToS);
			ifs.read((char *)&meg_buf, sizeof(meg_buf));
			while (meg_buf.pointerToS != 1)
			{
				if (meg_buf.isShared == 1)
				{
					cout << meg_buf.message << "  Shared from " << meg_buf.username << ": ";
					ifs.seekg(meg_buf.pointerToS);//the next
					ifs.read((char *)&meg_buf, sizeof(meg_buf));
				}
				else
					break;
			}
			sharedTimes = meg_buf.sharedTimes;
			cout << meg_buf.message << " ( The message has been shared " << sharedTimes << " times )" << endl;
		}
		else
		{
			cout << number << ".\t";
			meg.printMegTime();
			cout << meg.message << endl;
		}
		ifs.seekg(meg.pointerToNext);
		ifs.read((char *)&meg, sizeof(meg));
		number++;
	}
	//todo
	if (meg.isShared == 1 && meg.pointerToS != 1)//is shared
	{
		cout << number << ".\t";
		meg.printMegTime();
		cout << meg.message << "  Shared from " << meg.username << ": ";
		ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
		Message meg_buf;
		ifs.seekg(meg.pointerToS);
		ifs.read((char *)&meg_buf, sizeof(meg_buf));
		while (meg_buf.pointerToS != 1)
		{
			if (meg_buf.isShared == 1)
			{
				cout << meg_buf.message << "  Shared from " << meg_buf.username << ": ";
				ifs.seekg(meg_buf.pointerToS);//the next
				ifs.read((char *)&meg_buf, sizeof(meg_buf));
			}
			else
				break;
		}
		sharedTimes = meg_buf.sharedTimes;
		cout << meg_buf.message << " ( The message has been shared " << sharedTimes << " times )" << endl;
	}
	else
	{
		cout << number << ".\t";
		meg.printMegTime();
		cout << meg.message << endl;
	}
}

void Interface::showAllMessages()
{
	RecordNode buf;
	OLGArc buf_arc;
	OLGVNode buf_node;
	Information info;
	Message meg,buf_meg;
	vector<Message> listOfMeg, listOfIn;
	vector<long> v_p;//save the pointer to the message
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ifstream ifs0(INFOFILE, ios::in | ios::binary);
	ifstream ifs1(MESSAGEFILE, ios::in | ios::binary);

	int number = 0, shareTimes = 0;
	bool isFinished = false;
	memset(&meg, 0, sizeof(meg));//set 0

	//add yourself to the list
	v_p.push_back(user.userInfo.pointer);//push the newest node

	ifs.seekg(user.userInfo.pointerToLike);//find the like
	ifs.read((char *)&buf_node, sizeof(buf_node));//the user node
	if (buf_node.firstout != -1)//have liked someone
	{
		ifs.seekg(buf_node.firstout);//find the first likeone
		ifs.read((char *)&buf_arc, sizeof(buf_arc));//find the first likeone
		while (buf_arc.tlink != -1)//has another likeone
		{
			ifs.seekg(buf_arc.headvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));

			buf = hash->contains(buf_node.username);
			ifs0.seekg(buf.pointer);
			ifs0.read((char *)&info, sizeof(info));

			//to get the last node
			v_p.push_back(info.pointer);//push the newest node

			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
		}
		ifs.seekg(buf_arc.headvex);
		ifs.read((char *)&buf_node, sizeof(buf_node));

		buf = hash->contains(buf_node.username);
		ifs0.seekg(buf.pointer);
		ifs0.read((char *)&info, sizeof(info));

		v_p.push_back(info.pointer);

		ifs.seekg(buf_arc.tlink);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
	}

	// to get the first node
	for (int i = 0; i < v_p.size(); i++)
	{
		if (v_p[i] == 1)
		{
			v_p.erase(v_p.begin() + i);//maybe wrong
			continue;
		}
		ifs1.seekg(v_p[i]);
		ifs1.read((char *)&buf_meg, sizeof(buf_meg));
		if (meg.earlierThan(buf_meg))
		{
			meg = buf_meg;
		}
	}

	while (1)
	{
		if (v_p.size() == 0)
		{
			break;
		}

		for (int i = 0; i < v_p.size(); i++)
		{
			if (v_p[i] == 1)
			{
				v_p.erase(v_p.begin() + i);//maybe wrong
				continue;
			}
			ifs1.seekg(v_p[i]);
			ifs1.read((char *)&buf_meg, sizeof(buf_meg));
			if (meg.earlierThan(buf_meg))
			{
				v_p[i] = buf_meg.pointerToNext;
				listOfMeg.push_back(buf_meg);
				meg = buf_meg;
			}
		}

		//set 0
		memset(&meg, 0, sizeof(meg));
	}

	//output
	memset(&buf_meg, 0, sizeof(buf_meg));
	for (int i = 0; i < listOfMeg.size(); i++)
	{
		if (i % 10 == 0 && i != 0)
		{
			cout << "Continue(c) or quit(q)\n";
			char cmd;
			cin >> cmd;
			if (cmd == 'q')
				break;
		}
		if (listOfMeg[i].isShared == 1 && listOfMeg[i].pointerToS != 1)//is shared
		{
			ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
			ifs.seekg(listOfMeg[i].pointerToS);
			Message meg;
			ifs.read((char *)&meg, sizeof(meg));
			while (meg.pointerToS != 1 && meg.isShared == 1)
			{
				ifs.seekg(meg.pointerToS);//the next
				ifs.read((char *)&meg, sizeof(meg));
			}
			if (listOfIn.size() == 0)
			{
				listOfIn.push_back(meg);
				cout << i << ".\t";
				listOfMeg[i].printMegTime();
				cout << listOfMeg[i].message << "  Shared from " << listOfMeg[i].username << ": ";
				ifs.seekg(listOfMeg[i].pointerToS);
				ifs.read((char *)&meg, sizeof(meg));
				while (meg.pointerToS != 1 && meg.isShared == 1)
				{
					cout << meg.message << "  Shared from " << meg.username << ": ";
					ifs.seekg(meg.pointerToS);//the next
					ifs.read((char *)&meg, sizeof(meg));
				}
				cout << meg.message << "(The message has been shared " << meg.sharedTimes << ")" << endl;
			}
			else
			{
				bool isIn = false;
				for (int j = 0; j < listOfIn.size(); j++)
				{
					if (strcmp(listOfIn[j].message, meg.message) == 0)
					{
						isIn = true;
						break;
					}
				}
				if (isIn == false)
				{
					listOfIn.push_back(meg);
					cout << i << ".\t";
					listOfMeg[i].printMegTime();
					cout << listOfMeg[i].message << "  Shared from " << listOfMeg[i].username << ": ";
					ifs.seekg(listOfMeg[i].pointerToS);
					ifs.read((char *)&meg, sizeof(meg));
					while (meg.pointerToS != 1 && meg.isShared == 1)
					{
						cout << meg.message << "  Shared from " << meg.username << ": ";
						ifs.seekg(meg.pointerToS);//the next
						ifs.read((char *)&meg, sizeof(meg));
					}
					cout << meg.message << "(The message has been shared " << meg.sharedTimes << ")" << endl;
				}
			}
			//cout << buf_meg.message << "(The message has been shared " << shareTimes << ")" << endl;
		}
		else
		{
			bool isIn = false;
			for (int j = 0; j < listOfIn.size(); j++)
			{
				if (strcmp(listOfIn[j].message, listOfMeg[i].message) == 0)
				{
					isIn = true;
					break;
				}
			}
			if (isIn == false)
			{
				cout << i << ".\t";
				listOfMeg[i].printMegTime();
				cout << listOfMeg[i].message << endl;
			}
		}
	}
	cout << "Over~\nWhat else do you want to do~?\n";
}

void Interface::shareMessage()
{
	cout << "username: ";
	char buf[LENGTHOFID];
	int number = 0, buf_i = 0;
	cin >> buf;
	RecordNode buf_r;
	Information info;
	Message meg,sharedMeg;
	buf_r = hash->contains(buf);
	if (buf_r.pointer == -2)
	{
		cout << "The username doesn't exist\n";
	}
	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(buf_r.pointer);
	ifs.read((char *)&info, sizeof(info));
	ifs.close();
	ifs.open(MESSAGEFILE, ios::in | ios::binary);
	ifs.seekg(info.pointer);
	ifs.read((char *)&meg, sizeof(meg));

	while (meg.pointerToNext != 1)
	{
		cout << number << ". ";
		meg.printMegTime();
		cout << meg.message << endl;
		number++;
		ifs.seekg(meg.pointerToNext);
		ifs.read((char *)&meg, sizeof(meg));
	}
	cout << number << ". " << meg.message << endl;
	cout << "Input the number: \n";
	cin >> number;
	ifs.seekg(info.pointer);
	ifs.read((char *)&meg, sizeof(meg));
	if (number == 0)
	{
		buf_i = info.pointer;
	}
	for (int i = 0; i < number; i++)
	{
		if (i == number - 1)
			buf_i = meg.pointerToNext;
		ifs.seekg(meg.pointerToNext);
		ifs.read((char *)&meg, sizeof(meg));
	}
	cout << "The message:\t" << meg.message << endl;
	cout << "You can have some comments\n";

	//get the comments
	char commentBuf[LENGTHOFMESSAGE];
	char ch;
	int number_b = 0;
	cin.get(ch);
	cin.get(ch);
	while (ch != '\n' && number_b < LENGTHOFMESSAGE - 1)
	{
		commentBuf[number_b] = ch;
		number_b++;
		cin.get(ch);
	}
	commentBuf[number_b] = '\0';
	fflush(stdin);

	//cat the comments and content, now there is no need to do so
	/*strncat(commentBuf, meg.message, LENGTHOFMESSAGE - strlen(commentBuf) - 1);*/
	cout << commentBuf << endl;
	strcpy(sharedMeg.message, commentBuf);

	cout << "Comment successfuly\n";

	time_t t = time(NULL);
	sharedMeg.localTime = *localtime(&t);
	/*strcpy(sharedMeg.message, meg.message);*/
	strcpy(sharedMeg.username, buf);
	sharedMeg.isShared = 1;
	sharedMeg.pointerToS = buf_i;//err
	sharedMeg.pointerToNext = 1;
	
	while (meg.pointerToS != 1)
	{
		buf_i = meg.pointerToS;
		ifs.seekg(meg.pointerToS);
		ifs.read((char *)&meg, sizeof(meg));
	}

	meg.sharedTimes++;
	ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
	ofs.seekp(buf_i);
	ofs.write((char *)&meg, sizeof(meg));

	sharedMeg.pointerTo0 = buf_i;

	if (user.userInfo.pointer == 1)
	{
		ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(0, ios::end);
		user.userInfo.pointer = ofs.tellp();//get first
		if (user.userInfo.pointer == -1)
			user.userInfo.pointer = 0;
		ofs.write((char *)&sharedMeg, sizeof(sharedMeg));
		ofs.close();
		ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(user.pointerToData);
		ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
	}
	else
	{
		ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(0, ios::end);
		sharedMeg.pointerToNext = user.userInfo.pointer;
		user.userInfo.pointer = ofs.tellp();
		ofs.write((char *)&sharedMeg, sizeof(sharedMeg));
		ofs.close();
		ofs.open(INFOFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(user.pointerToData);
		ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
	}
}

void Interface::deleteMessage()
{
	int number;
	Message meg;
	ifstream ifs(MESSAGEFILE, ios::in | ios::binary);
	showMyMessages();
	cout << "Please input the number you want to delete\n, if don't, input -1\n";
	cin >> number;
	if (number == -1)
	{
		cout << "Over~\n";
		return;
	}
	else
	{
		ifs.seekg(user.userInfo.pointer);
		ifs.read((char *)&meg, sizeof(meg));
		if (number == 0)
		{
			ofstream ofs(INFOFILE, ios::in | ios::out | ios::binary);
			user.userInfo.pointer = meg.pointerToNext;
			ofs.seekp(user.pointerToData);
			ofs.write((char *)&(user.userInfo), sizeof(user.userInfo));
		}
		else
		{
			int buf_pointer = user.userInfo.pointer;
			Message buf_m;
			for (int i = 0; i < number - 1; i++)
			{
				buf_pointer = meg.pointerToNext;
				ifs.seekg(meg.pointerToNext);
				ifs.read((char *)&meg, sizeof(meg));
			}
			ifs.seekg(meg.pointerToNext);
			ifs.read((char *)&buf_m, sizeof(buf_m));
			meg.pointerToNext = buf_m.pointerToNext;
			ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
			ofs.seekp(buf_pointer);
			ofs.write((char *)&meg, sizeof(meg));
			cout << "Successfully~\n";
		}
	}
}

void Interface::deleteMessage(int number)
{
	Message meg;
	ifstream ifs(MESSAGEFILE, ios::in | ios::binary);

	ifs.seekg(user.userInfo.pointer);
	ifs.read((char *)&meg, sizeof(meg));
	if (number == 0)
	{
		ofstream ofs(INFOFILE, ios::in | ios::out | ios::binary);
		user.userInfo.pointer = meg.pointerToNext;
		ofs.seekp(user.pointerToData);
		ofs.write((char *)&(user.userInfo), sizeof(user.userInfo));
	}
	else
	{
		int buf_pointer = user.userInfo.pointer;
		Message buf_m;
		for (int i = 0; i < number - 1; i++)
		{
			buf_pointer = meg.pointerToNext;
			ifs.seekg(meg.pointerToNext);
			ifs.read((char *)&meg, sizeof(meg));
		}
		ifs.seekg(meg.pointerToNext);
		ifs.read((char *)&buf_m, sizeof(buf_m));
		meg.pointerToNext = buf_m.pointerToNext;
		ofstream ofs(MESSAGEFILE, ios::in | ios::out | ios::binary);
		ofs.seekp(buf_pointer);
		ofs.write((char *)&meg, sizeof(meg));
		cout << "Successfully~\n";
	}
}

void Interface::changeName()
{
	char newName[LENGTHOFNAME] = "aa";
	RecordNode_name buf_name;
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);

	strcpy(buf_name.username, user.userInfo.name);
	buf_name.pointer = user.pointerToData;
	hash_name->deleteNode(buf_name);
	strcpy_s(user.userInfo.name, newName);
	strcpy(buf_name.username, newName);
	hash_name->insert(buf_name);

	ofs.seekp(this->user.pointerToData);
	ofs.write((char *)(&user.userInfo), sizeof(user.userInfo));
	cout << "Change successfully~\n";
}

void Interface::changeInfo()
{
	char cmd_change;
	ofstream ofs(INFOFILE, ios_base::in | ios_base::out | ios::binary);

	cout << "Your Information:\n"
		<< "Username: " << user.userInfo.identification
		<< "\nName: " << user.userInfo.name;
	if (user.userInfo.gender == 1)
		cout << "\nGender: Man";
	else cout << "\nGender: Woman";
	cout << "\nBirthday: " << user.userInfo.birthday;
	if (user.userInfo.teleNumber[0] != 0)//maybe wrong
		cout<< "\nTele: " << user.userInfo.teleNumber;
	else cout << "\nTele: N/A";
	if (user.userInfo.address[0] != 0)
		cout << "\nAddress: " << user.userInfo.address << endl;
	else cout << "\nAddress: N/A" << endl;

	cout << "You can change:\n"
		<< "1.password\n"
		<< "2.Name\n"
		<< "3.Birthday\n"
		<< "4.Telephone number\n"
		<< "5.address\n"
		<<"Please input the number\n";
	cin >> cmd_change;

	switch (cmd_change)
	{
	case '1':
		char newPwd[LENGTHOFPWD];
		cout << "Please input the new password\n";
		cin >> newPwd;
		cout << "Your new password is " << newPwd << endl;
		strcpy_s(user.userInfo.password, newPwd);
		ofs.seekp(this->user.pointerToData);
		ofs.write((char *)(&user.userInfo), sizeof(user.userInfo));
		break;
	case '2':
	{
		char newName[LENGTHOFNAME];
		RecordNode_name buf_name;
		cout << "Please input the name you want to change\n";
		cin >> newName;
		cout << "Your name is " << newName << endl;

		strcpy(buf_name.username, user.userInfo.name);
		buf_name.pointer = user.pointerToData;
		hash_name->deleteNode(buf_name);
		strcpy_s(user.userInfo.name, newName);
		strcpy(buf_name.username, newName);
		hash_name->insert(buf_name);

		ofs.seekp(this->user.pointerToData);
		ofs.write((char *)(&user.userInfo), sizeof(user.userInfo));
		break;
	}
	case '3':
		char newBir[LENGTHOFBIR];
		RecordNode_bir buf_bir;
		cout << "Please input the birthday you want to change\n";
		cin >> newBir;
		cout << "Your birthday is " << newBir << endl;

		buf_bir.pointer = user.pointerToData;
		strcpy(buf_bir.username, user.userInfo.birthday);
		hash_bir->deleteNode(buf_bir);
		strcpy(buf_bir.username, newBir);
		hash_bir->insert(buf_bir);

		strcpy_s(user.userInfo.birthday, newBir);
		ofs.seekp(this->user.pointerToData);
		ofs.write((char *)(&user.userInfo), sizeof(user.userInfo));
		break;
	case '4':
	{
		char newTele[LENGTHOFTELEPHONE];
		RecordNode_tele buf_tele;
		cout << "Please input your new tele number\n";
		cin >> newTele;
		cout << "Your new tele number is " << newTele << endl;

		buf_tele.pointer = user.pointerToData;
		strcpy(buf_tele.username, user.userInfo.teleNumber);
		hash_tele->deleteNode(buf_tele);//deleter the node
		strcpy(buf_tele.username, newTele);
		hash_tele->insert(buf_tele);

		strcpy_s(user.userInfo.teleNumber, newTele);
		ofs.seekp(this->user.pointerToData);
		ofs.write((char *)(&user.userInfo), sizeof(user.userInfo));
		break;
	}
	case '5':
	{
		char newAdd[LENGTHOFADDRESS];
		RecordNode_add buf_add;
		cout << "Please input your new address\n";
		cin >> newAdd;
		cout << "Your new address is " << newAdd << endl;

		buf_add.pointer = user.pointerToData;
		strcpy(buf_add.username, user.userInfo.teleNumber);
		hash_add->deleteNode(buf_add);//deleter the node
		strcpy(buf_add.username, newAdd);
		hash_add->insert(buf_add);

		strcpy_s(this->user.userInfo.address, newAdd);
		ofs.seekp(this->user.pointerToData);
		ofs.write((char *)&user.userInfo, sizeof(user.userInfo));
		break;
	}
	default:
		cout << "The command you input is wrong\n";
		break;
	}
}

void Interface::searchSomeone()
{
	char buf;
	cout << "You can search someone by:\n"
		<< "0. Username\n"
		<< "1. Name\n"
		<< "2. Birthday\n"
		<< "3. Tele\n"
		<< "4. Address\n"
		<< "5. Multiple\n"
		<< "6. Recommand\n"
		<< endl;
	cin >> buf;
	switch (buf)
	{
	case '0':
	{
				char buf_u[LENGTHOFID];
				RecordNode buf_r;
				Information info;
				cout << "Please input the username you want to search\n";
				cin.getline(buf_u, LENGTHOFID);
				cin.getline(buf_u, LENGTHOFID);
				for (int i = 0; i < strlen(buf_u); i++)//low perf
				{
					if (!((buf_u[i] >= 48 && buf_u[i] <= 57) || (buf_u[i] >= 65 && buf_u[i] <= 90) || (buf_u[i] >= 97 && buf_u[i] <= 122)))
					{
						cout << "Formal error\n";
						cin.clear();
						memset(buf_u, 0, sizeof(buf_u));
						return;
					}
				}
				cin.clear();
				buf_r = hash->contains(buf_u);
				if (buf_r.pointer == -2)
				{
					cout << "The username doesn't exist\n";
					break;
				}
				ifstream ifs(INFOFILE, ios::in | ios::binary);
				ifs.seekg(buf_r.pointer);
				ifs.read((char *)&info, sizeof(info));
				printInfo(info);
				break;
	}
	case '1':
	{
				char buf_u[LENGTHOFNAME];
				SaveForm_name buf_r;
				Information info;
				int num = 0;
				cout << "Please input the name you want to search\n";
				cin.getline(buf_u, LENGTHOFNAME);
				cin.getline(buf_u, LENGTHOFNAME);
				for (int i = 0; i < strlen(buf_u); i++)//low perf
				{
					if (!((buf_u[i] >= 65 && buf_u[i] <= 90) || (buf_u[i] >= 97 && buf_u[i] <= 122)))
					{
						cout << "Formal error\n";
						cin.clear();
						memset(buf_u, 0, sizeof(buf_u));
						return;
					}
				}
				cin.clear();
				buf_r.pointerToNext = -3;
				buf_r = hash_name->containsFrom(buf_u, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The name doesn't exist\n";
					break;
				}
				while (buf_r.pointerToNext != -2)//maybe wrong
				{
					if (buf_r.pointerToNext == -1)
					{
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						cout << num << endl;
						printInfo(info);
						cout << endl;
						num++;
						break;
					}
					ifstream ifs(INFOFILE, ios::in | ios::binary);
					ifs.seekg(buf_r.record.pointer);
					ifs.read((char *)&info, sizeof(info));
					cout << num << endl;
					printInfo(info);
					cout << endl;
					num++;
					buf_r = hash_name->containsFrom(buf_u, buf_r);
				}
				break;
	}
	case '2':
	{
				char buf_u[LENGTHOFBIR];
				SaveForm_bir buf_r;
				Information info;
				int num = 0;
				cout << "Please input the birthday you want to search\n(XXXX-XX-XX)";
				cin >> buf_u;
				buf_r.pointerToNext = -3;
				buf_r = hash_bir->containsFrom(buf_u, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The birthday doesn't exist\n";
					break;
				}
				while (buf_r.pointerToNext != -2)//maybe wrong
				{
					if (buf_r.pointerToNext == -1)
					{
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						cout << num << endl;
						printInfo(info);
						cout << endl;
						num++;
						break;
					}
					ifstream ifs(INFOFILE, ios::in | ios::binary);
					ifs.seekg(buf_r.record.pointer);
					ifs.read((char *)&info, sizeof(info));
					cout << num << endl;
					printInfo(info);
					cout << endl;
					num++;
					buf_r = hash_bir->containsFrom(buf_u, buf_r);
				}
				break;
	}
	case '3':
	{
				char buf_u[LENGTHOFBIR];
				SaveForm_tele buf_r;
				Information info;
				int num = 0;
				cout << "Please input the Tele\n";
				cin >> buf_u;
				buf_r.pointerToNext = -3;
				buf_r = hash_tele->containsFrom(buf_u, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The Tele doesn't exist\n";
					break;
				}
				while (buf_r.pointerToNext != -2)//maybe wrong
				{
					if (buf_r.pointerToNext == -1)
					{
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						cout << num << endl;
						printInfo(info);
						cout << endl;
						num++;
						break;
					}
					ifstream ifs(INFOFILE, ios::in | ios::binary);
					ifs.seekg(buf_r.record.pointer);
					ifs.read((char *)&info, sizeof(info));
					cout << num << endl;
					printInfo(info);
					cout << endl;
					num++;
					buf_r = hash_tele->containsFrom(buf_u, buf_r);
				}
				break;
	}
	case '4':
	{
		char buf_u[LENGTHOFBIR];
		SaveForm_add buf_r;
		Information info;
		int num = 0;
		cout << "Please input the address\n";
		cin >> buf_u;
		buf_r.pointerToNext = -3;
		buf_r = hash_add->containsFrom(buf_u, buf_r);
		if (buf_r.pointerToNext == -2)
		{
			cout << "The address doesn't exist\n";
			break;
		}
		while (buf_r.pointerToNext != -2)//maybe wrong
		{
			if (buf_r.pointerToNext == -1)
			{
				ifstream ifs(INFOFILE, ios::in | ios::binary);
				ifs.seekg(buf_r.record.pointer);
				ifs.read((char *)&info, sizeof(info));
				cout << num << endl;
				printInfo(info);
				cout << endl;
				num++;
				break;
			}
			ifstream ifs(INFOFILE, ios::in | ios::binary);
			ifs.seekg(buf_r.record.pointer);
			ifs.read((char *)&info, sizeof(info));
			cout << num << endl;
			printInfo(info);
			cout << endl;
			num++;
			buf_r = hash_add->containsFrom(buf_u, buf_r);
		}
		break;
	}
	case '5':
		multipleSearch();
		break;
	case '6':
		recommand();
		break;
	default:
		break;
	}
}

void Interface::multipleSearch()
{
	char buf_name[LENGTHOFNAME], buf_bir[LENGTHOFBIR], buf_tele[LENGTHOFTELEPHONE], buf_add[LENGTHOFADDRESS];
	vector<Information> answer, buf;
	string cmd;
	cout << "Please input the condition you want to search:\n"
		<< "1. Name\n"
		<< "2. Birthday\n"
		<< "3. Tele\n"
		<< "4. Address\n";
	cout << "For example, \"123\" means you want to search by name, birthday, and tele\n";
	cin >> cmd;
	for (int i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] == '1')
		{
			cout << "cout the name\n";
			cin >> buf_name;
			SaveForm_name buf_r;
			Information info;
			buf_r.pointerToNext = -3;
			buf_r = hash_name->containsFrom(buf_name, buf_r);
			if (buf_r.pointerToNext != -2)
			{
				while (buf_r.pointerToNext != -2)//maybe wrong
				{
					if (buf_r.pointerToNext == -1)
					{
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						buf.push_back(info);
						break;
					}
					ifstream ifs(INFOFILE, ios::in | ios::binary);
					ifs.seekg(buf_r.record.pointer);
					ifs.read((char *)&info, sizeof(info));
					buf.push_back(info);
					buf_r = hash_name->containsFrom(buf_name, buf_r);
				}
			}
			else
			{
				cout << "The user doesn't exist;\n";
				break;
			}
			if (answer.size() == 0)
			{
				for (int i = 0; i < buf.size(); i++)
				{
					answer.push_back(buf[i]);
				}
			}
			else
			{
				for (int i = 0; i < answer.size(); i++)
				{
					bool isIn = false;
					for (int j = 0; j < buf.size(); j++)
					{
						if (strcmp(answer[i].identification, buf[j].identification) == 0)
						{
							isIn = true;
							break;
						}
					}
					if (isIn == false)
					{
						answer.erase(answer.begin() + i);
					}
				}
			}
			for (int i = 0; i < answer.size(); i++)
			{
				cout << i << ".\n";
				printInfo(answer[i]);
			}
			buf.clear();
		}
		else if (cmd[i] == '2')
		{
			cout << "Please input the birthday\n";
			cin >> buf_bir;
			if (strlen(buf_bir) != 0)
			{
				SaveForm_bir buf_r;
				Information info;
				buf_r.pointerToNext = -3;
				buf_r = hash_bir->containsFrom(buf_bir, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The birthday doesn't exist\n";
				}
				else
				{
					while (buf_r.pointerToNext != -2)//maybe wrong
					{
						if (buf_r.pointerToNext == -1)
						{
							ifstream ifs(INFOFILE, ios::in | ios::binary);
							ifs.seekg(buf_r.record.pointer);
							ifs.read((char *)&info, sizeof(info));
							buf.push_back(info);
							break;
						}
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						buf.push_back(info);
						buf_r = hash_bir->containsFrom(buf_bir, buf_r);
					}
				}
			}
			if (answer.size() == 0)
			{
				for (int i = 0; i < buf.size(); i++)
				{
					answer.push_back(buf[i]);
				}
			}
			else
			{
				for (int i = 0; i < answer.size(); i++)
				{
					bool isIn = false;
					for (int j = 0; j < buf.size(); j++)
					{
						if (strcmp(answer[i].identification, buf[j].identification) == 0)
						{
							isIn = true;
							break;
						}
					}
					if (isIn == false)
					{
						answer.erase(answer.begin() + i);
					}
				}
			}
			for (int i = 0; i < answer.size(); i++)
			{
				cout << i << ".\n";
				printInfo(answer[i]);
			}
			buf.clear();
		}
		else if (cmd[i] == '3')
		{
			cout << "Please input the tele\n";
			cin >> buf_tele;
			if (strlen(buf_tele) != 0)
			{
				SaveForm_tele buf_r;
				Information info;
				buf_r.pointerToNext = -3;
				buf_r = hash_tele->containsFrom(buf_tele, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The tele doesn't exist\n";
				}
				else
				{
					while (buf_r.pointerToNext != -2)//maybe wrong
					{
						if (buf_r.pointerToNext == -1)
						{
							ifstream ifs(INFOFILE, ios::in | ios::binary);
							ifs.seekg(buf_r.record.pointer);
							ifs.read((char *)&info, sizeof(info));
							buf.push_back(info);
							break;
						}
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						buf.push_back(info);
						buf_r = hash_tele->containsFrom(buf_tele, buf_r);
					}
				}
			}
			if (answer.size() == 0)
			{
				for (int i = 0; i < buf.size(); i++)
				{
					answer.push_back(buf[i]);
				}
			}
			else
			{
				for (int i = 0; i < answer.size(); i++)
				{
					bool isIn = false;
					for (int j = 0; j < buf.size(); j++)
					{
						if (strcmp(answer[i].identification, buf[j].identification) == 0)
						{
							isIn = true;
							break;
						}
					}
					if (isIn == false)
					{
						answer.erase(answer.begin() + i);
					}
				}
			}
			for (int i = 0; i < answer.size(); i++)
			{
				cout << i << ".\n";
				printInfo(answer[i]);
			}
			buf.clear();
		}
		else if (cmd[i] == '4')
		{
			cout << "Please input the tele\n";
			cin >> buf_add;
			if (strlen(buf_add) != 0)
			{
				SaveForm_add buf_r;
				Information info;
				buf_r.pointerToNext = -3;
				buf_r = hash_add->containsFrom(buf_add, buf_r);
				if (buf_r.pointerToNext == -2)
				{
					cout << "The tele doesn't exist\n";
				}
				else
				{
					while (buf_r.pointerToNext != -2)//maybe wrong
					{
						if (buf_r.pointerToNext == -1)
						{
							ifstream ifs(INFOFILE, ios::in | ios::binary);
							ifs.seekg(buf_r.record.pointer);
							ifs.read((char *)&info, sizeof(info));
							buf.push_back(info);
							break;
						}
						ifstream ifs(INFOFILE, ios::in | ios::binary);
						ifs.seekg(buf_r.record.pointer);
						ifs.read((char *)&info, sizeof(info));
						buf.push_back(info);
						buf_r = hash_add->containsFrom(buf_add, buf_r);
					}
				}
			}
			if (answer.size() == 0)
			{
				for (int i = 0; i < buf.size(); i++)
				{
					answer.push_back(buf[i]);
				}
			}
			else
			{
				for (int i = 0; i < answer.size(); i++)
				{
					bool isIn = false;
					for (int j = 0; j < buf.size(); j++)
					{
						if (strcmp(answer[i].identification, buf[j].identification) == 0)
						{
							isIn = true;
							break;
						}
					}
					if (isIn == false)
					{
						answer.erase(answer.begin() + i);
					}
				}
			}
			for (int i = 0; i < answer.size(); i++)
			{
				cout << i << ".\n";
				printInfo(answer[i]);
			}
			buf.clear();
		}
	}
	cout << "Over~\nWhat else do you want to do~?\n";
}

void Interface::recommand()//will output someone twice, and will output myself
{
	SaveForm_name buf_r;
	Information info;

	buf_r.pointerToNext = -3;
	buf_r = hash_name->containsFrom(user.userInfo.name, buf_r);
	if (buf_r.pointerToNext == -2)
	{
		cout << "The name doesn't exist\n";
	}
	else
	{
		cout << "Same Name: \n";
		while (buf_r.pointerToNext != -2)//maybe wrong
		{
			if (buf_r.pointerToNext == -1)
			{
				ifstream ifs(INFOFILE, ios::in | ios::binary);
				ifs.seekg(buf_r.record.pointer);
				ifs.read((char *)&info, sizeof(info));
				if (strcmp(user.userInfo.identification, info.identification) != 0)
					printInfo(info);
				break;
			}
			ifstream ifs(INFOFILE, ios::in | ios::binary);
			ifs.seekg(buf_r.record.pointer);
			ifs.read((char *)&info, sizeof(info));
			if (strcmp(user.userInfo.identification, info.identification) != 0)
				printInfo(info);
			buf_r = hash_name->containsFrom(user.userInfo.name, buf_r);
		}
	}

	SaveForm_bir buf_b;

	buf_b.pointerToNext = -3;
	buf_b = hash_bir->containsFrom(user.userInfo.birthday, buf_b);
	if (buf_b.pointerToNext == -2)
	{
		cout << "The birthday doesn't exist\n";
	}
	else
	{
		cout << "Same birthday: \n";
		while (buf_b.pointerToNext != -2)//maybe wrong
		{
			if (buf_b.pointerToNext == -1)
			{
				ifstream ifs(INFOFILE, ios::in | ios::binary);
				ifs.seekg(buf_b.record.pointer);
				ifs.read((char *)&info, sizeof(info));
				if (strcmp(user.userInfo.identification, info.identification) != 0)
					printInfo(info);
				break;
			}
			ifstream ifs(INFOFILE, ios::in | ios::binary);
			ifs.seekg(buf_b.record.pointer);
			ifs.read((char *)&info, sizeof(info));
			if (strcmp(user.userInfo.identification, info.identification) != 0)
				printInfo(info);
			buf_b = hash_bir->containsFrom(user.userInfo.birthday, buf_b);
		}
	}
}

void Interface::people()
{
	int number = 0;
	char cmd;
	OLGArc buf_arc;
	OLGVNode buf_node;
	Trie answer = Trie();
	Trie answer_ = Trie();
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	ifs.seekg(user.userInfo.pointerToLike);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	cout << "You can choose:\n"
		<< "0. by time order\n"
		<< "1. by alphabet order\n";
	cin >> cmd;
	switch (cmd)
	{
	case '0':
		cout << "\nPeople you like: \n";
		ifs.seekg(user.userInfo.pointerToLike);
		ifs.read((char *)&buf_node, sizeof(buf_node));
		if (buf_node.firstout != -1)
		{
			ifs.seekg(buf_node.firstout);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			while (buf_arc.tlink != -1)
			{
				cout << number << ".\t";
				ifs.seekg(buf_arc.headvex);
				ifs.read((char *)&buf_node, sizeof(buf_node));
				cout << buf_node.username << endl;
				number++;
				ifs.seekg(buf_arc.tlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
			}
			cout << number << ".\t";
			ifs.seekg(buf_arc.headvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));
			cout << buf_node.username << endl;
			number++;
		}
		number = 0;
		cout << "Peolple like you:\n";
		ifs.seekg(user.userInfo.pointerToLike);
		ifs.read((char *)&buf_node, sizeof(buf_node));
		if (buf_node.firstin != -1)
		{
			ifs.seekg(buf_node.firstin);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			while (buf_arc.hlink != -1)
			{
				cout << number << ".\t";
				ifs.seekg(buf_arc.tailvex);
				ifs.read((char *)&buf_node, sizeof(buf_node));
				cout << buf_node.username << endl;
				number++;
				ifs.seekg(buf_arc.hlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
			}
			cout << number << ".\t";
			ifs.seekg(buf_arc.tailvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));
			cout << buf_node.username << endl;
			number++;
		}
		break;
	case '1':
		cout << "\nPeople you like: \n";
		ifs.seekg(user.userInfo.pointerToLike);
		ifs.read((char *)&buf_node, sizeof(buf_node));
		if (buf_node.firstout != -1)
		{
			ifs.seekg(buf_node.firstout);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			while (buf_arc.tlink != -1)
			{
				ifs.seekg(buf_arc.headvex);
				ifs.read((char *)&buf_node, sizeof(buf_node));
				
				char usrname[LENGTHOFID];
				strcpy(usrname, buf_node.username);
				answer.insert(buf_node.username);

				ifs.seekg(buf_arc.tlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
			}
			ifs.seekg(buf_arc.headvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));

			char usrname[LENGTHOFID];
			strcpy(usrname, buf_node.username);
			answer.insert(usrname);
		}
		
		answer.print();

		cout << "Peolple like you:\n";
		ifs.seekg(user.userInfo.pointerToLike);
		ifs.read((char *)&buf_node, sizeof(buf_node));
		if (buf_node.firstin != -1)
		{
			ifs.seekg(buf_node.firstin);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			while (buf_arc.hlink != -1)
			{
				ifs.seekg(buf_arc.tailvex);
				ifs.read((char *)&buf_node, sizeof(buf_node));

				char usrname[LENGTHOFID];
				strcpy(usrname, buf_node.username);
				answer_.insert(usrname);

				ifs.seekg(buf_arc.hlink);
				ifs.read((char *)&buf_arc, sizeof(buf_arc));
			}
			ifs.seekg(buf_arc.tailvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));

			char usrname[LENGTHOFID];
			strcpy(usrname, buf_node.username);
			answer_.insert(usrname);
		}
		answer_.print();
		break;
	default:
		break;
	}
}

bool Interface::likeSomeone(char *buf)
{
	RecordNode r_buf;
	Information i_buf;

	if (strcmp(buf, user.userInfo.identification) == 0)
	{
		cout << "You can't like yourself\n";
		return false;
	}

	r_buf = hash->contains(buf);
	if (r_buf.pointer == -2)
	{
		cout << "The username do not exist\n";
		return false;
	}
	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_buf.pointer);
	ifs.read((char *)&i_buf, sizeof(i_buf));
	if (i_buf.isAvailable == false)
	{
		cout << "Sorry, The user has been closed\n";
		return false;
	}

	if (relationship->contains(i_buf.pointerToLike) == true)//like or not
	{
		cout << "You have liked the user\n";
		return false;
	}
	relationship->InsertArc(i_buf.pointerToLike);
	cout << "Successfully~\n";
	return true;
}

void Interface::likeSomeone()
{
	char buf[LENGTHOFID];
	RecordNode r_buf;
	Information i_buf;
	cout << "Please input the username\n";
	cin >> buf;
	if (strcmp(buf, user.userInfo.identification) == 0)
	{
		cout << "You can't like yourself\n";
		return;
	}
	r_buf = hash->contains(buf);
	if (r_buf.pointer == -2)
	{
		cout << "The username do not exist\n";
		return;
	}

	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(r_buf.pointer);
	ifs.read((char *)&i_buf, sizeof(i_buf));
	if (i_buf.isAvailable == false)
	{
		cout << "Sorry, The user has been closed\n";
		return;
	}

	if (relationship->contains(i_buf.pointerToLike) == true)//like or not
	{
		cout << "You have liked the user\n";
		return;
	}
	relationship->InsertArc(i_buf.pointerToLike);
	cout << "Successfully~\n";
}

void Interface::cancelLike()
{
	int number = 0;
	int inNumber = 0;
	OLGArc buf_arc;
	OLGVNode buf_node;
	ifstream ifs(LIKEFILE, ios::in | ios::binary);
	cout << "\nPeople you like: \n";
	ifs.seekg(user.userInfo.pointerToLike);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	if (buf_node.firstout != -1)
	{
		ifs.seekg(buf_node.firstout);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
		while (buf_arc.tlink != -1)
		{
			cout << number << ".\t";
			ifs.seekg(buf_arc.headvex);
			ifs.read((char *)&buf_node, sizeof(buf_node));
			cout << buf_node.username << endl;
			number++;
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
		}
		cout << number << ".\t";
		ifs.seekg(buf_arc.headvex);
		ifs.read((char *)&buf_node, sizeof(buf_node));
		cout << buf_node.username << endl;
		number++;
	}
	if (number == 0)
	{
		cout << "You have liked nobody\n";
		return;
	}
	cout << "Please input the number\n";
	cin >> inNumber;
	if (inNumber >= number)
	{
		cout << "Number error\n";
		return;
	}
	ifs.seekg(user.userInfo.pointerToLike);
	ifs.read((char *)&buf_node, sizeof(buf_node));
	if (buf_node.firstout != -1)
	{
		ifs.seekg(buf_node.firstout);
		ifs.read((char *)&buf_arc, sizeof(buf_arc));
		while (inNumber != 0)
		{
			ifs.seekg(buf_arc.tlink);
			ifs.read((char *)&buf_arc, sizeof(buf_arc));
			inNumber--;
		}
	}
	relationship->DeleteArc(buf_arc.headvex);
	cout << "Successfully~\nWhat else do you want to do\n";
}

void Interface::cancelLike(char *x)
{
	RecordNode buf = hash->contains(x);
	Information info;
	ifstream ifs(INFOFILE, ios::in | ios::binary);
	ifs.seekg(buf.pointer);
	ifs.read((char *)&info, sizeof(info));
	ifs.close();
	if (strcmp(x, user.userInfo.identification) == 0)
	{
		cout << "Hehe, You can't cancel it\n";
		return;
	}
	relationship->DeleteArc(info.pointerToLike);
	cout << "Cancel successfully~\n";
}

void Interface::work()
{
	while (1)
	{
		if (logFlag == true)
		{
			onlineWork();
		}
		cout << "Then what you want to do~?\n";
		if (quitFlag == true)
			break;
		getInput_offline();
	}
}

void Interface::clear()
{
	
}

void Interface::beginTowork()//make the interface work
{
	load();
	display_offline();
	usage_offline();
	getInput_offline();
	work();
	quitDo_offline();
}

void Interface::printInfo(Information &x)
{
	cout << "Username: " << x.identification << endl
		<< "Name: " << x.name << endl
		<< "Birthday: " << x.birthday << endl;
}