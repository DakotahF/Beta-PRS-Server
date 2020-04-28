#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <tuple>
#include "Variant.h"

/*class Options
{
public:
    //variables
    std::string file_path;
    std::unordered_map<std::string, std::string> weight_dict;
    bool header; 

    //constructor declaration
    Options(std::string file_path);
    //functions
    void populate_dict();
};
*/

using namespace std;

class Weight 
{
public: 
    //variables 
    std::unordered_map<std::string, Variant > var_weights;
    vector<std::string> phenotypes;
    std::string output_file;
    int phenotype_count;
    double p_val_threshold;
    
    //constructor declaration
    Weight() = default;
    Weight(std::string output_file);
    //functions
    tuple<std::string, double, double> process_pgs(std::stringstream &ss);
    tuple<std::string, double, double> process_standard(std::stringstream &ss);
    void populate_dict(int phenotype_idx, std::string weight_file, int phenotype_count, int format_type, int verbose_flag);
    void print_weights(int num_rows = -1);
    void print_to_output(std::string filename);
};

void merge_weights(Weight &merged_weights, std::string phenotype_list, int phenotype_count, int verbose_flag); 

