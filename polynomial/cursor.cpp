#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
using namespace std;

const int MAX_DEGREE = 1000; // �ִ� ����

// ���׽��� �迭�� ǥ�� (�ε��� = ����, �� = ���)
class Polynomial {
private:
    int coefficients[MAX_DEGREE + 1]; // ��� �迭
    int degree; // �ְ� ����

public:
    // ������ - ��� ����� 0���� �ʱ�ȭ
    Polynomial() {
        memset(coefficients, 0, sizeof(coefficients));
        degree = 0;
    }

    // ���ڿ����� ���׽� �Ľ�
    void parseFromString(const string& str) {
        // �迭 �ʱ�ȭ
        memset(coefficients, 0, sizeof(coefficients));
        degree = 0;

        string s = str;

        // ���� ����
        s.erase(remove(s.begin(), s.end(), ' '), s.end());

        // �� �� �Ľ�
        string currentTerm = "";
        bool isFirstTerm = true;

        for (size_t i = 0; i < s.length(); i++) {
            char c = s[i];

            // ��ȣ ó�� (ù ��° ���� �ƴ� ���)
            if ((c == '+' || c == '-') && !isFirstTerm) {
                if (!currentTerm.empty()) {
                    parseTerm(currentTerm);
                    currentTerm = "";
                }
            }

            currentTerm += c;
            isFirstTerm = false;
        }

        // ������ �� ó��
        if (!currentTerm.empty()) {
            parseTerm(currentTerm);
        }

        // �ְ� ���� ã��
        updateDegree();
    }

    // ���� �� �Ľ�
    void parseTerm(const string& termStr) {
        int coeff = 1;
        int exp = 0;
        bool hasX = false;

        size_t i = 0;

        // ��ȣ ó��
        if (termStr[i] == '-') {
            coeff = -1;
            i++;
        }
        else if (termStr[i] == '+') {
            i++;
        }

        // ��� �Ľ�
        string coeffStr = "";
        while (i < termStr.length() && isdigit(termStr[i])) {
            coeffStr += termStr[i];
            i++;
        }

        if (!coeffStr.empty()) {
            coeff *= stoi(coeffStr);
        }

        // x Ȯ��
        if (i < termStr.length() && termStr[i] == 'x') {
            hasX = true;
            exp = 1;
            i++;
        }

        // ���� �Ľ�
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

        // ����� ó��
        if (!hasX && coeffStr.empty() && termStr.find('x') == string::npos) {
            exp = 0;
        }

        // �迭�� ��� ����
        if (exp <= MAX_DEGREE) {
            coefficients[exp] += coeff;
        }
    }

    // �ְ� ���� ������Ʈ
    void updateDegree() {
        degree = 0;
        for (int i = MAX_DEGREE; i >= 0; i--) {
            if (coefficients[i] != 0) {
                degree = i;
                break;
            }
        }
    }

    // ���׽� ���
    void print() const {
        bool firstTerm = true;
        bool allZero = true;

        // ��� ����� 0���� Ȯ��
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

        // ���� �������� ���
        for (int i = degree; i >= 0; i--) {
            if (coefficients[i] == 0) continue;

            // ù ��° ���� �ƴ� ��� ��ȣ ���
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

            // ��� ��� (1�� ��� ����, �� ������� ���)
            int absCoeff = abs(coefficients[i]);
            if (absCoeff != 1 || i == 0) {
                cout << absCoeff;
            }

            // ������ ���� ���
            if (i > 0) {
                cout << "x";
                if (i > 1) {
                    cout << "^" << i;
                }
            }

            firstTerm = false;
        }
    }

    // ���׽� ����
    Polynomial add(const Polynomial& other) const {
        Polynomial result;

        for (int i = 0; i <= MAX_DEGREE; i++) {
            result.coefficients[i] = this->coefficients[i] + other.coefficients[i];
        }

        result.updateDegree();
        return result;
    }

    // ���׽� ����
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

// ���׽� ���� �Լ�
Polynomial add_poly(const Polynomial& poly1, const Polynomial& poly2) {
    return poly1.add(poly2);
}

// ���׽� ���� �Լ�
Polynomial multiply_poly(const Polynomial& poly1, const Polynomial& poly2) {
    return poly1.multiply(poly2);
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "input.txt ������ �� �� �����ϴ�." << endl;
        return 1;
    }

    string line;
    int n = 0;

    // ù ��° �ٿ��� ���� ���� �б�
    if (getline(file, line)) {
        // ���� ����
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());

        n = stoi(line);
    }

    if (n <= 0 || n > 100) {
        cout << "���� ������ ��ȿ���� �ʽ��ϴ�. (n = " << n << ")" << endl;
        file.close();
        return 1;
    }

    cout << "ó���� ���׽� ���� ����: " << n << endl << endl;

    for (int pair = 1; pair <= n; pair++) {
        cout << "== " << pair << "��° �� ==" << endl;

        // ù ��° ���׽� �б�
        if (getline(file, line)) {
            Polynomial poly1;
            poly1.parseFromString(line);
            cout << "ù ��°: ";
            poly1.print();

            // �� ��° ���׽� �б�
            if (getline(file, line)) {
                Polynomial poly2;
                poly2.parseFromString(line);
                cout << "�� ��°: ";
                poly2.print();

                // ����
                Polynomial sum = add_poly(poly1, poly2);
                cout << "���� ���: ";
                sum.print();

                // ����
                Polynomial product = multiply_poly(poly1, poly2);
                cout << "���� ���: ";
                product.print();
                cout << endl;
            }
        }
    }

    file.close();
    return 0;
}