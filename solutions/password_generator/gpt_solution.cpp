#include <iostream>
#include <string>
#include <random>
#include <ctime>

std::string generatePassword(int length, const std::string& chars) {
    std::string password;
    std::mt19937 rng(static_cast<unsigned>(time(0)));
    std::uniform_int_distribution<> dist(0, chars.size() - 1);
    for (int i = 0; i < length; ++i) {
        password += chars[dist(rng)];
    }
    return password;
}

int main() {
    int level;
    std::cout << "Выберите уровень сложности пароля (1 - легкий, 2 - средний, 3 - сложный): ";
    std::cin >> level;

    std::string chars;
    int length;

    switch (level) {
        case 1:
            chars = "abcdefghijklmnopqrstuvwxyz";
            length = 6;
            break;
        case 2:
            chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
            length = 10;
            break;
        case 3:
            chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=<>?";
            length = 16;
            break;
        default:
            std::cout << "Неверный уровень." << std::endl;
            return 1;
    }

    std::string password = generatePassword(length, chars);
    std::cout << "Сгенерированный пароль: " << password << std::endl;
    return 0;
} 