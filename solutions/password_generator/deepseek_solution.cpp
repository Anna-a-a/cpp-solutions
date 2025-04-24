#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

string generatePassword(int length, bool useLower, bool useUpper, bool useDigits, bool useSpecial) {
    string lowerChars = "abcdefghijklmnopqrstuvwxyz";
    string upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string digits = "0123456789";
    string specialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    string allChars;
    if (useLower) allChars += lowerChars;
    if (useUpper) allChars += upperChars;
    if (useDigits) allChars += digits;
    if (useSpecial) allChars += specialChars;
    
    if (allChars.empty()) {
        return "";
    }
    
    string password;
    for (int i = 0; i < length; ++i) {
        password += allChars[rand() % allChars.size()];
    }
    
    // Ensure at least one character from each selected set
    if (useLower && password.find_first_of(lowerChars) == string::npos) {
        password[rand() % password.size()] = lowerChars[rand() % lowerChars.size()];
    }
    if (useUpper && password.find_first_of(upperChars) == string::npos) {
        password[rand() % password.size()] = upperChars[rand() % upperChars.size()];
    }
    if (useDigits && password.find_first_of(digits) == string::npos) {
        password[rand() % password.size()] = digits[rand() % digits.size()];
    }
    if (useSpecial && password.find_first_of(specialChars) == string::npos) {
        password[rand() % password.size()] = specialChars[rand() % specialChars.size()];
    }
    
    return password;
}

int main() {
    srand(time(0));
    
    cout << "Выберите уровень сложности пароля:" << endl;
    cout << "1. Простой (8 символов, только буквы)" << endl;
    cout << "2. Средний (12 символов, буквы и цифры)" << endl;
    cout << "3. Сложный (16 символов, буквы, цифры и спецсимволы)" << endl;
    
    int choice;
    cin >> choice;
    
    string password;
    switch (choice) {
        case 1:
            password = generatePassword(8, true, true, false, false);
            break;
        case 2:
            password = generatePassword(12, true, true, true, false);
            break;
        case 3:
            password = generatePassword(16, true, true, true, true);
            break;
        default:
            cout << "Неверный выбор!" << endl;
            return 1;
    }
    
    cout << "Ваш пароль: " << password << endl;
    
    return 0;
} 