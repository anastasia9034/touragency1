#include "Booking.h"
using namespace std;

int Booking::counter = 0;

Booking::Booking(const string& client, int tid) : clientUsername(client), tourId(tid), status("Активно") { id = ++counter; }

int  Booking::getCounter() { return counter; }
void Booking::setCounter(int c) { counter = c; }

Booking::Booking(int id_, const string& client, int tid, const string& st)
	: id(id_), clientUsername(client), tourId(tid), status(st)
{
	if (id > counter) counter = id;   
}

int Booking::getId() const { return id; }
string Booking::getClient() const { return clientUsername; }
int Booking::getTourId() const { return tourId; }
string Booking::getStatus() const { return status; }

void Booking::cancel() {
	status = "Отменено";
}

ostream& operator<<(ostream& os, const Booking& b) {
	os << "Бронь #" << b.id << " | тур ID: " << b.tourId << " | клиент: " << b.clientUsername << " | статус: " << b.status;
	return os;
}

istream& operator>>(istream& is, Booking& b) {
	is >> b.clientUsername >> b.tourId >> b.status;
	return is;
}