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

Row::Row(std::string c, int p, std::string r, std::string a, double p_val, std::vector<long double> w_vec)
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

void Weights::read_weight_file(std::string weight_fn, double p_value)
{
    std::ifstream infile(weight_fn.c_str());
    int row_idx = 0;
    std::vector<std::string> col_names;
    if (infile.is_open()) {
        std::string line;
        while(getline(infile, line)) {
            std::stringstream ss(line);
            std::string val;
            std::vector<long double> weights;
            std::vector<std::string> line_split;
            if(row_idx == 0) {
                line_split = tokenize_header(line);
                this->col_names = line_split;
                this->num_weights = this->col_names.size() - 5;
                weights.resize(this->num_weights);
            }
            else {
                Row row;
                row = tokenize_row(line,this->num_weights, p_value); //fixme, hard-coded pvalue
                if(!row.is_empty)
                    this->rows.push_back(row);
            }
            row_idx += 1;
        }
    }
    else {
        throw("Error, weight_file was not able to be opened, check path\n"); 
    }
    infile.close(); 
    sort(this->rows.begin(),this->rows.end(),&comparator);
    return;
}

/* Free Helper Function Definitions */

bool comparator(const Row lhs, const Row rhs)
{
    if(lhs.chr == rhs.chr) //Assess for duplicates
        return lhs.pos < rhs.pos;
    if(lhs.chr == "X")
	return 0;
    if(rhs.chr == "X")
	return 1;
    return std::stoi(lhs.chr) < std::stoi(rhs.chr);  
}

std::vector<long double> calculate_prs(long double weight, std::vector<long double> dosages)
{
    std::vector<long double> prs_vec;
    prs_vec.reserve(dosages.size());
    for(auto dosage : dosages) {
        double prs = 0.00;
        if(weight < 0.00) { 
            dosage -= 2.0;
        }
        prs =  weight * dosage;
        prs_vec.push_back(prs);
    }
    return prs_vec;
}


std::vector<std::string> tokenize_header(std::string line, int vec_length)
{
    std::vector<std::string> value_vec;
    value_vec.reserve(vec_length);

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
    std::vector<long double> weights;
    weights.reserve(num_weights);
    std::string ea,oa,val;
    int idx = 0;
    std::stringstream ss(line);
    
    while(getline(ss,val,'\t')) {
        switch (idx)
        {
            case 0: chr=val; break;
            case 1: pos=std::stoi(val); break;
            case 2: ref=val; break;
            case 3: alt=val; break;
            case 4: p_val=std::stod(val); break;
            default:
                long double weight;
                weight=std::stod(val);
                weights.push_back(weight);
        }
        idx += 1;
    }
    if(chr.empty() || (pos == -1) || ref.empty() || alt.empty() || (p_val == -1))
        throw("All fields must be present in each line, refer to README for formatting\n"); 
    if(p_val > p_thresh) {
        return Row();
    }
    Row row(chr,pos,ref,alt,p_val,weights);
    return row;
}

