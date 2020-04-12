#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "Weights.h"
#include "Individuals.h"


/*
 Row constructor/method definitions
 */

Row::Row(std::string c, int p, std::string r, std::string a, double p_val, std::vector<double> w_vec)
: chr(c), pos(p), ref(r), alt(a), p_value(p_val), weights(w_vec),is_empty(0) {}

/*
 Weight constructor/method definitions
 */

bool Weights::check_input(std::string file_name)
{
    std::ifstream file;
    std::cout << "input sanitation not yet implemented" << '\n';
    return 0;
}

void Weights::read_weight_file(std::string weight_fn)
{
    std::ifstream infile(weight_fn.c_str());
    int row_idx = 0;
    std::vector<std::string> col_names;
    if (infile.is_open()) { //fixme, error checking
        std::string line;
        while(getline(infile, line)) {
            std::stringstream ss(line);
            std::string val;
            std::vector<double> weights;
            std::vector<std::string> line_split;
            if(row_idx == 0) {
                line_split = tokenize_header(line);
                this->col_names = line_split;
                this->num_weights = this->col_names.size() - 5; //5==# non-weight columns
                std::cout << "this->num_weights : " << this->num_weights << '\n'; 
                weights.resize(this->num_weights);
            }
            else {
                Row row;
                row = tokenize_row(line,this->num_weights, 1.00); //fixme, p-value should not be hardcoded
                if(!row.is_empty)
                    this->rows.push_back(row);
                //sort(this->rows.begin(),this->rows.end(),&comparator);
            }
            row_idx += 1;
        }
    }
    else {
        throw("Error, weight_file was not able to be opened, check path\n"); 
    }
    infile.close(); 
    sort(this->rows.begin(),this->rows.end(),&comparator);
    //std::cout << this->rows.size() << '\n';
    return;
}

/* Free Helper Function Definitions */

bool comparator(const Row lhs, const Row rhs)
{
    if(lhs.chr == rhs.chr) //does not currently allow for duplicate chr:pos...is this a problem?
        return lhs.pos < rhs.pos;
    if(lhs.chr == "X")
	return 0;
    if(rhs.chr == "X")
	return 1;
    return std::stoi(lhs.chr) < std::stoi(rhs.chr);
}

/*void print_vec(std::vector<double> vec)
{ //fixme, this helper can ONLY print std::vectors of type <std::string>
    for(auto i = vec.begin(); i != vec.end(); ++i) {
        std::cout << *i << ' ';
    }
    std::cout << '\n';
    return;
}*/

std::vector<double> calculate_prs(double weight, std::vector<double> dosages)
{
    //fixme,  add a check to skip zero values
    std::vector<double> prs_vec;
    prs_vec.reserve(dosages.size());
    for(auto& dosage : dosages) {
        double prs = 0.0;
        if(weight < 0)
            dosage -= 2.0;
        prs =  weight * dosage;
        prs_vec.push_back(prs);
    }
    return prs_vec;
}


std::vector<std::string> tokenize_header(std::string line, int vec_length)
{
    std::vector<std::string> value_vec;
    if (vec_length > 0)
        value_vec.reserve(vec_length);
    else 
        throw("Invalid header\n"); 

    std::stringstream ss(line);
    std::string val;
    while(getline(ss,val,'\t')) {
        value_vec.push_back(val);
    }
    return value_vec;
}


Row tokenize_row(std::string line, int num_weights, double p_thresh)
{
    if(!num_weights ||  num_weights == 0) 
	throw("no weights included\n");
    int pos = -1; 
    std::string chr,ref,alt;
    double p_val = -1;
    std::vector<double> weights;
    weights.reserve(num_weights);
    std::string ea,oa,val;
    int idx = 0;
    std::stringstream ss(line);
    
    while(getline(ss,val,'\t')) {
        switch (idx) //fixme, is this bad practice? Hacky?
        {
            case 0: chr=val; break;
            case 1: pos=std::stoi(val); break;
            case 2: ref=val; break;
            case 3: alt=val; break;
            case 4: p_val=std::atof(val.c_str()); break;
            default:
                double weight;
                weight=std::atof(val.c_str());
                weights.push_back(weight);
        }
        idx += 1;
    }
    if(chr.empty() || (pos == -1) || ref.empty() || alt.empty() || (p_val == -1))
        throw("All fields must be present in each line, refer to README for formatting\n") 
    if(p_val < p_thresh)
        return Row();
    Row row(chr,pos,ref,alt,p_val,weights);
    return row;
}

