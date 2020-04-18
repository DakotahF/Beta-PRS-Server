#include "Individuals.h"
#include <vector>
#include <string>
#include <tuple>


Individual_row::Individual_row(int c)
: cols(c) {}

std::vector<long double> Individual_row::return_row() {
    double init_val = 0.00; 
    std::vector<long double> vec(this->cols,init_val);
    return vec;
}
Individual_scores::Individual_scores(int n_w, int n_id)
: num_weights(n_w), num_ids(n_id) {
    this->scores.reserve(this->num_weights);
    int idx = 0; 
    while(idx < this->num_weights) {
        Individual_row row(n_id);
        this->scores.push_back(row.return_row());
        idx++;
    }
}

