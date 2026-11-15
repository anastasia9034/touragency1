#define _HAS_STD_BYTE 0
#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include "Tour.h"
#include "User.h"
#include "Booking.h"
#include "Exceptions.h"
using namespace std;

class Database {
private:
	vector<unique_ptr<Tour>> tours;
	vector<unique_ptr<User>> users;
	vector<unique_ptr<Booking>> bookings;
public:
	Database();

	void addTours(const string& title, const string& country, double price, int days, int stars = 3);
	void sortToursByPrice(bool ascending = true);
	void sortToursByDuration(bool ascending = true); 
	void removeTour(int id);
	const vector<unique_ptr<Tour>>& getTours() const;
	vector<const Tour*> searchToursByCountry(const string& countryPart) const;
	vector<const Tour*> filterTours(int minStars, double maxPrice, int minDays) const;

	void addUser(unique_ptr<User> user);
	User* findUser(const string& username);

	void bookTour(const string& username, int tourId);
	void cancelBooking(int bookingId, const string& username);
	vector<const Booking*> getBookingsFor(const string& username) const;
	const vector<unique_ptr<Booking>>& getAllBookings() const;

	void saveData(const string& filename) const;
	void loadData(const string& filename);
};