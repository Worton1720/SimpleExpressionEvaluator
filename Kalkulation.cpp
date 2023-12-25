#include <iostream>
#include <string>
#include <cctype>
#include <locale>
#include <stdexcept>

double expression(const char*& expressionToParse);

// Возвращает текущий символ в выражении, не изменяя указатель
char peek(const char* expressionToParse) {
    return *expressionToParse;
}

// Возвращает текущий символ и продвигает указатель вперёд
char get(const char*& expressionToParse) {
    return *expressionToParse++;
}

// Обрабатывает ошибку, выбрасывая исключение с заданным сообщением
void handleError(const std::string& message) {
    throw std::runtime_error("Ошибка: " + message);
}

// Парсит число из выражения и продвигает указатель
double number(const char*& expressionToParse) {
    // Используется std::stod для конвертации подстроки в число с плавающей точкой
    double result = std::stod(std::string(expressionToParse));

    // Продвигаем указатель до тех пор, пока символы являются цифрами или точкой
    while (std::isdigit(peek(expressionToParse)) || peek(expressionToParse) == '.') {
        get(expressionToParse);
    }
    return result;
}

// Обрабатывает фактор в выражении (число, унарный минус или выражение в скобках)
double factor(const char*& expressionToParse) throw(std::runtime_error) {
    // Если текущий символ - минус, обрабатываем унарный минус
    if (peek(expressionToParse) == '-') {
        get(expressionToParse);
        return -number(expressionToParse);
    }

    // Если текущий символ - плюс, обрабатываем сообщение "Партия гордится тобой!"
    else if (peek(expressionToParse) == '+' && peek(expressionToParse++) == '+') {
        handleError("Партия гордится тобой!");
        return 0;
    }

    // Если текущий символ - цифра, обрабатываем число
    else if (std::isdigit(peek(expressionToParse))) {
        return number(expressionToParse);
    }

    // Если текущий символ - открывающая скобка, обрабатываем выражение в скобках
    else if (peek(expressionToParse) == '(') {
        get(expressionToParse);
        double result = expression(expressionToParse);

        // Проверяем наличие закрывающей скобки
        if (peek(expressionToParse) != ')') {
            handleError("Отсутствует закрывающая скобка");
            return 0;
        }

        get(expressionToParse); // Продвигаем указатель на символ после закрывающей скобки
        return result;
    }

    throw std::runtime_error("Неверный формат выражения");
}

// Обрабатывает терм в выражении (умножение и деление)
double term(const char*& expressionToParse) {
    // Обрабатываем первый фактор
    double result = factor(expressionToParse);

    // Продолжаем обработку, пока есть умножение или деление
    while (peek(expressionToParse) == '*' || peek(expressionToParse) == '/') {
        char op = get(expressionToParse);
        double nextFactor = factor(expressionToParse);

        // Выполняем умножение или деление в зависимости от оператора
        if (op == '*') {
            result *= nextFactor;
        }
        else if (nextFactor != 0) {
            result /= nextFactor;
        }
        else {
            handleError("Деление на ноль");
            return 0;
        }
    }
    return result;
}

// Обрабатывает выражение (сложение и вычитание)
double expression(const char*& expressionToParse) {
    // Обрабатываем первый терм
    double result = term(expressionToParse);

    // Продолжаем обработку, пока есть сложение или вычитание
    while (peek(expressionToParse) == '+' || peek(expressionToParse) == '-') {
        char op = get(expressionToParse);
        double nextTerm = term(expressionToParse);

        
        // Выполняем сложение или вычитание в зависимости от оператора
        if (op == '+') {
            result += nextTerm;
        }
        else {
            result -= nextTerm;
        }
    }
    return result;
}

// Удаляет пробелы из строки
void removeSpaces(std::string& str) {
    std::locale loc("");
    // Используем erase и remove_if для удаления пробелов из строки
    str.erase(std::remove_if(str.begin(), str.end(), [&loc](unsigned char c) {
        return std::isspace(c, loc);
        }), str.end());
}

// Проверяет, является ли символ допустимым в выражении
bool isValidSymbol(char c) {
    return std::isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '.';
}

// Проверяет, содержит ли строка только допустимые символы
bool isValidExpression(const std::string& str) {
    // Проверяем каждый символ в строке
    for (char c : str) {
        if (!isValidSymbol(c)) {
            return false;
        }
    }
    return true;
}

// Получает ввод выражения от пользователя
std::string getInputExpression() {
    std::cout << ">_ ";
    std::string userInput;
    // Получаем строку ввода от пользователя с помощью std::getline
    std::getline(std::cin, userInput);
    return userInput;
}

// Основная функция программы
int main() {
    // Устанавливаем кодировку для корректного отображения кириллических символов в консоли
    system("chcp 1251");

    std::cout << "Введите арифметическое выражение:\n";

    // Основной цикл программы
    while (true) {
        // Получаем ввод выражения от пользователя
        std::string userInput = getInputExpression();

        // Проверяем, если введена пустая строка, завершаем программу
        if (userInput.empty()) {
            std::cout << "До встречи!" << std::endl;
            break;
        }

        // Удаляем пробелы из введённой строки
        removeSpaces(userInput);

        // Проверяем, содержит ли строка только допустимые символы
        if (!isValidExpression(userInput)) {
            std::cerr << "Ошибка: Введены недопустимые символы. Пожалуйста, введите выражение заново." << std::endl;
            continue;
        }

        // Преобразуем строку в массив символов для обработки
        const char* expressionToParse = userInput.c_str();

        try {
            // Вычисляем результат выражения
            double result = expression(expressionToParse);

            // Проверяем на переполнение
            if (std::isinf(result)) {
                std::cout << "Переполнение" << std::endl;
            }
            else {
                std::cout << "Результат: " << result << std::endl;
            }
        }
        // Обрабатываем исключения, возникающие при ошибках в выражении
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            continue;
        }
    }

    return 0;
}
