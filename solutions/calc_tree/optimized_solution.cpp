#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <iomanip>

/**
 * @file optimized_solution.cpp
 * @brief Оптимизированная реализация калькулятора выражений
 * @details Оптимизировано на основе решений DeepSeek и GPT-4o
 */

// Класс исключения для обработки ошибок в выражениях
// Взято из решения DeepSeek для улучшения обработки ошибок
class ExpressionError : public std::runtime_error {
public:
    // Создает новый объект исключения с указанным сообщением
    ExpressionError(const std::string& msg) : std::runtime_error(msg) {}
};

// Класс для представления узла в дереве выражений
// Взято из решения DeepSeek
// Использует умные указатели (std::shared_ptr) для автоматического управления памятью
class TreeNode {
public:
    std::string value;  // Значение узла (оператор или операнд)
    std::shared_ptr<TreeNode> left;  // Левый потомок узла
    std::shared_ptr<TreeNode> right;  // Правый потомок узла
    
    // Создает новый узел с указанным значением
    TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
};

// Основной класс для работы с деревом выражений
// Объединяет функциональность из решений DeepSeek, Mistral и GPT-4o
class ExpressionTree {
private:
    std::shared_ptr<TreeNode> root;
    
    // Определяет приоритет оператора
    // Взято из решения DeepSeek
    // Увеличен приоритет степени (^) до 4, что соответствует математическим правилам
    int getPriority(const std::string& op) {
        if (op == "^") return 4;
        if (op == "*" || op == "/" || op == "%") return 3;
        if (op == "+" || op == "-") return 2;
        return 0;
    }
    
    // Проверяет, является ли строка оператором
    bool isOperator(const std::string& c) {
        return c == "+" || c == "-" || c == "*" || c == "/" || c == "%" || c == "^";
    }
    
    // Проверяет, является ли строка допустимым числом
    // Взято из решения DeepSeek
    // Добавлена проверка диапазона (от 1 до 30) для предотвращения переполнения
    bool isValidNumber(const std::string& num) {
        try {
            int val = std::stoi(num);
            return val >= 1 && val <= 30;
        } catch (...) {
            return false;
        }
    }
    
    // Разбивает выражение на токены
    // Взято из решения DeepSeek
    // Улучшена проверка токенов с использованием функций isOperator и isValidNumber
    std::vector<std::string> tokenize(const std::string& expr) {
        std::vector<std::string> tokens;
        std::istringstream iss(expr);
        std::string token;
        
        while (iss >> token) {
            if (token == "x" || isOperator(token) || isValidNumber(token)) {
                tokens.push_back(token);
            } else {
                throw ExpressionError("Недопустимый токен: " + token);
            }
        }
        
        if (tokens.empty()) {
            throw ExpressionError("Пустое выражение");
        }
        
        return tokens;
    }
    
    // Строит дерево выражения из токенов
    // Взято из решения GPT-4o
    // Использует лямбда-функцию для применения операторов, что делает код более компактным
    std::shared_ptr<TreeNode> buildTreeFromTokens(const std::vector<std::string>& tokens) {
        std::stack<std::shared_ptr<TreeNode>> nodes;
        std::stack<std::string> ops;
        
        auto applyOp = [&]() {
            if (nodes.size() < 2) {
                throw ExpressionError("Недостаточно операндов для оператора");
            }
            
            std::string op = ops.top(); ops.pop();
            auto right = nodes.top(); nodes.pop();
            auto left = nodes.top(); nodes.pop();
            
            auto newNode = std::make_shared<TreeNode>(op);
            newNode->left = left;
            newNode->right = right;
            nodes.push(newNode);
        };
        
        for (const auto& token : tokens) {
            if (token == "x" || isValidNumber(token)) {
                nodes.push(std::make_shared<TreeNode>(token));
            } else if (isOperator(token)) {
                while (!ops.empty() && getPriority(ops.top()) >= getPriority(token)) {
                    applyOp();
                }
                ops.push(token);
            }
        }
        
        while (!ops.empty()) {
            applyOp();
        }
        
        if (nodes.size() != 1) {
            throw ExpressionError("Некорректное выражение");
        }
        
        return nodes.top();
    }
    
    // Вычисляет значение поддерева с корнем в данном узле
    // Взято из решения DeepSeek
    // Добавлена проверка деления на ноль и остатка от деления на ноль
    int evaluateNode(const std::shared_ptr<TreeNode>& node, int x) {
        if (!node) return 0;
        
        if (node->value == "x") return x;
        if (!isOperator(node->value)) return std::stoi(node->value);
        
        int left = evaluateNode(node->left, x);
        int right = evaluateNode(node->right, x);
        
        if (node->value == "+") return left + right;
        if (node->value == "-") return left - right;
        if (node->value == "*") return left * right;
        if (node->value == "/") {
            if (right == 0) throw ExpressionError("Деление на ноль");
            return left / right;
        }
        if (node->value == "%") {
            if (right == 0) throw ExpressionError("Остаток от деления на ноль");
            return left % right;
        }
        if (node->value == "^") {
            if (right < 0) throw ExpressionError("Отрицательная степень не поддерживается");
            return static_cast<int>(std::pow(left, right));
        }
        
        throw ExpressionError("Неизвестный оператор: " + node->value);
    }
    
    // Преобразует поддерево по правилу x*A → A*x
    // Взято из решения DeepSeek
    // Использует рекурсивный подход для обхода всего дерева
    void transformNode(std::shared_ptr<TreeNode>& node) {
        if (!node) return;
        
        transformNode(node->left);
        transformNode(node->right);
        
        if (node->value == "*" && node->left && node->left->value == "x") {
            std::swap(node->left, node->right);
        }
    }
    
    // Печатает поддерево в текстовом виде
    // Взято из решения DeepSeek
    // Использует отступы для визуального представления структуры дерева
    void printNode(const std::shared_ptr<TreeNode>& node, int depth, std::ostream& out) {
        if (!node) return;
        
        printNode(node->right, depth + 1, out);
        out << std::setw(depth * 4) << "" << node->value << "\n";
        printNode(node->left, depth + 1, out);
    }
    
public:
    // Строит дерево из строкового выражения
    void buildFromExpression(const std::string& expr) {
        auto tokens = tokenize(expr);
        root = buildTreeFromTokens(tokens);
    }
    
    // Вычисляет значение выражения
    int evaluate(int x) {
        if (!root) throw ExpressionError("Пустое дерево выражений");
        return evaluateNode(root, x);
    }
    
    // Преобразует дерево по правилу x*A → A*x
    void transform() {
        if (!root) throw ExpressionError("Пустое дерево выражений");
        transformNode(root);
    }
    
    // Печатает дерево в текстовом виде
    void print(std::ostream& out) {
        if (!root) throw ExpressionError("Пустое дерево выражений");
        printNode(root, 0, out);
    }
};

// Основная функция программы
// Читает выражение из файла FN1.txt, запрашивает значение x,
// вычисляет результат и записывает его вместе с преобразованным
// деревом в файл FN2.txt
// Улучшена обработка ошибок с использованием try-catch
int main() {
    try {
        // Открытие входного файла
        std::ifstream fin("FN1.txt");
        if (!fin) {
            throw ExpressionError("Не удалось открыть входной файл FN1.txt");
        }
        
        // Чтение выражения
        std::string expr;
        std::getline(fin, expr);
        fin.close();
        
        if (expr.empty()) {
            throw ExpressionError("Входной файл пуст");
        }
        
        // Запрос значения x
        int x;
        std::cout << "Введите значение x: ";
        std::cin >> x;
        
        // Построение и вычисление дерева
        ExpressionTree tree;
        tree.buildFromExpression(expr);
        
        int result = tree.evaluate(x);
        
        // Преобразование дерева
        tree.transform();
        
        // Запись результатов в файл
        std::ofstream fout("FN2.txt");
        if (!fout) {
            throw ExpressionError("Не удалось открыть выходной файл FN2.txt");
        }
        
        fout << "Исходное выражение: " << expr << "\n";
        fout << "Значение x: " << x << "\n";
        fout << "Результат вычисления: " << result << "\n\n";
        fout << "Преобразованное дерево:\n";
        tree.print(fout);
        
        fout.close();
        
        std::cout << "Результат: " << result << "\n";
        std::cout << "Результаты сохранены в файл FN2.txt\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
} 