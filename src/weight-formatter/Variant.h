#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Variant
{
public:
    //variables
    int phenotype_count;
    std::string variant_id;
    std::string rsid;
    std::vector<double> p_values;
    std::vector<double> weights;
    
    //constructor declaration
    Variant() = default;
    Variant(int phenotype_count, std::string variant_id);
    
    //functions
    void update_variant(int index, double p_value, double weight);
    //void print_variant();
    //void print_vector(vector<double> vec);
    vector<std::string> tokenize_variant_id(char delim = ':');
    void print_line(std::ofstream &output_file, bool PGS = false, char delim = ':'); 
};
