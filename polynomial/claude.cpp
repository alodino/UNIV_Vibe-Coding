#include 
#include 
#include 
#include 
#include 
#include 

using namespace std;

class Polynomial {
private:
    map terms; // 차수 -> 계수

public:
    Polynomial() {}

    // 문자열로부터 다항식 파싱
    Polynomial(const string& input) {
        parseFromString(input);
    }

    void parseFromString(const string& input) {
        terms.clear();

        if (input.empty() || input == "0") {
            return;
        }

        // x**를 x^로 변환
        string normalized = input;
        regex star_pattern(R"(\*\*)");
        normalized = regex_replace(normalized, star_pattern, "^");

        // 공백 제거
        normalized = regex_replace(normalized, regex(R"(\s+)"), "");

        // 빈 문자열 체크
        if (normalized.empty() || normalized == "0") {
            return;
        }

        // 부호 정규화 (처음에 +가 있으면 제거)
        if (!normalized.empty() && normalized[0] == '+') {
            normalized = normalized.substr(1);
        }

        // + 와 - 앞에 공백 추가하여 항 분리 (첫 번째 문자가 아닌 경우만)
        string result = "";
        for (size_t i = 0; i < normalized.length(); i++) {
            if ((normalized[i] == '+' || normalized[i] == '-') && i > 0) {
                result += " ";
            }
            result += normalized[i];
        }

        // 문자열을 스트림으로 변환하여 항별로 분리
        stringstream ss(result);
        string term;

        while (ss >> term) {
            if (term.empty()) continue;
            parseTerm(term);
        }

        // 계수가 0인 항들 제거
        auto it = terms.begin();
        while (it != terms.end()) {
            if (it->second == 0) {
                it = terms.erase(it);
            } else {
                ++it;
            }
        }
    }

    void parseTerm(const string& term) {
        if (term.empty()) return;

        // x가 없는 상수항인 경우
        if (term.find('x') == string::npos) {
            try {
                int coeff = stoi(term);
                terms[0] += coeff;
            } catch (...) {
                // 파싱 실패시 무시
            }
            return;
        }

        // x가 있는 경우
        int coeff = 1;
        int degree = 1;

        size_t x_pos = term.find('x');

        // 계수 부분 파싱
        if (x_pos > 0) {
            string coeff_str = term.substr(0, x_pos);
            if (coeff_str == "-") {
                coeff = -1;
            } else if (coeff_str == "+") {
                coeff = 1;
            } else {
                try {
                    coeff = stoi(coeff_str);
                } catch (...) {
                    coeff = 1; // 파싱 실패시 기본값
                }
            }
        } else if (!term.empty() && term[0] == '-') {
            coeff = -1;
        }

        // 차수 부분 파싱
        size_t caret_pos = term.find('^');
        if (caret_pos != string::npos) {
            string degree_str = term.substr(caret_pos + 1);
            try {
                degree = stoi(degree_str);
            } catch (...) {
                degree = 1; // 파싱 실패시 기본값
            }
        } else {
            degree = 1; // x만 있는 경우
        }

        terms[degree] += coeff;
    }

    // 덧셈 연산
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;

        // 자신의 모든 항 복사
        for (const auto& term : terms) {
            result.terms[term.first] = term.second;
        }

        // 다른 다항식의 항들 더하기
        for (const auto& term : other.terms) {
            result.terms[term.first] += term.second;
        }

        // 계수가 0인 항 제거
        auto it = result.terms.begin();
        while (it != result.terms.end()) {
            if (it->second == 0) {
                it = result.terms.erase(it);
            } else {
                ++it;
            }
        }

        return result;
    }

    // 곱셈 연산
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;

        for (const auto& term1 : terms) {
            for (const auto& term2 : other.terms) {
                int new_degree = term1.first + term2.first;
                int new_coeff = term1.second * term2.second;
                result.terms[new_degree] += new_coeff;
            }
        }

        // 계수가 0인 항 제거
        auto it = result.terms.begin();
        while (it != result.terms.end()) {
            if (it->second == 0) {
                it = result.terms.erase(it);
            } else {
                ++it;
            }
        }

        return result;
    }

    // 다항식을 문자열로 변환 (출력용)
    string toString() const {
        if (terms.empty()) {
            return "0";
        }

        string result = "";
        bool first = true;

        // 차수 내림차순으로 정렬하기 위해 reverse iterator 사용
        for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
            int degree = it->first;
            int coeff = it->second;

            if (coeff == 0) continue;

            if (!first) {
                if (coeff > 0) {
                    result += " + ";
                } else {
                    result += " - ";
                    coeff = -coeff;
                }
            } else {
                if (coeff < 0) {
                    result += "-";
                    coeff = -coeff;
                }
                first = false;
            }

            // 계수 출력
            if (degree == 0) {
                // 상수항
                result += to_string(coeff);
            } else if (coeff == 1) {
                // 계수가 1인 경우 생략
                if (degree == 1) {
                    result += "x";
                } else {
                    result += "x^" + to_string(degree);
                }
            } else {
                // 일반적인 경우
                if (degree == 1) {
                    result += to_string(coeff) + "x";
                } else {
                    result += to_string(coeff) + "x^" + to_string(degree);
                }
            }
        }

        return result.empty() ? "0" : result;
    }
};

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다." << endl;
        return 1;
    }

    int n;
    file >> n;
    file.ignore(); // 개행 문자 무시

    for (int i = 1; i <= n; i++) {
        string line1, line2;
        getline(file, line1);
        getline(file, line2);

        Polynomial poly1(line1);
        Polynomial poly2(line2);

        Polynomial sum = poly1 + poly2;
        Polynomial product = poly1 * poly2;

        cout << "== " << i << "번째 다항식 쌍 ==" << endl;
        cout << "첫 번째: " << poly1.toString() << endl;
        cout << "두 번째: " << poly2.toString() << endl;
        cout << "덧셈 결과: " << sum.toString() << endl;
        cout << "곱셈 결과: " << product.toString() << endl;
        cout << endl;
    }

    file.close();
    return 0;
}
