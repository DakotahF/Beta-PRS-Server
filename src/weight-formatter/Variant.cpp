#include "Variant.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> 

Variant::Variant(int phen_count, std::string snp_id): phenotype_count(phen_count), variant_id(snp_id) {
    this->p_values.resize(phenotype_count,-1.0);
    this->weights.resize(phenotype_count, 0.0);
};

void Variant::print_variant(){
    int idx = 0;
    std::cout << this->variant_id << '\n';
    std::cout << "WEIGHTS :" << '\n';
    this->print_vector(this->weights);
    std::cout << "P_VALUES :" << '\n';
    this->print_vector(this->p_values);
    return;
}


void Variant::print_vector(vector<double> vec) {
    if(vec.size() == 0)
        return;
    for(const auto &i:vec) {
        std::cout << i << '\t';
    }
    std::cout << '\n';
    return;
}

vector<std::string> Variant::tokenize_variant_id(char delim) {
    std::stringstream ss(this->variant_id);
    std::string val;
    vector<std::string> id_components;
    while(getline(ss,val,delim)) {
        id_components.push_back(val);
    }
    //std::cout << id_components[2] << id_components[3] << '\n';
    return id_components; 
}


void Variant::print_line(std::ofstream &output_file, bool PGS, char delim) {
    //plink-order SNP    CHR    POS    Allele1    Allele2    BETA    p.value
    //PGS-order rsID    chr_name    chr_position    effect_allele    reference_allele    effect_weight
    //necessary-order CHROM    POS    OA    EA    PVALUE    WEIGHT_
    vector<string> id_components;
    id_components = tokenize_variant_id(delim);
    int len = id_components.size();
    for (int idx = 0; idx < id_components.size(); idx++) {
        output_file << id_components[idx] << '\t';
    }
    
    double p_value = *max_element(this->p_values.begin(), this->p_values.end());
    len = this->weights.size();
    
    output_file << p_value << '\t';
    
    for (int idx = 0; idx < this->weights.size(); idx++) {
        if (idx != (len - 1))
            output_file << this->weights[idx] << '\t';
        else
            output_file << this->weights[idx] << '\n';
    }

    return;
}
