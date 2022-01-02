
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

#include "../lib/data.hpp"

std::vector<double> load_adjusted_close(std::string path) {
    std::vector<double> data;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line, val = "";
        unsigned int line_count, value_count = 0;

        while(std::getline(f, line)) {
            if(line_count != 0) {
                int comma[8];
                for(unsigned int i = 0; i < line.length(); i++) {
                    if(line[i] == ','){
                        comma[value_count] = i;
                        value_count++;
                    }
                }

                for(unsigned int i = comma[4] + 1; i < comma[5]; i++) {
                    val += line[i];
                }
                data.push_back(std::stod(val));
            }
            line_count++;
            value_count = 0;
            val = "";
        }
        f.close();
    }

    return data;
}
