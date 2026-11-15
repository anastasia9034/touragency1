#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Tour {
private:
	int id;
	string title;
	string country;
	double price;
	int durationDays;
	int stars;
	static int counter; //Статическое поле
public:
	Tour(const string& t, const string& c, double pr, int days);

	static void setCounter(int c);

	int getId() const;
	string getTitle() const;
	string getCountry() const;
	double getPrice() const;
	int getDuration() const;
	int getStars() const;

	void setPrice(double p);
	void setStars(int s);

	static int getCounter();

	friend ostream& operator<<(ostream& os, const Tour& tour);
	friend istream& operator>>(istream& is, Tour& tour);

	//Шаблонный метод для поиска/фильтрации
	template<typename T>
	bool matches(const string& field, T value) const {
		if constexpr (is_same_v<T, string> || is_convertible_v<T, const char*>) {
			if (field == "country") {
				return country == value;
			}
		}
		else {
			if (field == "max_price") {
				return price <= static_cast<double>(value);
			}
		}
		return false;
	}
};