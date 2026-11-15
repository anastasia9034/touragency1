#pragma once
#include <string>
#include <iostream>
using namespace std;

class User {
protected:
	string username;
	string passwordHash;
	string role;

	string hashPassword(const string& pwd) const;
public:
	User(const string& uname, const string& pwd, const string& r);
	virtual ~User() = default;

	virtual void showMenu() const = 0; //чисто виртуальная функция
	string getUsername() const;
	string getRole() const;
	bool checkPassword(const string& pwd) const;

	friend ostream& operator<<(ostream& os, const User& user);
	friend istream& operator>>(istream& is, User& user);
};

class Employee : public User {
protected:
	double salary;
public:
	Employee(const string& uname, const string& pwd, const string& r, double sal);
	void showMenu() const override;
};

class Admin : public Employee {
protected:
	double salary;
public:
	Admin(const string& uname, const string& pwd, double sal);
	void showMenu() const override;
};

class Manager : public Employee {
protected:
	double salary;
public:
	Manager(const string& uname, const string& pwd, double sal);
	void showMenu() const override;
};

class Client : public User {
protected:
	string phoneNumber;
public:
	Client(const string& uname, const string& pwd, const string& phone);
	void showMenu() const override;
};