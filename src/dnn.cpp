
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#include "../lib/dnn.hpp"
#include "../lib/data.hpp"
#include "../lib/bar.hpp"

double elu(double x) { return x > 0.00 ? x : exp(x) - 1; }
double elu_prime(double x) { return x > 0.00 ? 1.00 : exp(x); }

void softmax(std::vector<double> &v) {
    double exp_sum = 0.00;
    for(double &v_i: v)
        exp_sum += exp(v_i);

    for(double &v_i: v)
        v_i = exp(v_i) / exp_sum;
}

double mse(std::vector<double> &y, std::vector<double> &yhat) {
    double loss = 0.00;
    for(unsigned int i = 0; i < y.size(); i++)
        loss += pow(y[i] - yhat[i], 2);
    loss /= y.size();
    return loss;
}

double binary_cross_entropy(std::vector<double> &y, std::vector<double> &yhat) {
    double loss = 0.00;
    for(unsigned int i = 0; i < y.size(); i++)
        loss += y[i] * log(yhat[i]) + (1 - y[i]) * log(1 - yhat[i]);
    loss *= -1.00 / y.size();
    return loss;
}

// --- //

void Node::init() {
    sum = 0.00;
    act = 0.00;
    err = 0.00;
}

double Node::bias() { return b; }
double Node::summation() { return sum; }
double Node::activation() { return act; }
double Node::error() { return err; }
std::vector<double> *Node::weights() { return &w; }

void Node::set_summation(double dot) { sum = dot + b; }
void Node::compute_activation() { act = elu(sum); }
void Node::add_error(double val) { err += val; }
void Node::update_bias(double delta) { b -= delta; }

// --- //

unsigned int Layer::in_features() { return in; }
unsigned int Layer::out_features() { return out; }
std::vector<Node> *Layer::nodes() { return &n; }

// --- //

void DNN::use_classifier() {
    classifier = true;
}

std::vector<double> DNN::predict(std::vector<double> &x) {
    std::vector<double> yhat;
    for(unsigned int l = 0; l < layers.size(); l++) {
        std::vector<Node> *nodes = layers[l].nodes();
        for(unsigned int n = 0; n < layers[l].out_features(); n++) {
            double dot = 0.00;
            std::vector<double> *weights = (*nodes)[n].weights();
            for(unsigned int i = 0; i < layers[l].in_features(); i++) {
                if(l == 0)
                    dot += x[i] * (*weights)[i];
                else
                    dot += (*layers[l-1].nodes())[i].activation() * (*weights)[i];
            }

            (*nodes)[n].init();
            (*nodes)[n].set_summation(dot);
            (*nodes)[n].compute_activation();

            if(l == layers.size() - 1) {
                if(classifier)
                    yhat.push_back((*nodes)[n].summation());
                else
                    yhat.push_back((*nodes)[n].activation());
            }
        }
    }

    if(classifier)
        softmax(yhat);

    return yhat;
}

void DNN::fit(std::vector<double> &x, std::vector<double> &y, double alpha, unsigned int dataset_size) {
    std::vector<double> yhat = predict(x);
    // stochastic gradient descent
    for(int l = layers.size() - 1; l >= 0; l--) {
        std::vector<Node> *nodes = layers[l].nodes();

        double partial_gradient = 0.00, gradient = 0.00;
        for(unsigned int n = 0; n < layers[l].out_features(); n++) {
            if(l == layers.size() - 1) {
                if(classifier)
                    partial_gradient = -1.00 / y.size() * (y[n] - yhat[n]); // softmax layer, binary cross entropy
                else
                    partial_gradient = -2.00 / y.size() * (y[n] - yhat[n]) * elu_prime((*nodes)[n].summation()); // MSE
            }
            else
                partial_gradient = (*nodes)[n].error() * elu_prime((*nodes)[n].summation());

            (*nodes)[n].update_bias(alpha * partial_gradient);

            std::vector<double> *weights = (*nodes)[n].weights();
            for(unsigned int i = 0; i < layers[l].in_features(); i++) {
                if(l > 0) {
                    gradient = partial_gradient * (*layers[l-1].nodes())[i].activation();
                    (*layers[l-1].nodes())[i].add_error(partial_gradient * (*weights)[i]);
                }
                else
                   gradient = partial_gradient * x[i];

                gradient += 1.00 / dataset_size * (*weights)[i]; // L2 Regularization
                (*weights)[i] -= alpha * gradient;
            }
        }
    }

    std::vector<double>().swap(yhat);
}

void DNN::train(std::vector<std::vector<double>> &train_x, std::vector<std::vector<double>> &train_y,
                std::vector<std::vector<double>> &validation_x, std::vector<std::vector<double>> &validation_y,
                unsigned int epoch, unsigned int iteration, unsigned int batch_size, double alpha, double decay) {
    unsigned int batch_num = 1;
    unsigned int batch_start = 0;
    unsigned int batch_end = batch_size;

    double training_loss = 0.00;
    double validation_loss = 0.00;

    std::vector<double> training_loss_history, validation_loss_history;

    for(unsigned int e = 1; e <= epoch; e++) {
        shuffle(train_x, train_y);
        std::cout << "=======================================EPOCH " + std::to_string(e) + "=======================================\n";

        while(batch_start < train_x.size()) {
            std::vector<std::vector<double>> batch_x = {train_x.begin() + batch_start, train_x.begin() + batch_end};
            std::vector<std::vector<double>> batch_y = {train_y.begin() + batch_start, train_y.begin() + batch_end};

            for(unsigned int i = 1; i <= iteration; i++) {
                 for(unsigned int k = 0; k < batch_x.size(); k++)
                     fit(batch_x[k], batch_y[k], alpha, train_x.size());

                 double loss = 0.00;
                 for(unsigned int k = 0; k < batch_x.size(); k++) {
                     std::vector<double> yhat = predict(batch_x[k]);
                     if(classifier)
                         loss += binary_cross_entropy(batch_y[k], yhat);
                     else
                         loss += mse(batch_y[k], yhat);

                     std::vector<double>().swap(yhat);
                 }
                 loss /= batch_x.size();

                 progress_bar(i, iteration, "BATCH " + std::to_string(batch_num) + " [LOSS = " + std::to_string(loss) + "]");
            }

            std::vector<std::vector<double>>().swap(batch_x);
            std::vector<std::vector<double>>().swap(batch_y);

            batch_num++;
            batch_start += batch_size;
            batch_end + batch_size < train_x.size() ? batch_end += batch_size : batch_end = train_x.size();
        }

        // --- //

        training_loss = 0.00;
        for(unsigned int k = 0; k < train_x.size(); k++) {
            std::vector<double> yhat = predict(train_x[k]);
            if(classifier)
                training_loss += binary_cross_entropy(train_y[k], yhat);
            else
                training_loss += mse(train_y[k], yhat);

            std::vector<double>().swap(yhat);
        }
        training_loss /= train_x.size();
        training_loss_history.push_back(training_loss);
        std::cout << "\nTRAINING LOSS    = " << training_loss;

        // --- //

        validation_loss = 0.00;
        for(unsigned int k = 0; k < validation_x.size(); k++) {
            std::vector<double> yhat = predict(validation_x[k]);
            if(classifier)
                validation_loss += binary_cross_entropy(validation_y[k], yhat);
            else
                validation_loss += mse(validation_y[k], yhat);

            std::vector<double>().swap(yhat);
        }
        validation_loss /= validation_x.size();
        validation_loss_history.push_back(validation_loss);
        std::cout << "\nVALIDATION LOSS  = " << validation_loss << "\n\n";

        // --- //

        batch_num = 1;
        batch_start = 0;
        batch_end = batch_size;

        if(e % (int)(epoch / 10) == 0) alpha *= 1.00 - decay;
    }

    // save loss history
    std::ofstream out("./res/loss");
    if(out.is_open()) {
        for(unsigned int i = 0; i < training_loss_history.size(); i++) {
            out << training_loss_history[i] << " " << validation_loss_history[i];
            if(i != training_loss_history.size() - 1)
                out << "\n";
        }

        out.close();
    }

    std::vector<double>().swap(training_loss_history);
    std::vector<double>().swap(validation_loss_history);
}

