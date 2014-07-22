#include "program.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
Program::Program()
{

}

Program::~Program()
{
	free(ui);
}

void Program::work()
{
	char cmd;
	bool flag;
	cout << "Please choose the mode: \n"
		<< "0. Test mode\n"
		<< "1. UI mode\n"
		<< endl;
	cin >> cmd;
	switch (cmd)
	{
	case '0':
		testMode();
		break;
	case '1':
		cout << "0. save the message at the end\n"
			<< "1. save the message when it is been pushed\n";
		cin >> cmd;
		if (cmd == '0')
			flag = true;
		else if (cmd == '1')
			flag = false;
		ui = new Interface(flag);
		ui->beginTowork();
		break;
	default:
		break;
	}
}

void Program::testMode()
{
	bool flag = true;
	ui = new Interface(flag);
	cout << "0. User register test(performance)\n"
		<< "1. Hash Insert test(accuracy)\n"
		<< "2. Hash change test(accuracy)\n"
		<< "3. Message test(performance)\n"
		<< "4. Message test(accuracy)\n"
		<< "5. Like test(performance)\n"
		<< "6. Like test(accuracy)\n"
		<< endl;
	char cmd;
	cin >> cmd;
	switch (cmd)
	{
	case '0':
		testRegister();
		break;
	case '1':
		testHashSearch();
		break;
	case '2':
		testHashChange();
		break;
	case '3':
		testMessage();
		break;
	case '4':
		testMessage_a();
		break;
	case '5':
		testLike();
		break;
	case '6':
		testLike_a();
	default:
		break;
	}
}

//main memory leap//solved
void Program::testLike()
{
	vector<char *> buf;
	ofstream ofs("Statistic.txt", ios::out);
	clock_t start, finish;
	double myTime;
	cout << "input the number of users you want to test\n";
	int x, number = 0;
	cin >> x;
	ui->load();
	srand((unsigned)time(0));
	for (int i = 0; i < x; i++)
	{
		Information info = createUserMachine();
		ui->register_t(info);
		char *free1 = new char[LENGTHOFID];
		strcpy(free1, info.identification);
		buf.push_back(free1);
	}
	ui->login();
	start = clock();
	for (int i = 0; i < buf.size(); i++)
	{
		if (ui->likeSomeone(buf[i]) == true)
			number++;
		if (i % 1000 == 0)
		{
			finish = clock();
			myTime = (double)(finish - start) / CLOCKS_PER_SEC;
			myTime = myTime / 1000.0;
			ofs << i << "\t" << myTime << endl;
		}
	}
	cout << "number:" << number << endl;
	ui->quitDo_offline();
}

//some problems
void Program::testLike_a()
{
	vector<char *> buf;
	cout << "input the number of users you want to test\n";
	int x;
	cin >> x;
	ui->load();
	srand((unsigned)time(0));
	for (int i = 0; i < x; i++)
	{
		Information info = createUserMachine();
		ui->register_t(info);//don't sure is successful
		char *free1 = new char[LENGTHOFID];
		strcpy(free1, info.identification);
		buf.push_back(free1);
	}
	ui->login();
	for (int i = 0; i < buf.size(); i++)
	{
		if (ui->likeSomeone(buf[i]))//to avoid 1
			ui->cancelLike(buf[i]);
	}
	ui->quitDo_offline();
}

//test the insert of hash
void Program::testHashSearch()
{
	ofstream ofs("Statistic.txt", ios::out);
	ofstream ofs_user("./data/name.txt", ios::out);
	cout << "input the number of users you want to test\n";
	int x;
	cin >> x;
	ui->load();
	srand((unsigned)time(0));
	for (int i = 0; i < x; i++)
	{
		Information info = createUserMachine();
		ui->register_t(info);
		ofs_user << info.identification << endl;
		if (i % 100 == 0)
		{
			ofs << i << "\t" << info.identification << "\t" << ui->login(info) << endl;
		}
	}
	ui->quitDo_offline();
}

void Program::testHashChange()
{
	cout << "input the number of users you want to test\n";
	int x;
	cin >> x;
	ui->load();
	srand((unsigned)time(0));
	for (int i = 0; i < x; i++)
	{
		Information info = createUserMachine();
		ui->register_t(info);
		if (i % 100 == 0)
		{
			cout << i << "\t" << info.identification
				<< "\t" << ui->login(info)
				<< endl;
			ui->changeName();
		}
	}
	ui->quitDo_offline();
}

void Program::testRegister()
{
	ofstream ofs("Statistic.txt", ios::out);
	clock_t start, finish;
	double myTime;
	cout << "input the number of users you want to test\n";
	int x;
	cin >> x;
	start = clock();
	ui->load();
	srand((unsigned)time(0));
	for (int i = 0; i < x; i++)
	{
		ui->register_t(createUserMachine());
		if (i%100 == 0)
		{
			finish = clock();
			myTime = (double)(finish - start) / CLOCKS_PER_SEC;
			myTime = myTime / 1000.0;
			ofs << i << "\t" << myTime << endl;
		}
	}
	ui->quitDo_offline();
	finish = clock();
	myTime = (double)(finish - start) / CLOCKS_PER_SEC;
	myTime = myTime / 1000.0;
	cout << x << ". " << myTime << endl;
}

void Program::testMessage()
{
	cout << "Input the number\n";
	int number;
	ofstream ofs("Statistic.txt", ios::out);
	clock_t start, finish;
	double myTime;
	cin >> number;
	srand(time(NULL));
	ui->load();
	ui->login();
	start = clock();
	for (int i = 0; i < number; i++)
	{
		Message meg;
		char *free1 = randomString(LENGTHOFMESSAGE);
		strcpy(meg.message, free1);
		free(free1);
		ui->pushMessage(meg);
		if (i % 1000 == 0)
		{
			finish = clock();
			myTime = (double)(finish - start) / CLOCKS_PER_SEC;
			myTime = myTime / 1000.0;
			ofs << i << "\t" << myTime << endl;
		}
	}
	ui->quitDo_offline();
}

void Program::testMessage_a()
{
	cout << "Input the number\n";
	int number;
	cin >> number;
	srand(time(NULL));
	ui->load();
	ui->login();
	for (int i = 0; i < number; i++)
	{
		Message meg;
		char *free1 = randomString(LENGTHOFMESSAGE);
		strcpy(meg.message, free1);
		free(free1);
		ui->pushMessage(meg);
	}
	for (int i = 0; i < number; i += 100)
	{
		ui->deleteMessage(i);
	}
	ui->quitDo_offline();
}

char *Program::randomString(int maxLength)
{
	char *result = new char[maxLength];
	char c;
	int length = rand() % (maxLength - 1) + 1;//determine the length of ID
	for (int i = 0; i < length; i++)
	{
		c = rand() % (122 - 65) + 65;
		if (c > 90 && c < 97)
			c += 7;
		result[i] = c;
	}
	result[length] = '\0';
	return result;
}

Information Program::createUserMachine()
{
	Information result;
	char *free1 = randomString(LENGTHOFID);
	char *x = "a";
	char *y = "1911-11-11";
	strcpy(result.identification, free1);
	strcpy(result.password, x);
	strcpy(result.name, x);
	strcpy(result.birthday, y);
	result.gender = 1;
	free(free1);
	return result;
}