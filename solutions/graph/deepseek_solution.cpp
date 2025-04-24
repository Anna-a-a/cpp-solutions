#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Graph {
private:
    vector<vector<int>> adjacencyMatrix;
    int vertexCount;
    
    bool isValidMatrix() const {
        if (adjacencyMatrix.empty() || adjacencyMatrix.size() != vertexCount) {
            return false;
        }
        
        for (const auto& row : adjacencyMatrix) {
            if (row.size() != vertexCount) {
                return false;
            }
            for (int val : row) {
                if (val != 0 && val != 1) {
                    return false;
                }
            }
        }
        return true;
    }
    
public:
    Graph() : vertexCount(0) {}
    
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file: " + filename);
        }
        
        string line;
        getline(file, line);
        vertexCount = stoi(line);
        
        adjacencyMatrix.resize(vertexCount, vector<int>(vertexCount));
        
        for (int i = 0; i < vertexCount; i++) {
            getline(file, line);
            istringstream iss(line);
            for (int j = 0; j < vertexCount; j++) {
                iss >> adjacencyMatrix[i][j];
            }
        }
        
        if (!isValidMatrix()) {
            throw runtime_error("Invalid adjacency matrix");
        }
    }
    
    vector<int> calculateDegrees() const {
        vector<int> degrees(vertexCount, 0);
        
        for (int i = 0; i < vertexCount; i++) {
            for (int j = 0; j < vertexCount; j++) {
                degrees[i] += adjacencyMatrix[i][j];
            }
        }
        
        return degrees;
    }
    
    void printDegrees() const {
        vector<int> degrees = calculateDegrees();
        for (int i = 0; i < vertexCount; i++) {
            cout << "Vertex " << (i + 1) << ": " << degrees[i] << endl;
        }
    }
};

int main() {
    try {
        Graph graph;
        graph.readFromFile("input.txt");
        graph.printDegrees();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
} 