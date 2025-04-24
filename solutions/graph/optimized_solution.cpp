#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>

/**
 * @file optimized_solution.cpp
 * @brief Оптимизированная реализация калькулятора степеней графа
 * @details Объединяет лучшие аспекты решений DeepSeek, Mistral и GPT-4o
 */

// Класс исключения для обработки ошибок в работе с графом
// Взято из решения DeepSeek для улучшения обработки ошибок
class GraphError : public std::runtime_error {
public:
    // Создает новый объект исключения с указанным сообщением
    GraphError(const std::string& msg) : std::runtime_error(msg) {}
};

// Основной класс для работы с графом
// Объединяет функциональность из решений DeepSeek, Mistral и GPT-4o
class Graph {
private:
    std::vector<std::vector<int>> adjacencyMatrix;  // Матрица смежности графа
    int vertices;  // Количество вершин в графе
    
    // Проверяет корректность матрицы смежности
    // Взято из решения DeepSeek
    // Добавлены подробные проверки:
    // - Пустая матрица
    // - Несимметричная матрица
    // - Ненулевые элементы на диагонали
    // - Значения, отличные от 0 и 1
    void validateMatrix() const {
        // Проверка размерности
        if (adjacencyMatrix.empty()) {
            throw GraphError("Пустая матрица смежности");
        }
        
        int size = adjacencyMatrix.size();
        for (const auto& row : adjacencyMatrix) {
            if (row.size() != size) {
                throw GraphError("Матрица смежности не является квадратной");
            }
        }
        
        // Проверка симметричности и диагонали
        for (int i = 0; i < size; ++i) {
            if (adjacencyMatrix[i][i] != 0) {
                throw GraphError("На главной диагонали должны быть нули");
            }
            for (int j = 0; j < size; ++j) {
                if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) {
                    throw GraphError("Матрица смежности должна быть симметричной");
                }
                if (adjacencyMatrix[i][j] != 0 && adjacencyMatrix[i][j] != 1) {
                    throw GraphError("Матрица смежности должна содержать только 0 и 1");
                }
            }
        }
    }

public:
    // Конструктор по умолчанию
    Graph() : vertices(0) {}
    
    // Читает граф из файла
    // Взято из решения DeepSeek
    // Добавлена проверка количества вершин (от 1 до 100)
    void readFromFile(const std::string& filename) {
        std::ifstream fin(filename);
        if (!fin) {
            throw GraphError("Не удалось открыть входной файл: " + filename);
        }
        
        // Чтение размера матрицы
        fin >> vertices;
        if (vertices <= 0 || vertices > 100) {
            throw GraphError("Недопустимое количество вершин: " + std::to_string(vertices) + 
                            " (должно быть от 1 до 100)");
        }
        
        // Чтение матрицы смежности
        adjacencyMatrix.resize(vertices, std::vector<int>(vertices));
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (!(fin >> adjacencyMatrix[i][j])) {
                    throw GraphError("Ошибка при чтении элемента матрицы [" + 
                                    std::to_string(i) + "][" + std::to_string(j) + "]");
                }
            }
        }
        
        validateMatrix();
    }
    
    // Вычисляет степени всех вершин графа
    // Взято из решения GPT-4o
    // Использует std::count для подсчета единиц в строке матрицы
    std::vector<int> calculateDegrees() const {
        if (adjacencyMatrix.empty()) {
            throw GraphError("Граф не инициализирован");
        }
        
        std::vector<int> degrees(vertices);
        for (int i = 0; i < vertices; ++i) {
            degrees[i] = std::count(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 1);
        }
        return degrees;
    }
    
    // Записывает результаты в файл
    // Взято из решения DeepSeek
    // Добавлен вывод максимальной и минимальной степени вершин
    void writeResults(const std::string& filename, const std::vector<int>& degrees) const {
        if (degrees.size() != vertices) {
            throw GraphError("Размер вектора степеней не соответствует количеству вершин");
        }
        
        std::ofstream fout(filename);
        if (!fout) {
            throw GraphError("Не удалось открыть выходной файл: " + filename);
        }
        
        // Запись степеней вершин
        fout << "Степени вершин графа:\n";
        for (int i = 0; i < vertices; ++i) {
            fout << "Вершина " << (i + 1) << ": " << degrees[i] << "\n";
        }
        
        // Нахождение максимальной и минимальной степени
        auto [minIt, maxIt] = std::minmax_element(degrees.begin(), degrees.end());
        int minDegree = *minIt;
        int maxDegree = *maxIt;
        
        fout << "\nМинимальная степень вершины: " << minDegree << "\n";
        fout << "Максимальная степень вершины: " << maxDegree << "\n";
        
        // Анализ графа
        fout << "\nАнализ графа:\n";
        if (minDegree == 0) {
            fout << "- Граф содержит изолированные вершины\n";
        }
        if (maxDegree == vertices - 1) {
            fout << "- Граф содержит вершину, соединенную со всеми остальными\n";
        }
        if (minDegree == maxDegree && minDegree > 0) {
            fout << "- Граф является регулярным степени " << minDegree << "\n";
        }
        
        fout.close();
    }
};

// Основная функция программы
// Взято из решения DeepSeek
// Улучшена обработка ошибок с использованием try-catch
int main() {
    try {
        Graph graph;
        
        // Чтение графа из файла
        graph.readFromFile("FN1.txt");
        
        // Вычисление степеней вершин
        auto degrees = graph.calculateDegrees();
        
        // Запись результатов в файл
        graph.writeResults("FN2.txt", degrees);
        
        std::cout << "Расчет степеней вершин графа завершен успешно.\n";
        std::cout << "Результаты сохранены в файл FN2.txt\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
} 