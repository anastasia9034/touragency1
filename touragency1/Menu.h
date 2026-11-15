#pragma once
#include "Database.h"
using namespace std;

class Menu {
private:
	Database db;
	User* currentUser;

	void loginMenu();
	void adminMenu();
	void managerMenu();
	void clientMenu();
public:
	Menu();
	void run();
};