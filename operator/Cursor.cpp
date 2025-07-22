// 사용된 라이브러리들
#include <iostream>    // 입출력 스트림 (cout, cin)
#include <fstream>     // 파일 입출력 (ifstream, ofstream)
#include <string>      // 문자열 처리 (string)
#include <stack>       // 스택 자료구조 (stack)
#include <sstream>     // 문자열 스트림 (stringstream)
using namespace std;

// 연산자 우선순위 반환
int getPriority(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// 연산자인지 확인
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// 중위 표기법을 후위 표기법으로 변환
string infixToPostfix(string infix) {
    stack<char> operators;
    string postfix = "";
    
    for (int i = 0; i < infix.length(); i++) {
        char c = infix[i];
        
        // 공백 무시
        if (c == ' ') continue;
        
        // 숫자인 경우
        if (isdigit(c)) {
            while (i < infix.length() && isdigit(infix[i])) {
                postfix += infix[i];
                i++;
            }
            postfix += " ";
            i--;
        }
        // 여는 괄호인 경우
        else if (c == '(') {
            operators.push(c);
        }
        // 닫는 괄호인 경우
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                postfix += " ";
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // '(' 제거
        }
        // 연산자인 경우
        else if (isOperator(c)) {
            while (!operators.empty() && operators.top() != '(' && 
                   getPriority(operators.top()) >= getPriority(c)) {
                postfix += operators.top();
                postfix += " ";
                operators.pop();
            }
            operators.push(c);
        }
    }
    
    // 남은 연산자들 처리
    while (!operators.empty()) {
        postfix += operators.top();
        postfix += " ";
        operators.pop();
    }
    
    return postfix;
}

// 후위 표기법 계산
double calculate(string postfix) {
    stack<double> operands;
    stringstream ss(postfix);
    string token;
    
    while (ss >> token) {
        if (isdigit(token[0])) {
            operands.push(stod(token));
        }
        else {
            double b = operands.top(); operands.pop();
            double a = operands.top(); operands.pop();
            
            if (token == "+") operands.push(a + b);
            else if (token == "-") operands.push(a - b);
            else if (token == "*") operands.push(a * b);
            else if (token == "/") operands.push(a / b);
        }
    }
    
    return operands.top();
}

int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");
    
    if (!inputFile.is_open()) {
        cout << "input.txt 파일을 열 수 없습니다!" << endl;
        return 1;
    }
    
    string line;
    
    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        
        // 중위 표기법을 후위 표기법으로 변환
        string postfix = infixToPostfix(line);
        
        // 후위 표기법 계산
        double result = calculate(postfix);
        
        // 화면 출력
        cout << " 입력: " << line << endl;
        cout << " 출력: " << postfix << endl;
        cout << " 계산: " << (int)result << endl;
        cout << " -----------------------" << endl;
        
        // 파일 출력
        outputFile << " 입력: " << line << endl;
        outputFile << " 출력: " << postfix << endl;
        outputFile << " 계산: " << (int)result << endl;
        outputFile << " -----------------------" << endl;
    }
    
    inputFile.close();
    outputFile.close();
    
    cout << "\n결과가 output.txt 파일에도 저장되었습니다." << endl;
    
    return 0;
}