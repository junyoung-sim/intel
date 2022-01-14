
#include <iostream>
#include <cstdlib>
#include <vector>

#include "../lib/bar.hpp"
#include "../lib/util.hpp"
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
    std::vector<std::vector<double>> x = {{0.3, 0.2}, {0.6, 0.9}, {1.0,0.2}, {0.8, 0.9}, {0.4, 0.1}, {0.1, 0.3}};
    std::vector<std::vector<double>> y = {{0.0}, {1.0}, {1.0}, {1.0}, {0.0}, {0.0}};

    DeepNet model("test");
    if(!model.load()) model.init({{2,2},{2,1}});

    unsigned int epoch = 1000;
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
    model.kill();
}

void test_vector_estimation() {
    std::vector<double> data = load_data("./data/data")[0];

    double vector_est = vector_estimation(data);
    std::cout << vector_est << "\n";
    
}

int main()
{
    //test_linear();
    test_dnn();
    //test_vector_estimation();
    return 0;

}

