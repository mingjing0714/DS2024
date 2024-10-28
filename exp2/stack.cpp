#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <cctype>
#include <stdexcept>
#include <cmath>

template <typename T>
class Stack {
public:
    void push(const T& item) {
        items.push_back(item);
    }

    T pop() {
        if (is_empty()) {
            throw std::out_of_range("Stack is empty");
        }
        T item = items.back();
        items.pop_back();
        return item;
    }

    bool is_empty() const {
        return items.empty();
    }

    size_t size() const {
        return items.size();
    }

    T peek() const {
        if (is_empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return items.back();
    }

private:
    std::vector<T> items;
};

class StringCalculator {
public:
    StringCalculator() {
        operators['+'] = {1, [](double a, double b) { return a + b; }};
        operators['-'] = {1, [](double a, double b) { return a - b; }};
        operators['*'] = {2, [](double a, double b) { return a * b; }};
        operators['/'] = {2, [](double a, double b) { return a / b; }};
    }

    double calculate(const std::string& expression) {
        std::vector<std::string> tokens = tokenize(expression);
        std::vector<std::string> output;
        Stack<std::string> op_stack;

        for (const auto& token : tokens) {
            if (isdigit(token[0]) || (token.size() > 1 && isdigit(token[1]))) {
                output.push_back(token);
            } else if (operators.count(token[0])) {
                while (!op_stack.is_empty() && op_stack.peek() != "(" &&
                       precedence(op_stack.peek()[0]) >= precedence(token[0])) {
                    output.push_back(op_stack.pop());
                }
                op_stack.push(token);
            } else if (token == "(") {
                op_stack.push(token);
            } else if (token == ")") {
                while (!op_stack.is_empty() && op_stack.peek() != "(") {
                    output.push_back(op_stack.pop());
                }
                op_stack.pop();  // Pop the '('
            } else if (is_function(token)) {
                output.push_back(token); // Push the function to the output for RPN processing
            }
        }

        while (!op_stack.is_empty()) {
            output.push_back(op_stack.pop());
        }

        return evaluate_rpn(output);
    }

protected:
    struct Operator {
        int precedence;
        std::function<double(double, double)> func;
    };

    std::map<char, Operator> operators;

    int precedence(char op) {
        return operators[op].precedence;
    }

    std::vector<std::string> tokenize(const std::string& expression) {
        std::istringstream iss(expression);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    double evaluate_rpn(const std::vector<std::string>& rpn) {
        Stack<double> stack;
        for (const auto& token : rpn) {
            if (isdigit(token[0]) || (token.size() > 1 && isdigit(token[1]))) {
                stack.push(std::stod(token));
            } else if (operators.count(token[0])) {
                if (stack.size() < 2) throw std::runtime_error("Insufficient operands for operator");
                double b = stack.pop();
                double a = stack.pop();
                double result = operators[token[0]].func(a, b);
                stack.push(result);
            } else if (is_function(token)) {
                if (stack.is_empty()) throw std::runtime_error("Insufficient operands for function");
                double a = stack.pop();
                double result = evaluate_function(token, a);
                stack.push(result);
            }
        }
        if (stack.size() != 1) throw std::runtime_error("Error in expression evaluation");
        return stack.pop();
    }

    bool is_function(const std::string& token) {
        return token == "sin" || token == "cos" || token == "tan" || token == "log";
    }

    double evaluate_function(const std::string& func, double a) {
        if (func == "sin") return std::sin(a);
        if (func == "cos") return std::cos(a);
        if (func == "tan") return std::tan(a);
        if (func == "log") return std::log10(a);  // 使用以10为底的对数
        throw std::invalid_argument("Unknown function: " + func);
    }
};

class AdvancedStringCalculator : public StringCalculator {
public:
    double calculate(const std::string& expression) {
        std::string processed_expression = preprocess(expression);
        return StringCalculator::calculate(processed_expression);
    }

private:
    std::string preprocess(const std::string& expression) {
        std::string result = expression;
        replace(result, "pi", std::to_string(M_PI));
        replace(result, "e", std::to_string(M_E));
        return result;
    }

    void replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
};

int main() {
    AdvancedStringCalculator calc;

    // 测试基本运算
    std::cout << "3 + 5 * (2 - 8) = " << calc.calculate("3 + 5 * ( 2 - 8 )") << std::endl; // 输出: -27
    std::cout << "10 + 2 * 6 = " << calc.calculate("10 + 2 * 6") << std::endl;              // 输出: 22
    std::cout << "100 * 2 + 12 = " << calc.calculate("100 * 2 + 12") << std::endl;          // 输出: 212
    std::cout << "100 * (2 + 12) = " << calc.calculate("100 * ( 2 + 12 )") << std::endl;    // 输出: 1400
    std::cout << "100 * (2 + 12) / 14 = " << calc.calculate("100 * ( 2 + 12 ) / 14") << std::endl; // 输出: 100

    // 测试三角函数和对数
    std::cout << "sin(pi / 2) + 10 * log(100) = " << calc.calculate("sin(pi / 2) + 10 * log(100)") << std::endl; // 应输出: 20
    std::cout << "cos(0) + 5 = " << calc.calculate("cos(0) + 5") << std::endl;                          // 应输出: 6
    std::cout << "tan(pi / 4) = " << calc.calculate("tan(pi / 4)") << std::endl;                        // 应输出: 1
}
