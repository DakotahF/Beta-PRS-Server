#ifndef Weights_h
#define Weights_h
#include <string>
#include <vector>


class Row
{
public:
    int pos;
    std::string chr,ref,alt;
    double p_value;
    std::vector<long double> weights;
    bool is_empty = 1;
    
    //member functions
    Row() = default;
    Row(std::string chr, int pos, std::string ref, std::string alt, double p_value, std::vector<long double> weights);

    void read_weight(std::string line) const {};
    
};


class Weights {
protected:
    std::string file_name;
    std::string phenotype;
    std::string weight_col;
    int numrow;
    int numcol;
    bool is_reading;
    std::vector<std::string> selected_samples;
    std::vector<std::string> col_names; //fixme, initialize me!
    friend void print_vec(std::vector<std::string> vec);
public:
    int num_weights;
    std::vector<Row> rows;
    
    Weights() = default;
    
    bool check_input(std::string file_name);
    void read_weight_file(std::string weight_fn, double p_value = 1.00);

};

/* Free Helper Functions */

bool comparator(const Row lhs, const Row rhs);
void print_vec(std::vector<double> vec);
std::vector<long double> calculate_prs(long double weight, std::vector<long double> dosages);
std::vector<std::string> tokenize_header(std::string line, int vec_length = 0);
Row tokenize_row(std::string line, int num_weights, double p_thresh);

#endif /* Weights_h */
