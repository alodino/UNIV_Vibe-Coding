#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAX_DEGREE = 1000; // 최대 차수

// 다항식을 배열로 표현 (인덱스 = 차수, 값 = 계수)
class Polynomial {
private:
    int coefficients[MAX_DEGREE + 1]; // 계수 배열
    int degree; // 최고 차수

public:
    // 생성자 - 모든 계수를 0으로 초기화
    Polynomial() {
        memset(coefficients, 0, sizeof(coefficients));
        degree = 0;
    }

    // 문자열에서 다항식 파싱
    void parseFromString(const string& str) {
        // 배열 초기화
        memset(coefficients, 0, sizeof(coefficients));
        degree = 0;

        string s = str;

        // 공백 제거
        s.erase(remove(s.begin(), s.end(), ' '), s.end());

        // 각 항 파싱
        string currentTerm = "";
        bool isFirstTerm = true;

        for (size_t i = 0; i < s.length(); i++) {
            char c = s[i];

            // 부호 처리 (첫 번째 항이 아닌 경우)
            if ((c == '+' || c == '-') && !isFirstTerm) {
                if (!currentTerm.empty()) {
                    parseTerm(currentTerm);
                    currentTerm = "";
                }
            }

            currentTerm += c;
            isFirstTerm = false;
        }

        // 마지막 항 처리
        if (!currentTerm.empty()) {
            parseTerm(currentTerm);
        }

        // 최고 차수 찾기
        updateDegree();
    }

    // 개별 항 파싱
    void parseTerm(const string& termStr) {
        int coeff = 1;
        int exp = 0;
        bool hasX = false;

        size_t i = 0;

        // 부호 처리
        if (termStr[i] == '-') {
            coeff = -1;
            i++;
        }
        else if (termStr[i] == '+') {
            i++;
        }

        // 계수 파싱
        string coeffStr = "";
        while (i < termStr.length() && isdigit(termStr[i])) {
            coeffStr += termStr[i];
            i++;
        }

        if (!coeffStr.empty()) {
            coeff *= stoi(coeffStr);
        }

        // x 확인
        if (i < termStr.length() && termStr[i] == 'x') {
            hasX = true;
            exp = 1;
            i++;
        }

        // 지수 파싱
        if (i < termStr.length() && termStr[i] == '^') {
            i++;
            string expStr = "";
            while (i < termStr.length() && isdigit(termStr[i])) {
                expStr += termStr[i];
                i++;
            }
            if (!expStr.empty()) {
                exp = stoi(expStr);
            }
        }

        // 상수항 처리
        if (!hasX && coeffStr.empty() && termStr.find('x') == string::npos) {
            exp = 0;
        }

        // 배열에 계수 저장
        if (exp <= MAX_DEGREE) {
            coefficients[exp] += coeff;
        }
    }

    // 최고 차수 업데이트
    void updateDegree() {
        degree = 0;
        for (int i = MAX_DEGREE; i >= 0; i--) {
            if (coefficients[i] != 0) {
                degree = i;
                break;
            }
        }
    }

    // 다항식 출력
    void print() const {
        bool firstTerm = true;
        bool allZero = true;

        // 모든 계수가 0인지 확인
        for (int i = 0; i <= MAX_DEGREE; i++) {
            if (coefficients[i] != 0) {
                allZero = false;
                break;
            }
        }

        if (allZero) {
            cout << "0";
            return;
        }

        // 높은 차수부터 출력
        for (int i = degree; i >= 0; i--) {
            if (coefficients[i] == 0) continue;

            // 첫 번째 항이 아닌 경우 부호 출력
            if (!firstTerm) {
                if (coefficients[i] > 0) {
                    cout << " + ";
                }
                else {
                    cout << " - ";
                }
            }
            else if (coefficients[i] < 0) {
                cout << "-";
            }

            // 계수 출력 (1인 경우 생략, 단 상수항은 출력)
            int absCoeff = abs(coefficients[i]);
            if (absCoeff != 1 || i == 0) {
                cout << absCoeff;
            }

            // 변수와 지수 출력
            if (i > 0) {
                cout << "x";
                if (i > 1) {
                    cout << "^" << i;
                }
            }

            firstTerm = false;
        }
    }

    // 다항식 덧셈
    Polynomial add(const Polynomial& other) const {
        Polynomial result;

        for (int i = 0; i <= MAX_DEGREE; i++) {
            result.coefficients[i] = this->coefficients[i] + other.coefficients[i];
        }

        result.updateDegree();
        return result;
    }

    // 다항식 곱셈
    Polynomial multiply(const Polynomial& other) const {
        Polynomial result;

        for (int i = 0; i <= this->degree; i++) {
            for (int j = 0; j <= other.degree; j++) {
                int newExp = i + j;
                if (newExp <= MAX_DEGREE) {
                    result.coefficients[newExp] += this->coefficients[i] * other.coefficients[j];
                }
            }
        }

        result.updateDegree();
        return result;
    }
};

// 다항식 덧셈 함수
Polynomial add_poly(const Polynomial& poly1, const Polynomial& poly2) {
    return poly1.add(poly2);
}

// 다항식 곱셈 함수
Polynomial multiply_poly(const Polynomial& poly1, const Polynomial& poly2) {
    return poly1.multiply(poly2);
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "input.txt 파일을 열 수 없습니다." << endl;
        return 1;
    }

    string line;
    int n = 0;

    // 첫 번째 줄에서 쌍의 개수 읽기
    if (getline(file, line)) {
        // 공백 제거
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        n = stoi(line);
    }

    if (n <= 0 || n > 100) {
        cout << "쌍의 개수가 유효하지 않습니다. (n = " << n << ")" << endl;
        file.close();
        return 1;
    }

    cout << "처리할 다항식 쌍의 개수: " << n << endl << endl;

    for (int pair = 1; pair <= n; pair++) {
        cout << "== " << pair << "번째 쌍 ==" << endl;

        // 첫 번째 다항식 읽기
        if (getline(file, line)) {
            Polynomial poly1;
            poly1.parseFromString(line);
            cout << "첫 번째: ";
            poly1.print();

            // 두 번째 다항식 읽기
            if (getline(file, line)) {
                Polynomial poly2;
                poly2.parseFromString(line);
                cout << "두 번째: ";
                poly2.print();

                // 덧셈
                Polynomial sum = add_poly(poly1, poly2);
                cout << "덧셈 결과: ";
                sum.print();

                // 곱셈
                Polynomial product = multiply_poly(poly1, poly2);
                cout << "곱셈 결과: ";
                product.print();
                cout << endl;
            }
        }
    }

    file.close();
    return 0;
}