
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

#include "../lib/signal.hpp"

void binary_pattern_detection(std::vector<double> &raw, unsigned int window) {
    std::vector<unsigned int> signal; // 0: down (low), 1: up (high)
    for(unsigned int i = 0; i < raw.size() - 1; i++) {
        double delta = raw[i+1] - raw[i];
        signal.push_back(delta > 0 ? 1 : 0);
    }

    // detect patterns
    std::vector<std::vector<unsigned int>> pattern_stack;
    std::vector<unsigned int> frequency;
    std::vector<double> outcome_probability;

    for(unsigned int i = 0; i <= signal.size() - window; i++) {
        std::vector<unsigned int> pattern;
        for(unsigned int k = i; k < i + window; k++) {
            pattern.push_back(signal[k]);
        }

        bool new_pattern = true;
        for(std::vector<unsigned int> &stacked: pattern_stack) {
            unsigned int match = 0;
            for(unsigned int i = 0; i < window; i++) {
                if(pattern[i] == stacked[i]) match++;
            }
            if(match == window) new_pattern = false;
        }

        if(new_pattern) {
            std::cout << "Scanning signal for the pattern [ ";
            for(unsigned int &val: pattern) {
                std::cout << val << " ";
            }
            std::cout << "] ...\n";

            unsigned int count = 0;
            double p_high = 0.00;
            for(unsigned int k = 0; k <= signal.size() - window; k++) {
                bool match;
                for(unsigned int j = k; j < k + window; j++) {
                    match = signal[j] == pattern[j-k];
                    if(!match) break;
                }
                
                if(match) {
                    count++;
                    // update probability of observing high (1) after the pattern
                    if(signal[k+window] == 1) p_high += 1.00;
                }
            }

            p_high /= count;

            std::cout << "Frequency = " << count << "\n";
            std::cout << "P(X[t+1] = 1) = " << p_high << "\n\n";

            pattern_stack.push_back(pattern);
            frequency.push_back(count);
            outcome_probability.push_back(p_high);
        }
    }
}
