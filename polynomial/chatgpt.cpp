#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <vector>

using Poly = std::map<int, int, std::greater<int>>;                 // map variable key = coefficient, value = exponent

// parse each string into coefficient/exponent
std::pair<int, int> Parse(const std::string& term) {
    std::string s = term;
    s.erase(remove(s.begin(), s.end(), ' '), s.end());              // strip whitespaces

    if (s.empty()) return { 0, 0 };                                 // if it is empty return with key/value 0

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

        size_t exponent = s.find("**", xPos);                       // parse exponent by symbol "**"
        if (exponent == std::string::npos)
            exponent = s.find("^", xPos);                           // or symbol "^"

        if (exponent != std::string::npos)                          // if it is parsed properly
            exp = stoi(s.substr(exponent+(s[exponent] == '*' ? 2 : 1)));
        else
            exp = 1;                                                // if it has no exponent symbol

        if (exp > 100) {
            std::cerr << "Error: Exponent exceeds 100 in term: \"" << term << "\"" << std::endl;
            exit(1);
        }
    }

    return {exp, coef};  // exponent as key, coefficient as value
}

// Handling raw string to proper polynomial structure
Poly Refine(const std::string& line) {
    Poly poly;
    // divide to 3 category -> coef/x/exp || coef/x || coef
    std::regex termPattern(R"(([+-]?\s*\d*x(?:\*\*|\^)?\d+|[+-]?\s*\d*x|[+-]?\s*\d+))");
    auto begin = std::sregex_iterator(line.begin(), line.end(), termPattern);
    auto end = std::sregex_iterator();

    for (auto& it = begin; it != end; ++it) {
        std::string currentTerm = it->str(1);
        auto [exp, coef] = Parse(currentTerm);
        poly[exp] += coef;
    }
    return poly;
}

// Adds two polynomials
Poly Add(const Poly& him, const Poly& her) {
    Poly returnPoly = him;

    // adds coefficient corresponding exponent term 
    for (const auto& [exp, coef] : her) {
        returnPoly[exp] += coef;
    }
    return returnPoly;
}

// Times two polynomials
Poly Multiply(const Poly& him, const Poly& her) {
    Poly returnPoly;

    // calculate every combination of terms
    for (const auto& [hisExp, hisCoef] : him) {
        for (const auto& [herExp, herCoef] : her) {
            int theirExp = hisExp + herExp;
            int theirCoef = hisCoef * herCoef;
            returnPoly[theirExp] += theirCoef;
        }
    }
    return returnPoly;
}

// Prints polynomials with full form
std::string Print(const Poly& poly) {
    std::ostringstream printStream;
    bool isFirst = true;                                    // initial indicator
    for (const auto& [exp, coef] : poly) {
        if (coef == 0) continue;

        if (!isFirst) {                                     // after initial term
            printStream << (coef > 0 ? " + " : " - ");      // denote operator symbol +/-
        }
        else {
            if (coef < 0) printStream << "-";               // initial term
            isFirst = false;                                // turn off initial indicator
        }

        int absCoef = abs(coef);                            // normalize coefficient since it already denote operator
        if (exp == 0) printStream << absCoef;               // if exponent is 0 -> constant term
        else {
            if (absCoef != 1) printStream << absCoef;       // print coefficient except it is 1
            printStream << "x";
            if (exp != 1) printStream << "^" << exp;        // print exponent except it is 1
        }
    }
    if (isFirst) return "0";                                // if every component is 0, print out 0
    return printStream.str();
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to Open input.txt" << std::endl;
        return 1;
    }

    // insert pair count
    int pair;
    file >> pair;
    file.ignore();

    std::vector<Poly> equations;
    
    for (int currentPair = 1; currentPair <= pair; ++currentPair) {
        for (int i = 0; i < 2; ++i) {
            std::string line;
            std::getline(file, line);
            equations.push_back(Refine(line));
        }
    }

    for (int i = 0; i < pair*2; i+=2) {
        std::cout << "== " << (i/2)+1 << "번째 다항식 쌍 ==" << std::endl;
        Poly Addresult = Add(equations[i], equations[i+1]);
        Poly Multiplyresult = Multiply(equations[i], equations[i+1]);
        std::cout << "첫번째 다항식\t" << Print(equations[i]) << "\n"
                  << "두번째 다항식\t" << Print(equations[i+1]) << std::endl;
        std::cout << "덧셈 결과\t" << Print(Addresult) << "\n"
                  << "곱셈 결과\t" << Print(Multiplyresult) << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
