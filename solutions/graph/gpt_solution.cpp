#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream in("FileName.txt");
    int n;
    in >> n;

    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    std::vector<int> degrees(n, 0);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            in >> matrix[i][j];
            if (matrix[i][j]) {
                degrees[i] += matrix[i][j];
                if (i == j) degrees[i]++; // учёт петли дважды
            }
        }

    std::cout << "Степени вершин:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "Вершина " << i + 1 << ": " << degrees[i] << "\n";
    }

    return 0;
} 