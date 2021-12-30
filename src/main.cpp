
#include <iostream>
#include <cstdlib>
#include <vector>

#include "../lib/dnn.hpp"
#include "../lib/linear.hpp"

void test_linear() {
    LinearRegression model;

    std::vector<double> x;
    std::vector<double> y;

    for(unsigned int i = 0; i < 1000; i++) {
        x.push_back(i);
        y.push_back(i*-1+3);
    }

    double correlation = model.fit(x, y);

    std::cout << correlation << std::endl;
}

void test_dnn() {
    std::vector<std::vector<double>> x;
    for(unsigned int k = 0; k < 100; k++) {
        std::vector<double> x_k;
        for(unsigned int i = 0; i < 3; i++) {
            x_k.push_back(-1.0 + (double)rand() / RAND_MAX * (1.0 - (-1.0)));
        }
        x.push_back(x_k);
    }

    std::vector<std::vector<double>> y;
    for(unsigned int k = 0; k < 100; k++) {
        std::vector<double> y_k;
        for(unsigned int i = 0; i < 2; i++) {
            y_k.push_back((double)rand() / RAND_MAX);
        }
        y.push_back(y_k);
    }

    DeepNet model("test", {{3,3}, {3,3}, {3,2}});
    //model.use_softmax();
    model.load();

    for(unsigned int i = 0; i < 10; i++) {
        for(unsigned int k = 0; k < 100; k++) {
            model.fit(x[k], y[k], 0.01);
        }
    }

    model.save();
}

int main()
{
    //test_linear();
    //test_dnn();
    return 0;
}
