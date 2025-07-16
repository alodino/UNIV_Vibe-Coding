#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

class InfixToPostfixCalculator {
private:
    // 연산자 우선순위 반환
    int getPrecedence(char op) {
        switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
        }
    }

    // 연산자인지 확인
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    // 문자열을 토큰으로 분리 (숫자와 연산자를 구분)
    vector<string> tokenize(const string& expression) {
        vector<string> tokens;
        string current = "";

        for (int i = 0; i < expression.length(); i++) {
            char c = expression[i];

            // 공백 무시
            if (c == ' ') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current = "";
                }
                continue;
            }

            // 숫자인 경우
            if (isdigit(c)) {
                current += c;
            }
            // 연산자나 괄호인 경우
            else if (isOperator(c) || c == '(' || c == ')') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current = "";
                }
                tokens.push_back(string(1, c));
            }
            else {
                // 잘못된 문자가 있으면 빈 벡터 반환
                return vector<string>();
            }
        }

        // 마지막 토큰 추가
        if (!current.empty()) {
            tokens.push_back(current);
        }

        return tokens;
    }

    // 중위 표기식을 후위 표기식으로 변환
    string infixToPostfix(const string& infix) {
        vector<string> tokens = tokenize(infix);

        // 토큰화 실패시 빈 문자열 반환
        if (tokens.empty()) {
            return "";
        }

        stack<char> operatorStack;
        string postfix = "";

        for (const string& token : tokens) {
            // 숫자인 경우
            if (isdigit(token[0])) {
                if (!postfix.empty()) {
                    postfix += " ";
                }
                postfix += token;
            }
            // 여는 괄호인 경우
            else if (token == "(") {
                operatorStack.push('(');
            }
            // 닫는 괄호인 경우
            else if (token == ")") {
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    postfix += " ";
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    // 괄호가 맞지 않는 경우
                    return "";
                }
                operatorStack.pop(); // '(' 제거
            }
            // 연산자인 경우
            else if (token.length() == 1 && isOperator(token[0])) {
                char op = token[0];
                while (!operatorStack.empty() &&
                    operatorStack.top() != '(' &&
                    getPrecedence(operatorStack.top()) >= getPrecedence(op)) {
                    postfix += " ";
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(op);
            }
            else {
                // 잘못된 토큰
                return "";
            }
        }

        // 스택에 남은 연산자들 처리
        while (!operatorStack.empty()) {
            if (operatorStack.top() == '(' || operatorStack.top() == ')') {
                // 괄호가 맞지 않는 경우
                return "";
            }
            postfix += " ";
            postfix += operatorStack.top();
            operatorStack.pop();
        }

        return postfix;
    }

    // 후위 표기식 계산
    int calculatePostfix(const string& postfix) {
        if (postfix.empty()) {
            return 0;
        }

        stack<int> numberStack;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            // 숫자인 경우
            if (isdigit(token[0])) {
                numberStack.push(stoi(token));
            }
            // 연산자인 경우
            else if (token.length() == 1 && isOperator(token[0])) {
                if (numberStack.size() < 2) {
                    return 0; // 에러 처리
                }

                int operand2 = numberStack.top();
                numberStack.pop();
                int operand1 = numberStack.top();
                numberStack.pop();

                int result;
                switch (token[0]) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 == 0) {
                        return 0; // 0으로 나누기 에러
                    }
                    result = operand1 / operand2;
                    break;
                default:
                    return 0;
                }
                numberStack.push(result);
            }
        }

        return numberStack.empty() ? 0 : numberStack.top();
    }

public:
    // 파일 처리 및 결과 출력
    void processFile(const string& inputFile, const string& outputFile) {
        ifstream input("C:/Users/USER/source/repos/Project9/x64/Debug/input.txt");
        ofstream output(outputFile);

        if (!input.is_open()) {
            cout << "입력 파일을 열 수 없습니다: " << inputFile << endl;
            return;
        }

        if (!output.is_open()) {
            cout << "출력 파일을 열 수 없습니다: " << outputFile << endl;
            return;
        }

        string line;
        while (getline(input, line)) {
            // 빈 줄 건너뛰기
            if (line.empty()) {
                continue;
            }

            // 중위 표기식을 후위 표기식으로 변환
            string postfix = infixToPostfix(line);

            // 변환 실패시 해당 줄 건너뛰기
            if (postfix.empty()) {
                cout << "잘못된 입력: " << line << " (건너뛰기)" << endl;
                continue;
            }

            // 후위 표기식 계산
            int result = calculatePostfix(postfix);

            // 결과 출력 (콘솔과 파일 모두)
            string outputLine = " 입력: " + line + "\n";
            outputLine += " 출력: " + postfix + "\n";
            outputLine += " 계산: " + to_string(result) + "\n";
            outputLine += " -----------------------\n";

            cout << outputLine;
            output << outputLine;
        }

        input.close();
        output.close();
    }
};

int main() {
    InfixToPostfixCalculator calculator;

    // input.txt 파일 처리하여 output.txt에 결과 저장
    calculator.processFile("input.txt", "output.txt");

    cout << "처리가 완료되었습니다. output.txt 파일을 확인해주세요." << endl;

    return 0;
}
