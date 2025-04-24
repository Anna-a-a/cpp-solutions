#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <algorithm>

using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;
    
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};

int precedence(const string& op) {
    if (op == "^") return 4;
    if (op == "*" || op == "/" || op == "%") return 3;
    if (op == "+" || op == "-") return 2;
    return 0;
}

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
}

vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    for (size_t i = 0; i < expr.size(); ) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }
        
        if (isdigit(expr[i])) {
            string num;
            while (i < expr.size() && isdigit(expr[i])) {
                num += expr[i++];
            }
            tokens.push_back(num);
        } else if (expr[i] == 'x') {
            tokens.push_back("x");
            i++;
        } else if (isOperator(string(1, expr[i]))) {
            tokens.push_back(string(1, expr[i]));
            i++;
        } else {
            i++;
        }
    }
    return tokens;
}

vector<string> infixToPostfix(const vector<string>& tokens) {
    vector<string> output;
    stack<string> opStack;
    
    for (const auto& token : tokens) {
        if (isdigit(token[0])) {
            output.push_back(token);
        } else if (token == "x") {
            output.push_back(token);
        } else if (isOperator(token)) {
            while (!opStack.empty() && opStack.top() != "(" && 
                   (precedence(opStack.top()) > precedence(token) || 
                   (precedence(opStack.top()) == precedence(token) && token != "^"))) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        } else if (token == "(") {
            opStack.push(token);
        } else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.pop();
        }
    }
    
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }
    
    return output;
}

Node* buildExpressionTree(const vector<string>& postfix) {
    stack<Node*> st;
    
    for (const auto& token : postfix) {
        if (isOperator(token)) {
            Node* right = st.top(); st.pop();
            Node* left = st.top(); st.pop();
            Node* node = new Node(token);
            node->left = left;
            node->right = right;
            st.push(node);
        } else {
            st.push(new Node(token));
        }
    }
    
    return st.top();
}

int evaluate(Node* root, int x) {
    if (!root) return 0;
    
    if (isdigit(root->value[0])) {
        return stoi(root->value);
    } else if (root->value == "x") {
        return x;
    }
    
    int left = evaluate(root->left, x);
    int right = evaluate(root->right, x);
    
    if (root->value == "+") return left + right;
    if (root->value == "-") return left - right;
    if (root->value == "*") return left * right;
    if (root->value == "/") return left / right;
    if (root->value == "%") return left % right;
    if (root->value == "^") return (int)pow(left, right);
    
    return 0;
}

void transformTree(Node* root) {
    if (!root) return;
    
    transformTree(root->left);
    transformTree(root->right);
    
    if (root->value == "*" && root->left && root->left->value == "x") {
        swap(root->left, root->right);
    }
}

void printTree(Node* root, ofstream& out, int level = 0) {
    if (!root) return;
    
    printTree(root->right, out, level + 1);
    
    out << string(4 * level, ' ') << root->value << endl;
    
    printTree(root->left, out, level + 1);
}

int main() {
    ifstream in("FN1.txt");
    ofstream out("FN2.txt");
    
    string expr;
    getline(in, expr);
    
    int x;
    cout << "Enter value of x: ";
    cin >> x;
    
    vector<string> tokens = tokenize(expr);
    vector<string> postfix = infixToPostfix(tokens);
    Node* root = buildExpressionTree(postfix);
    
    int result = evaluate(root, x);
    out << "Result: " << result << endl << endl;
    
    transformTree(root);
    out << "Transformed tree:" << endl;
    printTree(root, out);
    
    in.close();
    out.close();
    
    return 0;
} 