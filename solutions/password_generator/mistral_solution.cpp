#include <iostream>
#include <string>
#include <random>
#include <ctime>

std::string generatePassword(int level) {
    std::string chars;
    int length;

    switch (level) {
        case 1:
            chars = "abcdefghijklmnopqrstuvwxyz";
            length = 8;
            break;
        case 2:
            chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            length = 12;
            break;
        case 3:
            chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
            length = 16;
            break;
        default:
            return "";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string password;
    for (int i = 0; i < length; ++i) {
        password += chars[dis(gen)];
    }

    return password;
}

int main() {
    int level;
    std::cout << "Введите уровень сложности пароля (1, 2 или 3): ";
    std::cin >> level;

    std::string password = generatePassword(level);
    std::cout << "Сгенерированный пароль: " << password << std::endl;

    return 0;
} 