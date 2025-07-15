#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <regex>

using namespace std;

map<int, int> parsePolynomial(const string& poly_str) {
    map<int, int> poly;
    string normalized_str = poly_str;

    normalized_str.erase(remove(normalized_str.begin(), normalized_str.end(), ' '), normalized_str.end());

    if (normalized_str[0] != '+' && normalized_str[0] != '-') {
        normalized_str = "+" + normalized_str;
    }

    regex term_regex("[+-]?[0-9]*(x([\\^\\*][\\*]?[0-9]+)?)?");
    auto terms_begin = sregex_iterator(normalized_str.begin(), normalized_str.end(), term_regex);
    auto terms_end = sregex_iterator();

    for (sregex_iterator i = terms_begin; i != terms_end; ++i) {
        smatch match = *i;
        string term_str = match.str();
        if (term_str.empty()) continue;

        int coeff;
        int power;

        size_t x_pos = term_str.find('x');

        if (x_pos == string::npos) {
            power = 0;
            coeff = stoi(term_str);
        }
        else { 
            string coeff_str = term_str.substr(0, x_pos);
            if (coeff_str == "+" || coeff_str.empty()) {
                coeff = 1;
            }
            else if (coeff_str == "-") {
                coeff = -1;
            }
            else {
                coeff = stoi(coeff_str);
            }
            size_t power_pos = term_str.find_first_of("^**", x_pos);
            if (power_pos == string::npos) { // x^1
                power = 1;
            }
            else {
                string power_str = term_str.substr(power_pos + (term_str[power_pos] == '*' ? 2 : 1));
                power = stoi(power_str);
            }
        }
        poly[power] += coeff;
    }

    for (auto it = poly.begin(); it != poly.end(); ) {
        if (it->second == 0) {
            it = poly.erase(it);
        }
        else {
            ++it;
        }
    }

    return poly;
}

map<int, int> addPolynomials(const map<int, int>& poly1, const map<int, int>& poly2) {
    map<int, int> result = poly1;
    for (const auto& pair : poly2) {
        result[pair.first] += pair.second;
    }
    for (auto it = result.begin(); it != result.end(); ) {
        if (it->second == 0) {
            it = result.erase(it);
        }
        else {
            ++it;
        }
    }
    return result;
}

map<int, int> multiplyPolynomials(const map<int, int>& poly1, const map<int, int>& poly2) {
    map<int, int> result;
    for (const auto& pair1 : poly1) {
        for (const auto& pair2 : poly2) {
            int new_power = pair1.first + pair2.first;
            int new_coeff = pair1.second * pair2.second;
            result[new_power] += new_coeff;
        }
    }
    for (auto it = result.begin(); it != result.end(); ) {
        if (it->second == 0) {
            it = result.erase(it);
        }
        else {
            ++it;
        }
    }
    return result;
}

string formatPolynomial(const map<int, int>& poly_dict) {
    if (poly_dict.empty()) {
        return "0";
    }

    string result = "";
    bool is_first = true;

    for (auto it = poly_dict.rbegin(); it != poly_dict.rend(); ++it) {
        int power = it->first;
        int coeff = it->second;

        if (is_first) {
            if (coeff < 0) {
                result += "-";
            }
        }
        else {
            if (coeff > 0) {
                result += " + ";
            }
            else {
                result += " - ";
            }
        }

        if (abs(coeff) != 1 || power == 0) {
            result += to_string(abs(coeff));
        }

        if (power > 0) {
            result += "x";
            if (power > 1) {
                result += "^" + to_string(power);
            }
        }

        is_first = false;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ifstream inputFile("C:\\Users\\민정\\OneDrive - SangMyung University\\문서\\input.txt");
    if (!inputFile.is_open()) {
        cerr << "오류: input.txt 파일을 열 수 없습니다." << endl;
        return 1;
    }

    int num_pairs;
    inputFile >> num_pairs;
    inputFile.ignore();

    vector<pair<string, string>> polynomial_pairs;
    for (int i = 0; i < num_pairs; ++i) {
        string poly1_str, poly2_str;
        getline(inputFile, poly1_str);
        getline(inputFile, poly2_str);
        polynomial_pairs.push_back({ poly1_str, poly2_str });
    }

    cout << "총 " << num_pairs << "쌍의 다항식이 있습니다." << endl;

    int choice;
    while (true) {
        cout << "계산할 다항식 쌍의 번호를 입력하세요 (1부터 " << num_pairs << "): ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > num_pairs) {
            cout << "잘못된 번호입니다. 1부터 " << num_pairs << " 사이의 숫자를 입력하세요." << endl;
            cin.clear();
            cin.ignore(100, '\n');
        }
        else {
            break;
        }
    }

    const auto& selected_pair = polynomial_pairs[choice - 1];
    string poly1_str = selected_pair.first;
    string poly2_str = selected_pair.second;

    map<int, int> poly1 = parsePolynomial(poly1_str);
    map<int, int> poly2 = parsePolynomial(poly2_str);

    map<int, int> sum_poly = addPolynomials(poly1, poly2);
    map<int, int> prod_poly = multiplyPolynomials(poly1, poly2);

    ofstream outputFile("C:\\Users\\민정\\OneDrive - SangMyung University\\문서\\output.txt");
    if (!outputFile.is_open()) {
        cerr << "오류: output.txt 파일을 생성할 수 없습니다." << endl;
        return 1;
    }

    outputFile << "=== 결과 ===" << endl;
    outputFile << "선택한 다항식 쌍: (" << poly1_str << ") \n (" << poly2_str << ")" << endl;
    outputFile << "합: " << formatPolynomial(sum_poly) << endl;
    outputFile << "곱: " << formatPolynomial(prod_poly) << endl;

    cout << "\n계산 결과가 output.txt 파일에 성공적으로 저장되었습니다." << endl;

    return 0;
}
