#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>
#include <stdexcept>

// Структура для хранения параметров генерации пароля
// Взято из решения GPT-4o
// Использование структуры вместо отдельных переменных делает код более организованным
struct PasswordParams {
    int length;  // Длина пароля (от 8 до 20 символов)
    bool useUppercase;  // Использовать ли заглавные буквы
    bool useLowercase;  // Использовать ли строчные буквы
    bool useDigits;  // Использовать ли цифры
    bool useSpecial;  // Использовать ли специальные символы
};

// Основной класс для генерации и проверки паролей
// Объединяет функциональность из решений DeepSeek, Mistral и GPT-4o
class PasswordGenerator {
private:
    // Генерирует случайный символ заданного типа
    // Взято из решения GPT-4o
    // Использует std::random для генерации случайных чисел
    char getRandomChar(const std::string& charType) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, charType.length() - 1);
        return charType[dis(gen)];
    }
    
    // Перемешивает символы в строке случайным образом
    // Взято из решения GPT-4o
    // Использует std::shuffle для перемешивания символов
    std::string shuffleString(std::string str) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::shuffle(str.begin(), str.end(), gen);
        return str;
    }
    
public:
    // Получает параметры генерации пароля от пользователя
    // Взято из решения GPT-4o
    // Добавлена валидация входных данных с информативными сообщениями об ошибках
    PasswordParams getParams() {
        PasswordParams params;
        
        std::cout << "Введите длину пароля (8-20 символов): ";
        std::cin >> params.length;
        
        while (params.length < 8 || params.length > 20) {
            std::cout << "Длина пароля должна быть от 8 до 20 символов. Попробуйте снова: ";
            std::cin >> params.length;
        }
        
        char choice;
        
        std::cout << "Использовать заглавные буквы (A-Z)? (y/n): ";
        std::cin >> choice;
        params.useUppercase = (choice == 'y' || choice == 'Y');
        
        std::cout << "Использовать строчные буквы (a-z)? (y/n): ";
        std::cin >> choice;
        params.useLowercase = (choice == 'y' || choice == 'Y');
        
        std::cout << "Использовать цифры (0-9)? (y/n): ";
        std::cin >> choice;
        params.useDigits = (choice == 'y' || choice == 'Y');
        
        std::cout << "Использовать специальные символы (!@#$%^&*)? (y/n): ";
        std::cin >> choice;
        params.useSpecial = (choice == 'y' || choice == 'Y');
        
        // Проверка, что хотя бы один тип символов выбран
        if (!params.useUppercase && !params.useLowercase && !params.useDigits && !params.useSpecial) {
            std::cout << "Должен быть выбран хотя бы один тип символов. Установлены все типы по умолчанию.\n";
            params.useUppercase = params.useLowercase = params.useDigits = params.useSpecial = true;
        }
        
        return params;
    }
    
    // Генерирует пароль согласно заданным параметрам
    // Взято из решения GPT-4o
    // Гарантирует наличие хотя бы одного символа каждого выбранного типа
    std::string generatePassword(const PasswordParams& params) {
        const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
        const std::string digits = "0123456789";
        const std::string special = "!@#$%^&*()_+-=[]{}|;:,.<>?";
        
        std::string password;
        std::string allChars;
        
        // Добавляем хотя бы один символ каждого выбранного типа
        if (params.useUppercase) {
            password += getRandomChar(uppercase);
            allChars += uppercase;
        }
        if (params.useLowercase) {
            password += getRandomChar(lowercase);
            allChars += lowercase;
        }
        if (params.useDigits) {
            password += getRandomChar(digits);
            allChars += digits;
        }
        if (params.useSpecial) {
            password += getRandomChar(special);
            allChars += special;
        }
        
        // Добавляем оставшиеся символы
        while (password.length() < params.length) {
            password += getRandomChar(allChars);
        }
        
        // Перемешиваем пароль
        return shuffleString(password);
    }
    
    // Проверяет сложность пароля
    // Взято из решения DeepSeek
    // Добавлена оценка сложности по 100-балльной шкале
    int checkPasswordStrength(const std::string& password) {
        int score = 0;
        
        // Оценка длины
        if (password.length() >= 12) {
            score += 30;
        } else if (password.length() >= 8) {
            score += 20;
        } else {
            score += 10;
        }
        
        // Оценка разнообразия символов
        bool hasUppercase = false;
        bool hasLowercase = false;
        bool hasDigit = false;
        bool hasSpecial = false;
        
        for (char c : password) {
            if (std::isupper(c)) hasUppercase = true;
            else if (std::islower(c)) hasLowercase = true;
            else if (std::isdigit(c)) hasDigit = true;
            else hasSpecial = true;
        }
        
        if (hasUppercase) score += 15;
        if (hasLowercase) score += 15;
        if (hasDigit) score += 15;
        if (hasSpecial) score += 15;
        
        // Оценка распределения символов
        int uniqueChars = 0;
        for (size_t i = 0; i < password.length(); ++i) {
            bool isUnique = true;
            for (size_t j = 0; j < i; ++j) {
                if (password[i] == password[j]) {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique) uniqueChars++;
        }
        
        score += (uniqueChars * 10) / password.length();
        
        return score;
    }
    
    // Сохраняет пароль в файл
    // Взято из решения DeepSeek
    // Добавлена проверка открытия файла
    void saveToFile(const std::string& password, const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }
        
        int strength = checkPasswordStrength(password);
        file << "Сгенерированный пароль: " << password << "\n";
        file << "Оценка сложности: " << strength << "/100\n";
        
        if (strength >= 80) {
            file << "Оценка: Отличный пароль\n";
        } else if (strength >= 60) {
            file << "Оценка: Хороший пароль\n";
        } else if (strength >= 40) {
            file << "Оценка: Средний пароль\n";
        } else {
            file << "Оценка: Слабый пароль\n";
        }
        
        file.close();
    }
};

// Основная функция программы
// Взято из решения DeepSeek
// Улучшена обработка ошибок с использованием try-catch
int main() {
    try {
        PasswordGenerator generator;
        
        std::cout << "Генератор паролей\n";
        std::cout << "=================\n\n";
        
        PasswordParams params = generator.getParams();
        std::string password = generator.generatePassword(params);
        
        std::cout << "\nСгенерированный пароль: " << password << "\n";
        
        int strength = generator.checkPasswordStrength(password);
        std::cout << "Оценка сложности: " << strength << "/100\n";
        
        if (strength >= 80) {
            std::cout << "Оценка: Отличный пароль\n";
        } else if (strength >= 60) {
            std::cout << "Оценка: Хороший пароль\n";
        } else if (strength >= 40) {
            std::cout << "Оценка: Средний пароль\n";
        } else {
            std::cout << "Оценка: Слабый пароль\n";
        }
        
        generator.saveToFile(password, "password.txt");
        std::cout << "\nПароль сохранен в файл password.txt\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
} 