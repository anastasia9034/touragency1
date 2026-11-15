#pragma once
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

inline string readPassword(const string& prompt = "Пароль: ") {
    cout << prompt;
    string pwd;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n')
            break;
        if (ch == '\b') {
            if (!pwd.empty()) {
                pwd.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= 32 && ch <= 126) {
            pwd += ch;
            cout << '*';
        }
    }
    cout << '\n';
    return pwd;
}