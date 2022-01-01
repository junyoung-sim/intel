
#include <cstdlib>
#include <vector>

#include "../lib/signal.hpp"

void pattern_detection(std::vector<double> &raw, unsigned int window) {
    /*
        detect predictable patterns in a signal and the probability of their successive behavior
    */
    std::vector<unsigned int> signal; // 0: low, 1: high
    for(unsigned int i = 0; i < raw.size() - 1; i++) {
        double delta = raw[i+1] - raw[i];
        signal.push_back(delta > 0 ? 1 : 0);
    }

    std::vector<std::vector<unsigned int>> pattern_stack;
    std::vector<unsigned int> frequency;

    for(unsigned int i = 0; i <= signal.size() - window; i++) {
        std::vector<unsigned int> pattern;
        for(unsigned int k = i; k < i + window; k++) {
            pattern.push_back(signal[k]);
        }
        pattern_stack.push_back(pattern);
        pattern.clear();

        unsigned int count = 0;
        for(unsigned int k = 0; k <= signal.size() - window; k++) {
            double match = 0.00;
            for(unsigned int j = k; j < k + window; j++) {
                if(signal[j] == pattern[j-k]) {
                    match += 1.00;
                }
            }
            match /= window;

            if(match > 0.80) count++;
        }
        frequency.push_back(count);
    }
}
