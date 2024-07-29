#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <map>

// Define the version of the program
const std::string VERSION = "0.0.1Alpha";

// Function to evaluate mathematical expressions
double evaluateExpression(const std::string& expr) {
    std::istringstream iss(expr);
    double result = 0.0;
    char op;
    iss >> result;
    while (iss >> op) {
        double value;
        iss >> value;
        switch (op) {
        case '+': result += value; break;
        case '-': result -= value; break;
        case '*': result *= value; break;
        case '/': result /= value; break;
        }
    }
    return result;
}

// Function to convert time to a 128-bit hexadecimal string
std::string timeToHex(std::chrono::duration<double> duration) {
    uint64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::stringstream ss;
    ss << std::hex << std::setw(32) << std::setfill('0') << microseconds;
    return ss.str();
}

// Function to convert time to a normal time format (HH:MM:SS)
std::string timeToString(std::chrono::duration<double> duration) {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << secs;
    return ss.str();
}

// Function to save logs
void saveLog(const std::string& logContent) {
    std::string path;
#ifdef _WIN32
    path = "C:\\ACE\\logs\\log.txt";
#else
    path = "/home/ACE/logs/log.txt";
#endif

    try {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
        std::ofstream logFile(path, std::ios::app);
        if (logFile.is_open()) {
            logFile << logContent << std::endl;
            logFile.close();
        }
        else {
            std::cerr << "Failed to open log file for writing." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception while saving log: " << e.what() << std::endl;
    }
}

// Load language mappings
std::map<std::string, std::string> loadLanguage(const std::string& languageCode) {
    std::map<std::string, std::string> languageMap;
    if (languageCode == "zh-CN-Hans") {
        languageMap["play"] = "播放";
        languageMap["stop"] = "停止";
        languageMap["Invalid input"] = "无效输入";
        languageMap["Result"] = "结果";
        languageMap["Logs saved."] = "日志已保存。";
        languageMap["Elapsed time (hex): "] = "经过的时间（十六进制）：";
        languageMap["Elapsed time (normal): "] = "经过的时间（正常）：";
        languageMap["help"] = "帮助";
        languageMap["Usage:"] = "使用方法:";
        languageMap["play <expression>"] = "播放 <表达式>";
        languageMap["stop"] = "停止";
        languageMap["ues <languageCode>"] = "设置语言 <语言代码>";
        languageMap["version"] = "版本";
    }
    else {
        // Default to English
        languageMap["play"] = "play";
        languageMap["stop"] = "stop";
        languageMap["Invalid input"] = "Invalid input";
        languageMap["Result"] = "Result";
        languageMap["Logs saved."] = "Logs saved.";
        languageMap["Elapsed time (hex): "] = "Elapsed time (hex): ";
        languageMap["Elapsed time (normal): "] = "Elapsed time (normal): ";
        languageMap["help"] = "help";
        languageMap["Usage:"] = "Usage:";
        languageMap["play <expression>"] = "play <expression>";
        languageMap["stop"] = "stop";
        languageMap["ues <languageCode>"] = "set language <languageCode>";
        languageMap["version"] = "version";
    }
    return languageMap;
}

// Function to display help information
void showHelp(const std::map<std::string, std::string>& languageMap) {
    std::cout << languageMap.at("Usage:") << std::endl;
    std::cout << "  " << languageMap.at("play") << " <expression> - " << languageMap.at("play <expression>") << std::endl;
    std::cout << "  " << languageMap.at("stop") << " - " << languageMap.at("stop") << std::endl;
    std::cout << "  " << languageMap.at("ues") << " <languageCode> - " << languageMap.at("ues <languageCode>") << std::endl;
    std::cout << "  " << languageMap.at("version") << " - Display version information" << std::endl;
}

// Function to parse and process the input command
void processCommand(const std::string& input, const std::chrono::steady_clock::time_point& startTime, const std::map<std::string, std::string>& languageMap) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    // Get elapsed time
    auto now = std::chrono::steady_clock::now();
    auto duration = now - startTime;

    if (command == languageMap.at("play")) {
        std::string argument;
        std::getline(iss, argument);
        argument = argument.substr(argument.find_first_not_of(" \t"));
        argument = argument.substr(0, argument.find_last_not_of(" \t") + 1);

        if (argument.size() >= 2 && argument.front() == '"' && argument.back() == '"') {
            std::string content = argument.substr(1, argument.size() - 2);
            std::cout << content << std::endl;
        }
        else {
            try {
                double result = evaluateExpression(argument);
                std::cout << languageMap.at("Result") << ": " << result << std::endl;
            }
            catch (...) {
                std::cout << languageMap.at("Invalid input") << ": " << input << std::endl;
            }
        }
    }
    else if (command == languageMap.at("stop")) {
        std::string logContent = languageMap.at("Elapsed time (hex): ") + timeToHex(duration) + "\n" +
            languageMap.at("Elapsed time (normal): ") + timeToString(duration);
        saveLog(logContent);
        std::cout << languageMap.at("Logs saved.") << std::endl;
    }
    else if (command == languageMap.at("help")) {
        showHelp(languageMap);
    }
    else if (command == languageMap.at("version")) {
        std::cout << "Current version: " << VERSION << std::endl;
    }
    else {
        std::cout << languageMap.at("Invalid input") << ": " << input << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-v") {
            std::cout << "Current version: " << VERSION << std::endl;
            return 0;
        }
    }

    auto startTime = std::chrono::steady_clock::now();
    std::string input;
    std::string languageCode = "en"; // Default language

    while (true) {
        std::cout << "ACE Compiler Interactive Console" << std::endl;
        std::cout << "Enter a command, file path, or expression:" << std::endl;
        std::cout << "> ";
        std::getline(std::cin, input);

        // Check if the input is for setting the language
        if (input.find("ues ") == 0) {
            languageCode = input.substr(4);
            std::cout << "Language set to " << languageCode << std::endl;
            continue;
        }

        // Load language mappings based on the selected language code
        auto languageMap = loadLanguage(languageCode);

        if (input == "exit") {
            break;
        }

        processCommand(input, startTime, languageMap);
    }
    return 0;
}
