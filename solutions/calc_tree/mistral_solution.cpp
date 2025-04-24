#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <map>

struct TreeNode {
    std::string value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
};

int precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^") return 3;
    return 0;
}

TreeNode* buildTree(std::istringstream& tokens) {
    std::stack<TreeNode*> values;
    std::stack<std::string> ops;

    std::string token;
    while (tokens >> token) {
        if (isdigit(token[0]) || token == "x") {
            values.push(new TreeNode(token));
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                TreeNode* right = values.top(); values.pop();
                TreeNode* left = values.top(); values.pop();
                TreeNode* node = new TreeNode(ops.top());
                node->left = left;
                node->right = right;
                values.push(node);
                ops.pop();
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        TreeNode* right = values.top(); values.pop();
        TreeNode* left = values.top(); values.pop();
        TreeNode* node = new TreeNode(ops.top());
        node->left = left;
        node->right = right;
        values.push(node);
        ops.pop();
    }

    return values.top();
}

int evaluate(TreeNode* node, int x) {
    if (!node->left && !node->right) {
        if (node->value == "x") return x;
        return std::stoi(node->value);
    }

    int leftVal = evaluate(node->left, x);
    int rightVal = evaluate(node->right, x);

    if (node->value == "+") return leftVal + rightVal;
    if (node->value == "-") return leftVal - rightVal;
    if (node->value == "*") return leftVal * rightVal;
    if (node->value == "/") return leftVal / rightVal;
    if (node->value == "%") return leftVal % rightVal;
    if (node->value == "^") {
        int result = 1;
        for (int i = 0; i < rightVal; ++i) {
            result *= leftVal;
        }
        return result;
    }

    return 0;
}

void transformTree(TreeNode* node) {
    if (!node) return;
    transformTree(node->left);
    transformTree(node->right);

    if (node->value == "*" && node->left && node->left->value == "x") {
        TreeNode* temp = node->left;
        node->left = node->right;
        node->right = temp;
    }
}

void printTree(TreeNode* node, std::ofstream& out, int level = 0) {
    if (!node) return;
    printTree(node->right, out, level + 1);
    out << std::string(level * 4, ' ') << node->value << std::endl;
    printTree(node->left, out, level + 1);
}

int main() {
    std::ifstream inFile("FN1.txt");
    std::ofstream outFile("FN2.txt");
    std::string expression;
    std::getline(inFile, expression);

    std::istringstream tokens(expression);
    TreeNode* root = buildTree(tokens);

    int x;
    std::cout << "Введите значение переменной x: ";
    std::cin >> x;

    int result = evaluate(root, x);
    outFile << "Результат: " << result << std::endl;

    transformTree(root);
    printTree(root, outFile);

    inFile.close();
    outFile.close();

    return 0;
} 