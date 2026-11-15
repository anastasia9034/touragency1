#pragma once
#include <string>
#include <iostream>
using namespace std;

class Booking {
private:
	int id;
	string clientUsername;
	int tourId;
	string status;
	static int counter;
public:
	Booking(const string& client, int tourId);

	Booking(int id, const string& client, int tourId, const string& status);

	static int  getCounter();
	static void setCounter(int c);

	int getId() const;
	string getClient() const;
	int getTourId() const;
	string getStatus() const;

	void cancel();

	friend ostream& operator<<(ostream& os, const Booking& b);
	friend istream& operator>>(istream& is, Booking& b);
};