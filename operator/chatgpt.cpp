#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

// ������ �켱���� ��ȯ �Լ�
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// ���� ���� �Լ�
double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b == 0 ? 0 : a / b;
    default: return 0;
    }
}

// ���� ǥ������� ��ȯ�ϴ� �Լ�
string infixToPostfix(const string& infix, bool& valid) {
    stack<char> ops;
    stringstream output;
    valid = true;

    for (size_t i = 0; i < infix.length(); ++i) {
        char c = infix[i];

        if (isspace(c)) continue;

        if (isdigit(c) || c == '.' ||
            (c == '-' && (i == 0 || infix[i - 1] == '(' || infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == '*' || infix[i - 1] == '/'))) {
            // ���� ���� ���� ���� ó��
            output << c;
            ++i;
            while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                output << infix[i++];
            }
            output << ' ';
            --i;
        }
        else if (c == '(') {
            ops.push(c);
        }
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output << ops.top() << ' ';
                ops.pop();
            }
            if (ops.empty()) {
                valid = false;
                return "";
            }
            ops.pop(); // ���� ��ȣ ����
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // Ư�� ó��: ���� ���̳ʽ� (-) ������ ��ȣ �տ� ������ ��� -> -(...) => 0 - (...)
            if (c == '-' && i + 1 < infix.length() && infix[i + 1] == '(' &&
                (i == 0 || infix[i - 1] == '(' || infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == '*' || infix[i - 1] == '/')) {
                output << "0 ";
                ops.push('-');
                continue;
            }
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                output << ops.top() << ' ';
                ops.pop();
            }
            ops.push(c);
        }
        else {
            valid = false;
            return "";
        }
    }

    while (!ops.empty()) {
        if (ops.top() == '(') {
            valid = false;
            return "";
        }
        output << ops.top() << ' ';
        ops.pop();
    }

    return output.str();
}

// ���� ǥ����� ����ϴ� �Լ�
double evaluatePostfix(const string& postfix) {
    stack<double> st;
    stringstream ss(postfix);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1) || token.find('.') != string::npos) {
            st.push(stod(token));
        }
        else if (token.length() == 1 && string("+-*/").find(token[0]) != string::npos) {
            if (st.size() < 2) return 0;
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            st.push(applyOperation(a, b, token[0]));
        }
        else {
            return 0;
        }
    }
    return st.top();
}

int main() {
    ifstream input("input.txt");
    ofstream output("output.txt");

    if (!input.is_open() || !output.is_open()) {
        cerr << "������ �� �� �����ϴ�." << endl;
        return 1;
    }

    string line;
    while (getline(input, line)) {
        if (line.empty()) continue;

        bool valid;
        string postfix = infixToPostfix(line, valid);

        cout << "�Է�: " << line << endl;
        output << "�Է�: " << line << endl;

        if (!valid) {
            cout << "�ùٸ��� ���� ����. �ǳʶ�.\n-----------------------\n";
            output << "�ùٸ��� ���� ����. �ǳʶ�.\n-----------------------\n";
            continue;
        }

        double result = evaluatePostfix(postfix);

        cout << "���: " << postfix << endl;
        cout << fixed << setprecision(6);
        cout << "���: " << result << endl;
        cout << "-----------------------" << endl;

        output << "���: " << postfix << endl;
        output << fixed << setprecision(6);
        output << "���: " << result << endl;
        output << "-----------------------" << endl;
    }

    input.close();
    output.close();
    return 0;
}
