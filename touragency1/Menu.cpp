#include "Menu.h"
#include "Utils.h"
#include <iostream>
#include <limits>
using namespace std;

Menu::Menu() : currentUser(nullptr) {
	try {
		db.loadData("tours_data.txt");
	}
	catch (const exception& e) {
		cerr << e.what() << '\n';
	}
}

void Menu::run() {
	int choice = 0;
	do {
		cout << "\n=== СИСТЕМА УЧЕТА ТУРИСТИЧЕСКОГО АГЕНТСТВА ===\n";
		cout << "1. Войти в систему\n2. Зарегистрироваться (Клиент)\n3. Просмотреть тур\n0. Выход\n";
		cout << "Выберите пункт: ";

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(10000, '\n');
			break;
		}
		try {
			if (choice == 1) {
				loginMenu();
			}
			else if (choice == 2) {
				string uname, pwd, phone;
				cout << "Логин: "; cin >> uname;
				pwd = readPassword("Пароль: ");
				cout << "Телефон: "; cin >> phone;

				db.addUser(make_unique<Client>(uname, pwd, phone));
				cout << "Регистрация завершена успешно!\n";
			}
			else if (choice == 3) {
				cout << "--- Список туров ---\n";
				for (const auto& t : db.getTours()) {
					cout << *t << "\n";
				}
			}
		}
		catch (const exception& e) {
			cerr << "[Ошибка]: " << e.what() << "\n";
		}
	} while (choice != 0);
	try {
		db.saveData("tours_data.txt");
	}
	catch (...) {}
}

void Menu::loginMenu() {
	string uname, pwd;
	cout << "Введите логин: "; cin >> uname;
	pwd = readPassword("Введите пароль: ");

	User* u = db.findUser(uname);
	if (u && u->checkPassword(pwd)) {
		currentUser = u;
		cout << "Добро пожаловать, " << currentUser->getUsername() << " (" << currentUser->getRole() << ")!\n";

		if (currentUser->getRole() == "Admin") adminMenu();
		else if (currentUser->getRole() == "Manager") managerMenu();
		else if (currentUser->getRole() == "Client") clientMenu();

		currentUser = nullptr;
	}
	else {
		throw AuthException("Неверный логин или пароль.");
	}
}

void Menu::adminMenu() {
	int ch;
	do {
		cout << "--- Меню администратора ---\n1. Добавить тур\n2. Удалить тур\n3. Отчет по турам\n0. Выход из аккаунта\nВыбор: ";
		cin >> ch;
		switch (ch) {
		case 1: {
			string title, country;
			double price;
			int days;
			int stars;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Название: "; getline(cin, title);
			cout << "Страна: "; getline(cin, country);
			cout << "Цена: "; cin >> price;
			cout << "Дней: "; cin >> days;
			cout << "Звезды отеля (1-5): "; cin >> stars;
			db.addTours(title, country, price, days, stars);
			break;
		}
		case 2: {
			int id;
			cout << "ID тура для удаления: "; cin >> id;
			db.removeTour(id);
			break;
		}
		case 3: {
			cout << "Отчет: Всего туров в базе: " << db.getTours().size() << "\n";
			break;
		}
		}
	} while (ch != 0);
}

void Menu::managerMenu() {
	int ch;
	do {
		cout << "--- Меню менеджера ---\n1. Просмотр туров\n2. Добавить тур\n0. Выход из аккаунта\nВыбор: ";
		cin >> ch;
		switch (ch) {
		case 1: {
			for (const auto& t : db.getTours()) cout << *t << "\n";
			break;
		}
		case 2: {
			string title, country;
			double price;
			int days;
			int stars;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Название: "; getline(cin, title);
			cout << "Страна: "; getline(cin, country);
			cout << "Цена: "; cin >> price;
			cout << "Дней: "; cin >> days;
			cout << "Звезды отеля (1-5): "; cin >> stars;
			db.addTours(title, country, price, days, stars);
			break;
		}
		}
	} while (ch != 0);
}

void Menu::clientMenu() {
	int ch;
	do {
		cout << "--- Меню клиента ---\n"
			<< "1. Просмотр туров\n"
			<< "2. Поиск по стране\n"
			<< "3. Фильтр туров (звёзды/цена/дни)\n"
			<< "4. Сортировка туров\n"
			<< "5. Забронировать тур\n"
			<< "6. Мои брони\n"
			<< "7. Отменить бронь\n"
			<< "0. Выход из аккаунта\n"
			<< "Выбор: ";
		cin >> ch;
		try {
			switch (ch) {
			case 1: {
				for (const auto& t : db.getTours()) cout << *t << "\n";
				break;
			}
			case 2: {  
				string c;
				cout << "Введите страну (часть названия): ";
				cin >> c;
				auto found = db.searchToursByCountry(c);
				if (found.empty()) {
					cout << "По вашему запросу ничего не найдено.\n";
				}
				else {
					for (const Tour* t : found) cout << *t << "\n";
				}
				break;
			}
			case 3: {   
				int stars, days;
				double maxPrice;
				cout << "Мин. звёзды (1-5): "; cin >> stars;
				cout << "Макс. цена: ";       cin >> maxPrice;
				cout << "Мин. дней: ";        cin >> days;

				auto found = db.filterTours(stars, maxPrice, days);
				if (found.empty()) {
					cout << "По вашему запросу ничего не найдено.\n";
				}
				else {
					for (const Tour* t : found) cout << *t << "\n";
				}
				break;
			}
			case 4: {  
				int key, dir;
				cout << "Сортировать по: 1 - цене, 2 - длительности: ";
				cin >> key;
				cout << "Направление: 1 - по возрастанию, 2 - по убыванию: ";
				cin >> dir;
				bool asc = (dir == 1);
				if (key == 1) db.sortToursByPrice(asc);
				else          db.sortToursByDuration(asc);
				cout << "Отсортировано:\n";
				for (const auto& t : db.getTours()) cout << *t << "\n";
				break;
			}
			case 5: { 
				cout << "Доступные туры:\n";
				for (const auto& t : db.getTours()) cout << *t << "\n";
				int id;
				cout << "Введите ID тура для брони: ";
				if (!(cin >> id)) {
					cin.clear();
					cin.ignore(10000, '\n');
					throw ValidationException("ID должен быть числом.");
				}
				db.bookTour(currentUser->getUsername(), id);
				cout << "Тур успешно забронирован!\n";
				break;
			}
			case 6: {
				auto list = db.getBookingsFor(currentUser->getUsername());
				if (list.empty()) {
					cout << "У вас пока нет броней.\n";
				}
				else {
					for (const Booking* b : list) cout << *b << "\n";
				}
				break;
			}
			case 7: {
				int id;
				cout << "Введите ID брони для отмены: ";
				if (!(cin >> id)) {
					cin.clear();
					cin.ignore(10000, '\n');
					throw ValidationException("ID должен быть числом.");
				}
				db.cancelBooking(id, currentUser->getUsername());
				cout << "Бронь отменена.\n";
				break;
			}
			}
		}
		catch (const exception& e) {
			cerr << "[Ошибка]: " << e.what() << "\n";
		}
	} while (ch != 0);
}