
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

double vector_estimation(std::vector<double> &data) {
    double vector_sum = 0.00;
    double positive_probability = 0.00;

    for(unsigned int i = 0; i < data.size() - 1; i++) {
        double vec_i = (data[data.size() - 1] - data[i]) / ((data.size() - 1) - i);
        vector_sum += vec_i;

        if(vec_i > 0.00) positive_probability += 1.0;
    }

    positive_probability /= data.size() - 1;
    vector_sum = 1 / (1 + exp(-vector_sum)); // sigmoid

    double vector_est = positive_probability * vector_sum;
    return vector_est;
}
