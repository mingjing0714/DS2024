#include <iostream>
#include <cctype>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

template <typename T>
class Stack {
public:
    Stack(int size);
    ~Stack();
    bool IsEmpty() { return top == -1; }
    bool IsFull() { return top == maxTop; }
    T Top();
    void Push(const T x);
    T Pop();
private:
    int maxTop;
    int top;
    T* values;
};

template <typename T>
Stack<T>::Stack(int size) {
    maxTop = size - 1;
    values = new T[size];
    top = -1;
}

template <typename T>
Stack<T>::~Stack() {
    delete[] values;
}

template <typename T>
void Stack<T>::Push(const T x) {
    if (IsFull())
        cout << "Error: the stack is full." << endl;
    else
        values[++top] = x;
}

template <typename T>
T Stack<T>::Pop() {
    if (IsEmpty()) {
        cout << "Error: the stack is empty." << endl;
        return T();
    } else {
        return values[top--];
    }
}

template <typename T>
T Stack<T>::Top() {
    if (IsEmpty()) {
        cout << "Error: the stack is empty." << endl;
        return T();
    } else {
        return values[top];
    }
}

// 运算符优先级表
const int N_OPTR = 9;
const char pri[N_OPTR][N_OPTR] = {
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}
};

int optrIndex(char op) {
    switch (op) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        case '^': return 4;
        case '!': return 5;
        case '(': return 6;
        case ')': return 7;
        case '\0': return 8;
        default: return -1;
    }
}

char getPriority(char topOp, char currentOp) {
    int i = optrIndex(topOp);
    int j = optrIndex(currentOp);
    if (i == -1 || j == -1) {
        cout << "错误的运算符" << endl;
        exit(-1);
    }
    return pri[i][j];
}

double factorial(int n) {
    if (n < 0) {
        cout << "Error: Negative factorial" << endl;
        exit(-1);
    }
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double calcu(double operand1, char op, double operand2 = 0) {
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 == 0) {
                cout << "Error: Division by zero" << endl;
                exit(-1);
            }
            return operand1 / operand2;
        case '^': return pow(operand1, operand2);
        default:
            cout << "Error: Unknown operator" << endl;
            exit(-1);
    }
}

double calcu(char op, double operand) {
    switch (op) {
        case '!':
            return factorial(static_cast<int>(operand));
        case 's': // sin
            return sin(operand);
        case 'c': // cos
            return cos(operand);
        case 't': // tan
            return tan(operand);
        case 'l': // log
            return log(operand);
        default:
            cout << "Error: Unknown function" << endl;
            exit(-1);
    }
}

double evaluate(const string& expr) {
    Stack<double> opnd(100);
    Stack<char> optr(100);
    optr.Push('\0');
    const char* s = expr.c_str();
    
    while (*s) {
        if (isdigit(*s) || (*s == '.')) {
            double num = 0;
            double frac = 0.1;
            bool hasDecimal = false;

            while (isdigit(*s) || (*s == '.')) {
                if (*s == '.') {
                    hasDecimal = true;
                } else {
                    if (hasDecimal) {
                        num += (*s - '0') * frac;
                        frac *= 0.1;
                    } else {
                        num = num * 10 + (*s - '0');
                    }
                }
                s++;
            }
            opnd.Push(num);
        } else if (*s == 's' || *s == 'c' || *s == 't' || *s == 'l') {
            char func = *s;
            s++; // Skip the function character
            if (*s != '(') {
                cout << "Error: Expected '(' after function" << endl;
                exit(-1);
            }
            s++; // Skip '('
            double operand = evaluate(s);
            opnd.Push(calcu(func, operand));
            while (*s != ')') s++; // Skip to ')'
            s++; // Skip ')'
        } else {
            switch (getPriority(optr.Top(), *s)) {
                case '<':
                    optr.Push(*s);
                    s++;
                    break;

                case '>': {
                    char op = optr.Pop();
                    if (op == '!') {
                        opnd.Push(calcu(op, opnd.Pop()));
                    } else {
                        double opnd2 = opnd.Pop();
                        double opnd1 = opnd.Pop();
                        opnd.Push(calcu(opnd1, op, opnd2));
                    }
                    break;
                }

                case '=':
                    optr.Pop();
                    s++;
                    break;
                default:
                    s++;
            }
        }
    }
    return opnd.Pop();
}


int main() {
    string expression;
    cout << "请输入一个数学表达式: ";
    getline(cin, expression);
    double result = evaluate(expression);
    cout << "计算结果: " << result << endl;

    return 0;
}
