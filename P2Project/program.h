#ifndef PROGRAM_H
#define PROGRAM_H
#include "interface.h"
class Program
{
public:
	Program();
	~Program();
	void work();
private:
	Interface *ui;

	void testMode();
	void testRegister();
	void testHashSearch();
	void testHashChange();
	void testLike_a();
	void testMessage();
	void testMessage_a();
	void testLike();
	char *randomString(int maxLength);
	Information createUserMachine();//create the user
};

#endif