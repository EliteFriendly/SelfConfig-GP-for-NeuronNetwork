
#include <random>
#include <cmath>
#include <iostream>
#include <functional>
using namespace std;

#pragma once
//General 
static mt19937 gen(90086); // random generator

//for Tree
static const int amBinaryFunc = 2; // Количество бинарных функций
static const int amUnaryFunc = 1; // Количество унарных функций
static const vector<string> strBinaryFunc = { "+", ">" }; // Символьный вывод функции
static const vector<string> strUnaryFunc = {"recc"}; // Символьный вывод функции unary
static const int amFuncActive = 15; // Количество функций активации

static double fixExp(double x)
{ // fix exponent
    if (x <= -10)
        return exp(-10);
    if (x >= 10)
        return exp(10);
    return exp(x);
}
static double fixLog(double x)
{ // fix log
    if (x <= 0)
        return 0;
    return log(x);
}
static double fixPow(double x, double y)
{ // fix pow
    double result = pow(x, y);
    if (result == 0 && x != 0)
        return 0;
    if (abs(result) > 1e50)
    {
        if (result > 0)
            return 1e50;
        else
            return -1e50;
    }
    return pow(x, y);
}


static function<double(double)> funcActivation[16] = {
        [](double x) { return x; },      // 0
        [](double x) { return sin(x); }, // 1
        [](double x) {
            if (x < -1)
                return -1.0;
            if (x > 1)
                return 1.0;
            else
                return x;
        },                                                // 2
        [](double x) { return 2 / (1 + fixExp(x)) - 1; }, // 3
        [](double x) { return fixExp(x); },               // 4
        [](double x) { return abs(x); },                  // 5
        [](double x) { return 1 - fixExp(x); },           // 6
        [](double x) { return 0; },                       // 7
        [](double x) { return fixPow(x, 2); },            // 8
        [](double x) { return fixPow(x, 3); },            // 9
        [](double x) {
            if (x == 0)
                return 0.0;
            return fixPow(x, -1);
        },                                             // 10
        [](double x) { return 1; },                    // 11
        [](double x) { return 1 / (1 + fixExp(-x)); }, // 12
        [](double x) { return fixExp(-(x * x) / 2); }, // 13
        [](double x) {
            if (x < -1 / 2)
                return -1.0;
            if (x > 1 / 2)
                return 1.0;
            else
                return x + 1 / 2;
        } // 14
    };





//for DE
static const int numInd = 30;
static const int numGen = 30;
