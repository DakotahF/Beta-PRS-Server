//#include savvy
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Weights.h"
#include "Individuals.h"
#include "Genotypes.h"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/reader.hpp"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/armadillo_vector.hpp"
#include <getopt.h>
#include <stdio.h>

void populate_ofile(std::string file_name, Individual_scores indivs) { 
     std::ofstream txt_out; 
     txt_out.open(file_name);
     for(int idx=0; idx < indivs.IIDs.size(); idx++) {
         txt_out << indivs.IIDs[idx] << '\t';
         int counter = 1; 
         for(int w_idx=0; w_idx < indivs.scores.size(); w_idx++) { 
             txt_out << indivs.scores[w_idx][idx]; 
	     if(counter != indivs.scores.size()) 
                 txt_out << '\t';
             counter++; 
         }
         txt_out << '\n'; //fixme, will a newline at the end cause issues? 
     }
     txt_out.close();
     return;
}


int main(int argc, char* argv[]) {
    std::string weight_file, dosage_file, output_file;
    int verbose_flag = 0;
    int c;  

    while(1) 
    {
        static struct option long_options[] =  
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"weight", required_argument,	0, 'w'},
            {"dosage", required_argument,	0, 'd'},
            {"output", required_argument,	0, 'o'}
        };

        int option_index = 0; 
        c  = getopt_long(argc, argv, "w:d:o:", long_options, &option_index); 

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
                std::cout << weight_file << '\n';
                break;  
            case 'd':
                dosage_file = optarg; 
                std::cout << dosage_file << '\n';
                break;  
            case 'o': 
                output_file = optarg; 
                std::cout << output_file << '\n';
                break;  
            default: 
                std::cout << "enters default and exits \n"; 
                exit(0);  
        }
    }
    if (weight_file.empty() || dosage_file.empty() || output_file.empty()) 
    {
        std::cerr << "Weight, dosage, and output file arguments cannot be empty\n"; 
        exit(0); 
    }
    unsigned int iid_count,pos,var;
    std::vector<double> dosages;
    std::string chr,ea,oa; 
    try { 
        Genotypes genotypes(dosage_file);
        std::vector<std::string> indiv_iids;
        if (verbose_flag)  
    	    std::cout << "Read in genotypes" << '\n';
        Weights weights;
        weights.read_weight_file(weight_file);
        if (verbose_flag) 
    	    std::cout << "Read in weight_file" << '\n';
        std::vector<std::string> samples;
        genotypes.open(samples);
        iid_count = genotypes.get_selected_samples().size();
        Individual_scores scores(weights.num_weights, iid_count);
        scores.IIDs = genotypes.get_selected_samples();
    } 
    catch(const char* msg) { 
        std::cerr << msg << '\n';
        exit(1);  
    } 

    var = 0;
    for(Row w_row : weights.rows){
        ea = w_row.alt;
        oa = w_row.ref;
        chr = w_row.chr;
        pos = w_row.pos;

	if(var % 10000 == 0) 
            std::cout << "Processed " << var << " rows from weight_file" << '\n';
        int it = 0;
        dosages = genotypes.read_variant(chr,pos,ea,oa);
        var++;
        if(dosages.size() == 0)
            continue;
        double weight = 0.00;
        while(it < weights.num_weights){
            std::vector<double> prs_vec;
            weight = w_row.weights[it];
            try { 
            	prs_vec = calculate_prs(weight,dosages);
            } 
            catch(const char* msg) {
                std::cerr << msg << '\n'; 
                exit(1); 
            } 
            std::transform(scores.scores[it].begin(),scores.scores[it].end(),prs_vec.begin(),scores.scores[it].begin(), std::plus<double>());
            it++;
        }
    }
   if (verbose_flag)
        std::cout << "Printing results to " << output_file << '\n'; 
   populate_ofile(output_file, scores); 
}
