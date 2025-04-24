#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <map>
#include <cmath>
#include <iomanip>

struct Node {
    std::string value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(std::string val) : value(val) {}
};

// приоритет операций
int getPriority(const std::string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool isOperator(const std::string& c) {
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "%" || c == "^";
}

Node* buildTreeFromInfix(const std::string& expr) {
    std::istringstream iss(expr);
    std::stack<Node*> nodes;
    std::stack<std::string> ops;

    auto applyOp = [&]() {
        std::string op = ops.top(); ops.pop();
        Node* right = nodes.top(); nodes.pop();
        Node* left = nodes.top(); nodes.pop();
        Node* newNode = new Node(op);
        newNode->left = left;
        newNode->right = right;
        nodes.push(newNode);
    };

    std::string token;
    while (iss >> token) {
        if (isdigit(token[0]) || token == "x") {
            nodes.push(new Node(token));
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

    return nodes.top();
}

int eval(Node* root, int x) {
    if (!root) return 0;
    if (root->value == "x") return x;
    if (!isOperator(root->value)) return std::stoi(root->value);
    int left = eval(root->left, x);
    int right = eval(root->right, x);
    if (root->value == "+") return left + right;
    if (root->value == "-") return left - right;
    if (root->value == "*") return left * right;
    if (root->value == "/") return right != 0 ? left / right : 0;
    if (root->value == "%") return right != 0 ? left % right : 0;
    if (root->value == "^") return (int)pow(left, right);
    return 0;
}

// Поворот поддеревьев x*A → A*x
void transformTree(Node* root) {
    if (!root) return;
    transformTree(root->left);
    transformTree(root->right);
    if (root->value == "*" && root->left && root->left->value == "x") {
        std::swap(root->left, root->right);
    }
}

void printTree(Node* root, int depth = 0, std::ostream& out = std::cout) {
    if (!root) return;
    printTree(root->right, depth + 1, out);
    out << std::setw(depth * 4) << "" << root->value << "\n";
    printTree(root->left, depth + 1, out);
}

int main() {
    std::ifstream fin("FN1.txt");
    std::ofstream fout("FN2.txt");
    std::string expr;
    std::getline(fin, expr);
    int x;
    fin >> x;

    Node* root = buildTreeFromInfix(expr);
    int result = eval(root, x);
    fout << "Result: " << result << "\n";

    transformTree(root);
    fout << "Transformed Tree:\n";
    printTree(root, 0, fout);

    return 0;
} 