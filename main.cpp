
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

}

int main()
{
    //test_linear();
    //test_dnn();
    return 0;
}
