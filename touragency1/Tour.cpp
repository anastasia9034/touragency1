#include "Tour.h"
using namespace std;

int Tour::counter = 0;

Tour::Tour(const string& t, const string& c, double p, int days) : title(t), country(c), price(p), durationDays(days), stars(3) { id = ++counter; }

int Tour::getId() const { return id; }
string Tour::getTitle() const { return title; }
string Tour::getCountry() const { return country; }
double Tour::getPrice() const { return price; }
int Tour::getDuration() const { return durationDays; }
int Tour::getStars() const { return stars; }

void Tour::setPrice(double p) { price = p; }
void Tour::setStars(int s) {
	if (s >= 1 && s <= 5) stars = s;
}
int Tour::getCounter() { return counter; }
void Tour::setCounter(int c) { counter = c; }

ostream& operator<<(ostream& os, const Tour& tour) {
	os << tour.id << "|" << tour.title << "|" << tour.country << "|$" << tour.price << "|" << tour.durationDays << "дней";
	return os;
}

istream& operator>>(istream& is, Tour& tour) {
	is >> tour.title >> tour.country >> tour.price >> tour.durationDays;
	return is;
}