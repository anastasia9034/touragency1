#include "Database.h"
#include <algorithm>   
#include <cctype>      
#include <vector>      
#include <iostream>    
#include <string>
using namespace std;

Database::Database() {
	users.push_back(make_unique<Admin>("admin", "admin123", 1500.0));
	users.push_back(make_unique<Manager>("manager", "manager123", 1200.0));
}

void Database::addTours(const string& title, const string& country, double price, int days, int stars) {
	if (price < 0 || days <= 0) {
		throw ValidationException("Неверная цена или длительность тура.");
	}
    if (stars < 1 || stars > 5) {
        throw ValidationException("Количество звезд должно быть от 1 до 5.");
    }
	tours.push_back(make_unique<Tour>(title, country, price, days));
    tours.back()->setStars(stars);
}

void Database::removeTour(int id) {
	auto it = remove_if(tours.begin(), tours.end(), [id](const unique_ptr<Tour>& t) {
		return t->getId() == id;
		});
	if (it == tours.end()) {
		throw ValidationException("Тур с указанным ID не найден.");
	}
	tours.erase(it, tours.end());
}

const vector<unique_ptr<Tour>>& Database::getTours() const { return tours; }

void Database::addUser(unique_ptr<User> user) {
	users.push_back(move(user));
}

User* Database::findUser(const string& username) {
	for (auto& u : users) {
		if (u->getUsername() == username) {
			return u.get();
		}
	}
	return nullptr;
}

void Database::bookTour(const string& username, int tourId) {
	auto it = find_if(tours.begin(), tours.end(), [tourId](const unique_ptr<Tour>& t) {return t->getId() == tourId; });
	if (it == tours.end()) {
		throw ValidationException("Тур с таким ID гне найден.");
	}
	for (const auto& b : bookings) {
		if (b->getClient() == username &&
			b->getTourId() == tourId &&
			b->getStatus() == "Активно") {
			throw ValidationException("Вы уже забронировали этот тур.");
		}
	}
	bookings.push_back(make_unique<Booking>(username, tourId));
}

void Database::cancelBooking(int bookingId, const string& username) {
	for (auto& b : bookings) {
		if (b->getClient() == username &&
			b->getId() == bookingId &&
			b->getStatus() == "Активно") {
			b->cancel();
			return;
		}
	}
	throw ValidationException("Бронь не найдена или уже отменена.");
}

vector<const Booking*> Database::getBookingsFor(const string& username) const {
	vector<const Booking*> result;
	for (const auto& b : bookings) {
		if (b->getClient() == username) {
			result.push_back(b.get());
		}
	}
	return result;
}

const vector<unique_ptr<Booking>>& Database::getAllBookings() const { return bookings; }

void Database::saveData(const string& filename) const {
	ofstream file(filename, ios::trunc);
	if (!file.is_open()) {
		throw FileException("Не удалось открыть файл для записи: " + filename);
	}

	// Туры
	file << tours.size() << '\n';
	for (const auto& t : tours) {
		file << t->getId() << '|'
			<< t->getTitle() << '|'
			<< t->getCountry() << '|'
			<< t->getPrice() << '|'
			<< t->getDuration() << '|' 
            << t->getStars() <<'\n';
	}

	// Брони 
	file << bookings.size() << '\n';
	for (const auto& b : bookings) {
		file << b->getId() << '|'
			<< b->getClient() << '|'
			<< b->getTourId() << '|'
			<< b->getStatus() << '\n';
	}

	if (!file) {
		throw FileException("Ошибка записи в файл: " + filename);
	}
}

void Database::loadData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    auto split = [](const string& line, char sep) {
        vector<string> parts;
        string cur;
        for (char ch : line) {
            if (ch == sep) { parts.push_back(cur); cur.clear(); }
            else            cur += ch;
        }
        parts.push_back(cur);
        return parts;
        };

    auto trim = [](string s) {
        auto notSpace = [](unsigned char c) { return !isspace(c); };
        s.erase(s.begin(), find_if(s.begin(), s.end(), notSpace));
        s.erase(find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
        return s;
        };

    auto toDouble = [&trim](const string& s, const string& ctx) {
        try { return stod(trim(s)); }
        catch (...) { throw FileException("Некорректное число ('" + s + "') в " + ctx); }
        };
    auto toInt = [&trim](const string& s, const string& ctx) {
        try { return stoi(trim(s)); }
        catch (...) { throw FileException("Некорректное целое ('" + s + "') в " + ctx); }
        };

    // очистка 
    tours.clear();
    bookings.clear();
    Tour::setCounter(0);
    Booking::setCounter(0);

    string line;
    int maxTourId = 0;
    int maxBookingId = 0;
   
    if (!getline(file, line)) return;           
    line = trim(line);
    if (line.empty()) return;

    int tourCount = toInt(line, "заголовке туров");
    if (tourCount < 0) throw FileException("Отрицательное число туров.");

    for (int i = 0; i < tourCount; ++i) {
        if (!getline(file, line)) {
            throw FileException("Неожиданный конец файла при чтении туров.");
        }
        auto p = split(line, '|');

        string title, country;
        double price = 0.0;
        int    days = 0;
        int stars = 3;
        int    id = 0;

        if (p.size() == 6) {
            id = toInt(p[0], "ID тура (строка " + to_string(i + 1) + ")");
            title = trim(p[1]);
            country = trim(p[2]);
            price = toDouble(p[3], "цене тура");
            days = toInt(p[4], "длительности тура");
            stars = toInt(p[5], "звездам отеля");
        }
        else if (p.size() == 5) {
            id = toInt(p[0], "ID тура (строка " + to_string(i + 1) + ")");
            title = trim(p[1]);
            country = trim(p[2]);
            price = toDouble(p[3], "цене тура");
            days = toInt(p[4], "длительности тура");
            stars = 3;
        }
        else {
            throw FileException("Повреждённая запись тура (строка " +
                to_string(i + 1) + "): '" + line + "'");
        }

        if (price < 0 || days <= 0) {
            throw ValidationException("Некорректные данные тура в файле: " + title);
        }

        auto t = make_unique<Tour>(title, country, price, days);
        if (id > 0) {
            maxTourId = max(maxTourId, id);
        }
        else {
            maxTourId = max(maxTourId, t->getId());
        }
        tours.push_back(move(t));
    }
    Tour::setCounter(maxTourId);

    if (!getline(file, line)) return;           
    line = trim(line);
    if (line.empty()) return;

    int bookCount = toInt(line, "заголовке броней");
    if (bookCount < 0) throw FileException("Отрицательное число броней.");

    for (int i = 0; i < bookCount; ++i) {
        if (!getline(file, line)) {
            throw FileException("Неожиданный конец файла при чтении броней.");
        }
        auto p = split(line, '|');

        string client, status;
        int    tourId = 0;
        int    id = 0;

        if (p.size() == 4) {
            id = toInt(p[0], "ID брони (строка " + to_string(i + 1) + ")");
            client = trim(p[1]);
            tourId = toInt(p[2], "ID тура в брони");
            status = trim(p[3]);
        }
        else if (p.size() == 3) {
            client = trim(p[0]);
            tourId = toInt(p[1], "ID тура в брони");
            status = trim(p[2]);
            id = 0;
        }
        else {
            throw FileException("Повреждённая запись брони (строка " +
                to_string(i + 1) + "): '" + line + "'");
        }

        if (status == "cancelled" || status == "Cancelled" || status == "Отменено") {
            status = "Отменено";
        }
        else {
            status = "Активно";
        }

        bool tourExists = any_of(tours.begin(), tours.end(),
            [tourId](const unique_ptr<Tour>& t) { return t->getId() == tourId; });
        if (!tourExists) {
            cerr << "[loadData] Пропущена бронь #" << id
                << ": тур с ID " << tourId << " не найден.\n";
            continue;
        }

        unique_ptr<Booking> b;
        if (id > 0) {
            b = make_unique<Booking>(id, client, tourId, status);
            maxBookingId = max(maxBookingId, id);
        }
        else {
            b = make_unique<Booking>(client, tourId);
            if (status == "Отменено") b->cancel();
            maxBookingId = max(maxBookingId, b->getId());
        }
        bookings.push_back(move(b));
    }
    Booking::setCounter(maxBookingId);
}
void Database::sortToursByPrice(bool ascending) {
    sort(tours.begin(), tours.end(),
        [ascending](const unique_ptr<Tour>& a, const unique_ptr<Tour>& b) {
            return ascending ? a->getPrice() < b->getPrice()
                : a->getPrice() > b->getPrice();
        });
}

void Database::sortToursByDuration(bool ascending) {
    sort(tours.begin(), tours.end(),
        [ascending](const unique_ptr<Tour>& a, const unique_ptr<Tour>& b) {
            return ascending ? a->getDuration() < b->getDuration()
                : a->getDuration() > b->getDuration();
        });
}

vector<const Tour*> Database::searchToursByCountry(const string& countryPart) const {
    vector<const Tour*> result;
    string needle = countryPart;
    transform(needle.begin(), needle.end(), needle.begin(),
        [](unsigned char c) { return static_cast<char>(tolower(c)); });

    for (const auto& t : tours) {
        string hay = t->getCountry();
        transform(hay.begin(), hay.end(), hay.begin(),
            [](unsigned char c) { return static_cast<char>(tolower(c)); });
        if (hay.find(needle) != string::npos) {
            result.push_back(t.get());
        }
    }
    return result;
}

vector<const Tour*> Database::filterTours(int minStars, double maxPrice, int minDays) const {
    vector<const Tour*> result;
    for (const auto& t : tours) {
        if (t->getStars() >= minStars &&
            t->getPrice() <= maxPrice &&
            t->getDuration() >= minDays) {
            result.push_back(t.get());
        }
    }
    return result;
}