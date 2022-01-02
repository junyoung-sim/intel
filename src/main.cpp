
#include <iostream>
#include <cstdlib>
#include <vector>

#include "../lib/bar.hpp"
#include "../lib/data.hpp"
#include "../lib/dnn.hpp"
#include "../lib/linear.hpp"
#include "../lib/signal.hpp"

void test_linear() {
    LinearRegression model;

    std::vector<double> x;
    std::vector<double> y;

    for(unsigned int i = 0; i < 1000; i++) {
        x.push_back(i);
        y.push_back(i*-1+3);
    }

    double r = model.fit(x, y);

    std::cout << r << std::endl;
}

void test_dnn() {
    std::vector<std::vector<double>> x;
    for(unsigned int k = 0; k < 1000; k++) {
        std::vector<double> x_k;
        for(unsigned int i = 0; i < 3; i++) {
            x_k.push_back(-1.0 + (double)rand() / RAND_MAX * (1.0 - (-1.0)));
        }
        x.push_back(x_k);
    }

    std::vector<std::vector<double>> y;
    for(unsigned int k = 0; k < 1000; k++) {
        std::vector<double> y_k;
        for(unsigned int i = 0; i < 2; i++) {
            y_k.push_back((double)rand() / RAND_MAX);
        }
        y.push_back(y_k);
    }

    DeepNet model("test", {{3,3},{3,2}});
    model.load();

    unsigned int epoch = 10000;
    double alpha = 0.01;
    double decay = 0.01;

    for(unsigned int t = 1; t <= epoch; t++) {
        for(unsigned int k = 0; k < x.size(); k++) {
            model.fit(x[k], y[k], alpha);
        }

        double loss = 0.00;
        for(unsigned int k = 0; k < x.size(); k++) {
            std::vector<double> yhat = model.predict(x[k]);
            loss += mse(y[k], yhat);
        }
        loss /= x.size();

        if(t % (int)(epoch / 10) == 0) alpha *= 1 - decay;

        progress_bar(t, epoch, "EPOCH #" + std::to_string(t) + " [LOSS = " + std::to_string(loss) + "]");
    }

    model.save();
}

void test_signal_pattern_detection() {
     std::vector<double> raw = load_adjusted_close("./data/spy.csv");
     binary_pattern_detection(raw, 5);
}

int main()
{
    //test_linear();
    //test_dnn();
    //test_signal_pattern_detection();
    return 0;
}

