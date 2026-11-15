#include "User.h"
#include <functional>
using namespace std;

User::User(const string& uname, const string& pwd, const string& r) :username(uname), passwordHash(hashPassword(pwd)), role(r) {}

string User::hashPassword(const string& pwd) const {
	return to_string(hash<string>{}(pwd));
}

string User::getUsername() const {
	return username;
}
string User::getRole() const {
	return role;
}

bool User::checkPassword(const string& pwd) const {
	return passwordHash == hashPassword(pwd);
}

ostream& operator<<(ostream& os, const User& user) {
	os << user.username << "" << user.role;
	return os;
}

istream& operator>>(istream& is, User& user) {
	is >> user.username >> user.role;
	return is;
}

Employee::Employee(const string& uname, const string& pwd, const string& r, double sal) : User(uname, pwd, r), salary(sal) {}
void Employee::showMenu() const {
	cout << "--- Меню сотрудника ---\n";
}

Admin::Admin(const string& uname, const string& pwd, double sal) : Employee(uname, pwd, "Admin", sal) {}
void Admin::showMenu() const {
	cout << "1. Управление пользователями\n2. Просмотр отчетов\n";
}

Manager::Manager(const string& uname, const string& pwd, double sal) : Employee(uname, pwd, "Manager", sal) {}
void Manager::showMenu() const {
	cout << "1. Управление турами\n2. Оформление заказов\n";
}

Client::Client(const string& uname, const string& pwd, const string& phone) : User(uname, pwd, "Client"), phoneNumber(phone) {}
void Client::showMenu() const {
	cout << "1. Просмотр доступных туров\n2. Забронировать тур\n3. Мои заказы\n";
}