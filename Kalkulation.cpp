#include <iostream>
#include <string>
#include <cctype>
#include <locale>
#include <stdexcept>

double expression(const char*& expressionToParse);

// ���������� ������� ������ � ���������, �� ������� ���������
char peek(const char* expressionToParse) {
    return *expressionToParse;
}

// ���������� ������� ������ � ���������� ��������� �����
char get(const char*& expressionToParse) {
    return *expressionToParse++;
}

// ������������ ������, ���������� ���������� � �������� ����������
void handleError(const std::string& message) {
    throw std::runtime_error("������: " + message);
}

// ������ ����� �� ��������� � ���������� ���������
double number(const char*& expressionToParse) {
    // ������������ std::stod ��� ����������� ��������� � ����� � ��������� ������
    double result = std::stod(std::string(expressionToParse));

    // ���������� ��������� �� ��� ���, ���� ������� �������� ������� ��� ������
    while (std::isdigit(peek(expressionToParse)) || peek(expressionToParse) == '.') {
        get(expressionToParse);
    }
    return result;
}

// ������������ ������ � ��������� (�����, ������� ����� ��� ��������� � �������)
double factor(const char*& expressionToParse) throw(std::runtime_error) {
    // ���� ������� ������ - �����, ������������ ������� �����
    if (peek(expressionToParse) == '-') {
        get(expressionToParse);
        return -number(expressionToParse);
    }

    // ���� ������� ������ - ����, ������������ ��������� "������ �������� �����!"
    else if (peek(expressionToParse) == '+' && peek(expressionToParse++) == '+') {
        handleError("������ �������� �����!");
        return 0;
    }

    // ���� ������� ������ - �����, ������������ �����
    else if (std::isdigit(peek(expressionToParse))) {
        return number(expressionToParse);
    }

    // ���� ������� ������ - ����������� ������, ������������ ��������� � �������
    else if (peek(expressionToParse) == '(') {
        get(expressionToParse);
        double result = expression(expressionToParse);

        // ��������� ������� ����������� ������
        if (peek(expressionToParse) != ')') {
            handleError("����������� ����������� ������");
            return 0;
        }

        get(expressionToParse); // ���������� ��������� �� ������ ����� ����������� ������
        return result;
    }

    throw std::runtime_error("�������� ������ ���������");
}

// ������������ ���� � ��������� (��������� � �������)
double term(const char*& expressionToParse) {
    // ������������ ������ ������
    double result = factor(expressionToParse);

    // ���������� ���������, ���� ���� ��������� ��� �������
    while (peek(expressionToParse) == '*' || peek(expressionToParse) == '/') {
        char op = get(expressionToParse);
        double nextFactor = factor(expressionToParse);

        // ��������� ��������� ��� ������� � ����������� �� ���������
        if (op == '*') {
            result *= nextFactor;
        }
        else if (nextFactor != 0) {
            result /= nextFactor;
        }
        else {
            handleError("������� �� ����");
            return 0;
        }
    }
    return result;
}

// ������������ ��������� (�������� � ���������)
double expression(const char*& expressionToParse) {
    // ������������ ������ ����
    double result = term(expressionToParse);

    // ���������� ���������, ���� ���� �������� ��� ���������
    while (peek(expressionToParse) == '+' || peek(expressionToParse) == '-') {
        char op = get(expressionToParse);
        double nextTerm = term(expressionToParse);

        
        // ��������� �������� ��� ��������� � ����������� �� ���������
        if (op == '+') {
            result += nextTerm;
        }
        else {
            result -= nextTerm;
        }
    }
    return result;
}

// ������� ������� �� ������
void removeSpaces(std::string& str) {
    std::locale loc("");
    // ���������� erase � remove_if ��� �������� �������� �� ������
    str.erase(std::remove_if(str.begin(), str.end(), [&loc](unsigned char c) {
        return std::isspace(c, loc);
        }), str.end());
}

// ���������, �������� �� ������ ���������� � ���������
bool isValidSymbol(char c) {
    return std::isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '.';
}

// ���������, �������� �� ������ ������ ���������� �������
bool isValidExpression(const std::string& str) {
    // ��������� ������ ������ � ������
    for (char c : str) {
        if (!isValidSymbol(c)) {
            return false;
        }
    }
    return true;
}

// �������� ���� ��������� �� ������������
std::string getInputExpression() {
    std::cout << ">_ ";
    std::string userInput;
    // �������� ������ ����� �� ������������ � ������� std::getline
    std::getline(std::cin, userInput);
    return userInput;
}

// �������� ������� ���������
int main() {
    // ������������� ��������� ��� ����������� ����������� ������������� �������� � �������
    system("chcp 1251");

    std::cout << "������� �������������� ���������:\n";

    // �������� ���� ���������
    while (true) {
        // �������� ���� ��������� �� ������������
        std::string userInput = getInputExpression();

        // ���������, ���� ������� ������ ������, ��������� ���������
        if (userInput.empty()) {
            std::cout << "�� �������!" << std::endl;
            break;
        }

        // ������� ������� �� �������� ������
        removeSpaces(userInput);

        // ���������, �������� �� ������ ������ ���������� �������
        if (!isValidExpression(userInput)) {
            std::cerr << "������: ������� ������������ �������. ����������, ������� ��������� ������." << std::endl;
            continue;
        }

        // ����������� ������ � ������ �������� ��� ���������
        const char* expressionToParse = userInput.c_str();

        try {
            // ��������� ��������� ���������
            double result = expression(expressionToParse);

            // ��������� �� ������������
            if (std::isinf(result)) {
                std::cout << "������������" << std::endl;
            }
            else {
                std::cout << "���������: " << result << std::endl;
            }
        }
        // ������������ ����������, ����������� ��� ������� � ���������
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            continue;
        }
    }

    return 0;
}
