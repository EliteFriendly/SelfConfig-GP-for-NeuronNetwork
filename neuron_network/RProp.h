#include "Tree.h"
#include <functional>
#include <iostream>

using namespace std;

class RProp
{
  private:
    int amderivativeFuncActivation = 17;
    function<double(double)> derivativeFuncActivation[17] = {
        [](double x) { return 1; },                                                   // 0
        [](double x) { return (1 / (1 + fixExp(-x))) / (1 - 1 / (1 + fixExp(-x))); }, // 1
        [](double x) {
            return ((1 / (1 + fixExp(-x))) + x * (1 / (1 + fixExp(-x)))) / (1 - 1 / (1 + fixExp(-x)));
        },                                   // 2
        [](double x) { return fixExp(-x); }, // 3
        [](double x) {
            if (x > 0)
                return 1;
            else
                return 0;
        },                                                 // 4
        [](double x) { return (1.0 - pow(tanh(x), 2)); },  // 5
        [](double x) { return pow(x / (1 + abs(x)), 2); }, // 6
        [](double x) { return cos(x); },                   // 7
        [](double x) {
            if (x >= -1 and x <= 1)
                return 1;
            else
                return 0;
        },                                                                 // 8
        [](double x) { return -2 * fixExp(x) / pow((1 + fixExp(x)), 2); }, // 9
        [](double x) { return (fixExp(x)); },                              // 10
        [](double x) { return 0; },                                        // 11
        [](double x) { return 2 * x; },                                    // 12
        [](double x) { return 3 * fixPow(x, 2); },                         // 13
        [](double x) {
            if (x == 0)
                return 0.0;
            return -1 * fixPow(x, -2);
        },                                                        // 14
        [](double x) { return 0; },                               // 15
        [](double x) { return -0.5 * x * fixExp(-(x * x) / 2); }, // 16

    };

  public:
    RProp(Neuron us, double *coef, int) {};
}
