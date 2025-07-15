#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <vector>

using Poly = std::map<int, int, std::greater<int>>; // map variable key = coefficient, value = exponenet

std::pair<int, int> Parse(const std::string& term) {
    std::string s = term;
    s.erase(remove(s.begin(), s.end(), ' '), s.end()); // strip spaces

    if (s.empty()) return { 0, 0 };

    int coef = 0, exp = 0;
    size_t xPos = s.find('x');

    // if there is no 'x' -> constant term
    if (xPos == std::string::npos) {
        coef = stoi(s);
        exp = 0;
    }

    else {
        // parse coefficient
        std::string coefficient = s.substr(0, xPos);
        
        if (coefficient.empty() || coefficient == "+") coef = +1;   // if coefficient does not have numeric part and positive -> +1
        else if (coefficient == "-")                   coef = -1;   // or ... and negative -> -1
        else coef = std::stoi(coefficient);                         // else normally parse

        size_t exponent = s.find("**", xPos);                       // parse exponent by "**"
        if (exponent == std::string::npos)
            exponent = s.find("^", xPos);                           // or "^"

        if (exponent != std::string::npos)                          // if it is parsed properly
            exp = stoi(s.substr(exponent+(s[exponent] == '*' ? 2 : 1)));
        else
            exp = 1;
    }

    return {exp, coef};  // exponent as key, coefficient as value
}

// handling raw string to proper polynomial structure
Poly parsePolynomial(const std::string& line) {
    Poly poly;
    // divide to 3 category -> coef/x/exp || coef/x || coef
    std::regex termPattern(R"(([+-]?\s*\d*x(?:\*\*|\^)?\d+|[+-]?\s*\d*x|[+-]?\s*\d+))");
    auto begin = std::sregex_iterator(line.begin(), line.end(), termPattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::string inputTerm = it->str(1);
        auto [exp, coef] = Parse(inputTerm);
        poly[exp] += coef;
    }
    return poly;
}

// Adds two polynomials
Poly Add(const Poly& a, const Poly& b) {
    Poly result = a;
    for (const auto& [exp, coef] : b) {
        result[exp] += coef;
    }
    return result;
}

Poly Multiply(const Poly& a, const Poly& b) {
    Poly result;
    for (const auto& [expA, coefA] : a) {
        for (const auto& [expB, coefB] : b) {
            int newExp = expA + expB;
            int newCoef = coefA * coefB;
            result[newExp] += newCoef;
        }
    }
    return result;
}

// Outputs polynomial
std::string Print(const Poly& poly) {
    std::ostringstream oss;
    bool first = true;
    for (const auto& [exp, coef] : poly) {
        if (coef == 0) continue;

        if (!first) {
            oss << (coef > 0 ? " + " : " - ");
        }
        else {
            if (coef < 0) oss << "-";
            first = false;
        }

        int absCoef = abs(coef);
        if (exp == 0) oss << absCoef;
        else {
            if (absCoef != 1) oss << absCoef;
            oss << "x";
            if (exp != 1) oss << "**" << exp;
        }
    }
    if (first) return "0"; // all zero
    return oss.str();
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
        return 1;
    }

    // insert pair count
    int pair;
    file >> pair;
    file.ignore();

    std::vector<Poly> equations;
    for (int i = 0; i < pair*2; ++i) {
        std::string line;
        std::getline(file, line);
        equations.push_back(parsePolynomial(line));
    }

    // debugging
    int count = 1;
    for (auto& equation : equations) {
        std::cout << "equation #" << count << " " << std::flush;
        for (auto& [exp, coef] : equation)
            std::cout << coef << " x ^ " << exp << " " << std::flush;
        std::cout << "\n" << std::endl;
        count++;
    }

    std::cout << "\nPairwise Addition Results:\n";

    for (int i = 0; i < pair*2; i+=2) {
        std::cout << "== " << i/2 << "번째 다항식 쌍 ==" << std::endl;
        Poly Addresult = Add(equations[i], equations[i+1]);
        Poly Multiplyresult = Multiply(equations[i], equations[i + 1]);
        std::cout << "Equation " << i+1 << " + Equation " << i+2 << " = " << Print(Addresult) << "\n"
                  << "Equation " << i+1 << " * Equation " << i+2 << " = " << Print(Multiplyresult) << std::endl;
    }

    return 0;
}
