#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

// 연산자 우선순위 반환 함수
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 연산 수행 함수
double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b == 0 ? 0 : a / b;
    default: return 0;
    }
}

// 후위 표기식으로 변환하는 함수
string infixToPostfix(const string& infix, bool& valid) {
    stack<char> ops;
    stringstream output;
    valid = true;

    for (size_t i = 0; i < infix.length(); ++i) {
        char c = infix[i];

        if (isspace(c)) continue;

        if (isdigit(c) || c == '.' ||
            (c == '-' && (i == 0 || infix[i - 1] == '(' || infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == '*' || infix[i - 1] == '/'))) {
            // 음수 숫자 포함 숫자 처리
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
            ops.pop(); // 여는 괄호 제거
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // 특별 처리: 단항 마이너스 (-) 연산자 괄호 앞에 나오는 경우 -> -(...) => 0 - (...)
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

// 후위 표기식을 계산하는 함수
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
        cerr << "파일을 열 수 없습니다." << endl;
        return 1;
    }

    string line;
    while (getline(input, line)) {
        if (line.empty()) continue;

        bool valid;
        string postfix = infixToPostfix(line, valid);

        cout << "입력: " << line << endl;
        output << "입력: " << line << endl;

        if (!valid) {
            cout << "올바르지 않은 수식. 건너뜀.\n-----------------------\n";
            output << "올바르지 않은 수식. 건너뜀.\n-----------------------\n";
            continue;
        }

        double result = evaluatePostfix(postfix);

        cout << "출력: " << postfix << endl;
        cout << fixed << setprecision(6);
        cout << "계산: " << result << endl;
        cout << "-----------------------" << endl;

        output << "출력: " << postfix << endl;
        output << fixed << setprecision(6);
        output << "계산: " << result << endl;
        output << "-----------------------" << endl;
    }

    input.close();
    output.close();
    return 0;
}
