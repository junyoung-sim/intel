
#ifndef __DATA_HPP_
#define __DATA_HPP_

#include <cstdlib>
#include <vector>
#include <string>

std::vector<double> read(std::string path);
void write(std::vector<double> &dat, std::string path);

// --- //

void shuffle(std::vector<std::vector<double>> &x, std::vector<std::vector<double>> &y);

// --- //

double mean(std::vector<double> &dat);
double std_dev(std::vector<double> &dat);

void standardize(std::vector<double> &dat);

#endif
