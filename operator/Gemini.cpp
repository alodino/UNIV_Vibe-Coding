#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <cctype>

// 연산자의 우선순위를 반환하는 함수
int get_precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == '/') {
        return 2;
    }
    return 0; // 괄호의 경우
}

// 중위 표기식을 후위 표기식으로 변환하는 함수
std::string infix_to_postfix(const std::string& infix) {
    std::stack<char> s;
    std::stringstream postfix;
    bool last_was_digit = false;

    for (char c : infix) {
        if (isspace(c)) {
            continue; // 공백은 무시
        }

        if (isdigit(c)) {
            if (last_was_digit) { // 이전 문자가 숫자였다면 현재 숫자 이어붙임
                postfix << c;
            }
            else { // 새로운 숫자의 시작
                postfix << " " << c;
            }
            last_was_digit = true;
        }
        else {
            last_was_digit = false;
            if (c == '(') {
                s.push(c);
            }
            else if (c == ')') {
                while (!s.empty() && s.top() != '(') {
                    postfix << " " << s.top();
                    s.pop();
                }
                if (!s.empty()) {
                    s.pop(); // '(' 제거
                }
            }
            else { // 연산자일 경우
                while (!s.empty() && get_precedence(s.top()) >= get_precedence(c)) {
                    postfix << " " << s.top();
                    s.pop();
                }
                s.push(c);
            }
        }
    }

    while (!s.empty()) {
        postfix << " " << s.top();
        s.pop();
    }

    // 맨 앞 공백 제거 후 반환
    std::string result = postfix.str();
    if (!result.empty() && result[0] == ' ') {
        return result.substr(1);
    }
    return result;
}

// 후위 표기식을 계산하는 함수
int evaluate_postfix(const std::string& postfix) {
    std::stack<int> s;
    std::stringstream ss(postfix);
    std::string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) {
            s.push(std::stoi(token));
        }
        else {
            if (s.size() < 2) {
                // 유효하지 않은 후위 표기식 (피연산자 부족)
                // 이 경우 계산을 중단하고 0을 반환하거나 예외 처리를 할 수 있습니다.
                // 여기서는 0을 반환하도록 처리합니다.
                return 0;
            }
            int val2 = s.top();
            s.pop();
            int val1 = s.top();
            s.pop();

            switch (token[0]) {
            case '+': s.push(val1 + val2); break;
            case '-': s.push(val1 - val2); break;
            case '*': s.push(val1 * val2); break;
            case '/': s.push(val1 / val2); break;
            }
        }
    }
    return s.top();
}


int main() {
    std::ifstream input_file("C:\\Users\\민정\\OneDrive - SangMyung University\\문서\\input.txt");
    std::ofstream output_file("C:\\Users\\민정\\OneDrive - SangMyung University\\문서\\output.txt");
    std::string infix_expression;

    if (!input_file.is_open()) {
        std::cerr << "Error: input.txt 파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    if (!output_file.is_open()) {
        std::cerr << "Error: output.txt 파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    while (getline(input_file, infix_expression)) {
        // 유효하지 않은 입력은 건너뛰기
        bool is_valid = true;
        for (char c : infix_expression) {
            if (!isdigit(c) && !isspace(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')') {
                is_valid = false;
                break;
            }
        }
        if (!is_valid) {
            continue;
        }

        std::string postfix_expression = infix_to_postfix(infix_expression);
        int result = evaluate_postfix(postfix_expression);

        // 콘솔 출력
        std::cout << "입력: " << infix_expression << std::endl;
        std::cout << "출력: " << postfix_expression << std::endl;
        std::cout << "계산: " << result << std::endl;
        std::cout << "-----------------------" << std::endl;

        // 파일 출력
        output_file << "입력: " << infix_expression << std::endl;
        output_file << "출력: " << postfix_expression << std::endl;
        output_file << "계산: " << result << std::endl;
        output_file << "-----------------------" << std::endl;
    }

    input_file.close();
    output_file.close();

    return 0;
}
