/*
CS-11
Purpose :

Incorporated weight file formatting : 

plink-order:  SNP    CHR    POS    Allele1    Allele2    BETA    p.value
------------------------------------------------------------------------
PGS-order:  rsID    chr_name    chr_position    effect_allele    reference_allele    effect_weight
---------------------------------------------------------------------------------------------------
necessary-order: CHROM    POS    OA    EA    PVALUE    WEIGHT_X    WEIGHT_X ... 
--------------------------------------------------------------------------------
*these are subject to change with integration of the standardized PGS API (--src--)
*/
#include "Options.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <sstream>
#include <string>
#include <tuple>

//Helper Functions  
std::string clean_id(std::string snp_id) {
    if (snp_id.length() <= 0) {
        std::cerr << "Error, snp_id of length 0" << '\n';
        exit(1);
    }
    if(snp_id[0] == 'c')
        return snp_id.erase(0,3);
    return snp_id;
}


Weight::Weight(std::string o_file): output_file(o_file) {};


tuple<std::string, double, double> Weight::process_standard(std::stringstream &ss) {
    std::string snp_id, val;
    double weight, p_value;
    int idx = 0;
    while(getline(ss,val,'\t'))
    {
        switch(idx) {
            case 0: snp_id = clean_id(val); break;
            case 5: weight = stod(val); break;
            case 6: p_value = stod(val); break;
            default: break;
        }
        idx++;
    }
    tuple<std::string, double, double> var_info(snp_id, weight, p_value);
    return var_info;
}

tuple<std::string, double, double> Weight::process_pgs(std::stringstream &ss) {
    std::string chr, pos, ref, alt;
    std::string snp_id, val;
    double weight = 0.0;
    double p_value = 0.0;
    int idx = 0;
    while(getline(ss,val,'\t'))
    {
        switch(idx) {
            case 1:
                chr = clean_id(val);
                snp_id += chr + ':';
                break;
            case 2: snp_id += val + ':'; break;
            case 3: snp_id += val + ':'; break;
            case 4: snp_id += val; break;
            case 5: weight = stod(val); break;
            default:  break;
        }
        idx++;
    }
    tuple<std::string, double, double> var_info(snp_id, weight, p_value);
    return var_info;
}

void Weight::populate_dict(int phenotype_idx, std::string w_file, int phenotype_count, int format_type, int verbose_flag) {
    std::ifstream weight_file(w_file.c_str());
    bool header=true;
    if (weight_file.is_open()) {
        if (verbose_flag)
            std::cout << "weight file " << w_file << " is open\n";
        std::string line;
        while(getline(weight_file, line)) {
            if(line.empty())
                continue;
            if (line[0] == '#')
                continue;
            if (header) {
                header = false;
                continue;
            }
            std::stringstream ss(line);
            std::string snp_id,val;
            double p_value, weight;
            std::tuple<std::string, double,double> var_info;
            
            //check if weight file is formatted for pgs-catalog or standard input, format elaborated in README
            if (format_type)
                var_info = process_pgs(ss);
            else
                var_info = process_standard(ss);
            
            snp_id = get<0>(var_info);
            weight = get<1>(var_info);
            p_value = get<2>(var_info);
            
            
            if (snp_id.empty()) // FIXME, add more testing/checking for poorly-formatted weights/p-values
                throw("Weight file " + w_file + " not properly formatted, please see README for proper formatting instructions.");
            if (this->var_weights.find(snp_id) == this->var_weights.end())
            {
                Variant variant(phenotype_count, snp_id);
                var_weights[snp_id] = variant;
            }
            
            var_weights[snp_id].p_values[phenotype_idx] = p_value;
            var_weights[snp_id].weights[phenotype_idx] = weight;
        }
    }
}

/*void Weight::print_weights(int num_rows) { //for development only 
    if(num_rows < 0)
        num_rows = this->var_weights.size(); 
    Variant variant_temp;
    int idx = 0;
    for(auto it : this->var_weights) {
        variant_temp = it.second;
        variant_temp.print_variant();
        idx++;
        if (idx == num_rows)
            return;
    }
    return;
}*/

void Weight::print_to_output(std::string filename) {
    std::ofstream txt_out;
    txt_out.open(filename);
    if (!txt_out.is_open()) {
        throw("Output file not able to be opened, check the file path.");
    }
    txt_out << "CHROM\tPOS\tOA\tEA\tPVALUE\t";
    int num_phenotypes = this->phenotypes.size();
    for(int idx=0; idx < this->phenotypes.size(); idx++){
        txt_out << "WEIGHT_" << this->phenotypes[idx];
        if (idx != (num_phenotypes-1))
            txt_out << '\t';
        else
            txt_out << '\n';
    }
    Variant variant_temp;
    for(auto it : this->var_weights) {
        variant_temp = it.second;
        variant_temp.print_line(txt_out); 
    }
}

void merge_weights(Weight &merged_weights, std::string phenotype_list, int phenotype_count, int verbose_flag) {
    merged_weights.phenotypes.resize(phenotype_count);
    std::ifstream phen_list(phenotype_list.c_str());
    if(!phen_list) 
        std::cerr << "Error, invalid phenotype_list file\n"; 
    std::string phenotype,phen_location;
    if (phen_list.is_open()) {
        if (verbose_flag)
            std::cout << "Iterating through phenotype weights\n";
        std::string line;
        int line_num = 0;
        int format_type = 0;
        bool header = true;
        while(getline(phen_list, line)) {
            if(header) {
                header = false;
                continue;
            }
            std::stringstream ss(line);
            std::string val;
            int idx = 0;
            while(getline(ss,val,'\t')){ 
                switch(idx) 
                {
                    case 0: phenotype=val; break;
                    case 1: phen_location=val; break;
                    case 2: format_type=stoi(val); break;
                    default:
                        std::cerr << "Check weight list formatting\n"; //FIXME, improve error catching
                        break; 
                }
                idx++;
            }
            if (phenotype.empty() || phen_location.empty())
                throw("Null phenotype and/or null phenotype weight location found in file. Ensure that values are tab-separated. Check README for more information.");
            try {
                merged_weights.populate_dict(line_num, phen_location,phenotype_count, format_type, verbose_flag);
                if (merged_weights.phenotypes.size() < line_num)
                    throw("User-provided phenotype number does not match actual number of phenotype weights provided in list file.");
                merged_weights.phenotypes[line_num] = phenotype;
            }
            catch(const char* msg) {
                throw(msg);
            }
            line_num++;
        }
    }
    else
        throw("Phenotype list file not valid, check path");
    phen_list.close();
    return;
}

