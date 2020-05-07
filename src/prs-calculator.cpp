#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Weights.h"
#include "Individuals.h"
#include "Genotypes.h"
#include <getopt.h>
#include <stdio.h>
#include <iomanip>

void populate_ofile(std::string file_name, Individual_scores indivs, std::vector<std::string> traits) { 
     std::ofstream txt_out; 
     txt_out.open(file_name);
     txt_out << "IID\t";
     for (auto idx=5; idx < traits.size(); idx++) { //starts at five to select only phenotype names, FIXME this should be changed elsewhere
         if (idx != (traits.size() - 1))
             txt_out << traits[idx] << '\t';
         else 
             txt_out << traits[idx] << '\n';
     }
     for(int idx=0; idx < indivs.IIDs.size(); idx++) {
         txt_out << indivs.IIDs[idx] << '\t';
         int counter = 1; 
         for(int w_idx=0; w_idx < indivs.scores.size(); w_idx++) { 
             txt_out << std::setprecision(16) << indivs.scores[w_idx][idx]; 
	     if(counter != indivs.scores.size()) 
                 txt_out << '\t';
             counter++; 
         }
         txt_out << '\n'; 
     }
     txt_out.close();
     return;
}


int main(int argc, char* argv[]) {
    std::string weight_file, dosage_file, output_file;
    double p_thresh = -1.00; 
    int verbose_flag = 0;
    int c;  

    while(1) 
    {
        static struct option long_options[] =  
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"weight", required_argument,	0, 'w'},
            {"dosage", required_argument,	0, 'd'},
            {"output", required_argument,	0, 'o'},
            {"pthresh", required_argument,      0, 'p'}
        };
 
        int option_index = 0; 
        c  = getopt_long(argc, argv, "w:d:o:p:", long_options, &option_index); 

        if (c == -1) 
            break; 
        
        switch(c) 
        {
            case 0: 
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if(optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break; 
            case 'w':
                weight_file = optarg;
                break;  
            case 'd':
                dosage_file = optarg; 
                break;  
            case 'o': 
                output_file = optarg; 
                break;
            case 'p':
                p_thresh = std::stod(optarg); 
                break;
            default: 
                std::cout << "enters default and exits \n"; 
                exit(0);
                break;   
        }
    }
    if (weight_file.empty() || dosage_file.empty() || output_file.empty()) 
    {
        std::cerr << "Weight, dosage, and output file arguments cannot be empty\n"; 
        exit(0); 
    }
    unsigned int iid_count,pos,var;
    std::vector<long double> dosages;
    std::string chr,ea,oa; 
    try 
    { 
        std::cout << "Enters main prs calculation successfully, reading in genotypes from " << dosage_file << " with verbose flag = " << verbose_flag << '\n'; 
        Genotypes genotypes(dosage_file);
        std::vector<std::string> indiv_iids;
        if (verbose_flag)  
    	    std::cout << "Read in genotypes" << '\n';
        Weights weights;
        if (p_thresh == -1.00)   
            weights.read_weight_file(weight_file);
        else
            weights.read_weight_file(weight_file, p_thresh);
        if (verbose_flag) 
    	    std::cout << "Read in weight_file" << '\n';
        std::vector<std::string> samples;
        genotypes.open(samples);
        iid_count = genotypes.get_selected_samples().size();
        Individual_scores scores(weights.num_weights, iid_count);
        scores.IIDs = genotypes.get_selected_samples();
        std::cout.flush(); 
        var = 0; 
        for(Row w_row : weights.rows){
            ea = w_row.alt;
            oa = w_row.ref;
            chr = w_row.chr;
            pos = w_row.pos;

            if(var % 10000 == 0) {
                std::cout << "Processed " << var << " rows from weight_file" << '\n';
                std::cout.flush(); 
            }
            int it = 0;
            dosages = genotypes.read_variant(chr,pos,ea,oa);
            var++;
            if(dosages.empty()) {
                continue;
            }
            double weight = 0.00;
            while(it < weights.num_weights){
                std::vector<long double> prs_vec;
                weight = w_row.weights[it];
                if (weight == 0) {
                    it++;
                    continue; 
                }
                prs_vec = calculate_prs(weight,dosages); 
                std::transform(scores.scores[it].begin(),scores.scores[it].end(),prs_vec.begin(),scores.scores[it].begin(), std::plus<double>());
                it++;
            }
        }
       if (verbose_flag)
            std::cout << "Printing results to " << output_file << '\n'; 
       populate_ofile(output_file, scores, weights.col_names);
    }
    catch(const char* msg) { 
        std::cerr << msg << '\n';
        exit(1);  
    } 
}
