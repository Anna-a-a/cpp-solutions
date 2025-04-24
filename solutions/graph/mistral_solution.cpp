#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream inFile("FileName.txt");
    int n;
    inFile >> n;

    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inFile >> adjMatrix[i][j];
        }
    }

    std::vector<int> degrees(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjMatrix[i][j] == 1) {
                degrees[i]++;
                if (i != j) degrees[j]++;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        std::cout << "Степень вершины " << i << ": " << degrees[i] << std::endl;
    }

    inFile.close();
    return 0;
} 