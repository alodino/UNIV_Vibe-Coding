#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <memory>
#include <algorithm>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

std::vector<std::string> runPythonCrawler(const std::string& mode, const std::string& apiKey, const std::string& query) {
    std::vector<std::string> result;
    std::string command = "python web.py " + mode + " \"" + apiKey + "\" \"" + query + "\"";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run Python script.\n";
        return result;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string line(buffer);
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        result.push_back(line);
    }

    pclose(pipe);
    return result;
}

int main() {
    std::string apiKey;
    std::cout << "Enter your TMDB API Key: ";
    std::getline(std::cin, apiKey);

    while (true) {
        std::cout << "\nChoose mode:\n1. Actor ¡æ Movies\n2. Movie ¡æ Actors\n3. Exit\n> ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // flush newline

        if (choice == 3) break;

        std::string inputName;
        std::cout << "Enter name (English): ";
        std::getline(std::cin, inputName);

        std::string mode = (choice == 1) ? "actor" : "movie";
        std::vector<std::string> result = runPythonCrawler(mode, apiKey, inputName);

        if (result.empty()) {
            std::cout << "No result found.\n";
        }
        else {
            std::cout << "\n=== Result ===\n";
            for (const auto& item : result)
                std::cout << item << "\n";
        }
    }

    return 0;
}
