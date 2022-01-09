
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

#include "../lib/util.hpp"

bool data_exists(std::string ticker) {
    std::ifstream f("./data/" + ticker + ".csv");
    return f.is_open();
}

bool download(std::string ticker) {
    if (!data_exists(ticker)) {
        std::string cmd = "./python/download.py " + ticker;
        std::system(cmd.c_str());
    }
    return data_exists(ticker);
}

std::vector<std::string> read_lines(std::string path) {
    std::vector<std::string> lines;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line;
        while(std::getline(f, line)) {
            lines.push_back(line);
        }
        f.close();
    }

    return lines;
}

std::vector<std::vector<double>> load_data(std::string path) {
    std::vector<std::vector<double>> data;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line, val;
        while(std::getline(f, line)) {
            std::vector<double> x;
            for(unsigned int i = 0; i < line.length(); i++) {
                if(line[i] != ' ') {
                    val += line[i];
                    if(line[i+1] == ' ' || i == line.length() - 1) {
                        x.push_back(std::stod(val));
                        val = "";
                    }
                }
            }
            data.push_back(x);
            x.clear();
        }
        f.close();
    }

    return data;
}

void sort(std::vector<std::string> &label, std::vector<double> &value) {
    // selection sort (max ... min)
    for(unsigned int i = 0; i < value.size() - 1; i++) {
        std::string max_label = "";
        double max_value = 0.00;
        int k = 0;
        for(unsigned int j = i; j < value.size(); j++) {
            if(value[j] > max_value) {
                max_label = label[j];
                max_value = value[j];
                k = j;
            }
        }

        std::swap(label[i], label[k]);
        std::swap(value[i], value[k]);
    }
}

