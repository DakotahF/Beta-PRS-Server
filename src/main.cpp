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

void print_vec(std::vector<double> vec)
{ //fixme, remove me when cleaning or template and place with other helper functions
   if(vec.size() == 0) 
	return; 
   for(const auto &i:vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    return;
}

void print_vec_string(std::vector<std::string> vec, int max)
{ //fixme, remove me when cleaning or template and place with other helper functions
   if(vec.size() == 0)
        return; 
   int counter = 0; 
   for(const auto &i:vec) {
        std::cout << i << ' ';
	if(counter > max)
	    break;
	counter++;
    }
    std::cout << '\n';
    return;
}

template <typename T> 
void populate_ofile(std::string file_name,std::vector<T> data){
    std::ofstream txt_out; 
    txt_out.open(file_name,std::fstream::app);
    int len = data.size(); 
    int counter = 0; 
    for(const auto &i:data) { 
    	txt_out << i; 
        if(counter != (len - 1))
	    txt_out << '\t';
        counter++;
    }
    txt_out << '\n';
    txt_out.close();
    return; 
}

void print_prs(std::string file_name, Individual_scores indivs) { 
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
    std::string weight, dosage, out;
    int verbose_flag = 0;
    int c;  
    while(1) 
    {
        static struct option long_options[] =  
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"weight", required_argument, 0, 'w'},
            {"dosage", required_argument, 0, 'd'},
            {"output", required_argument, 0, 'o'}
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
            case 'w':
                weight = optarg; 
            case 'd':
                dosage = optarg; 
            case 'o': 
                out = optarg; 
            default: 
                exit(0);  
        }
    }
    //std::string filename = "/net/hunt/home/kotah/prs-server-beta/plink-weights/raw/PRS_weights_PheCode_X626.8_SAIGE_b38_5e-2_clumped_r2_0.2.chr16.clumped.cleaned.sorted";
    std::string filename = "/net/hunt/home/kotah/prs-server-beta/plink-weights/raw/PRS_weights_PheCode_X626.8_SAIGE_b38_5e-2_clumped_r2_0.2.clumped.cleaned.head10000.sorted";
    //std::string genotype_f = "/net/hunt/home/kotah/prs-server-beta/imputation/imp-server-output/chr16.dose.vcf.gz";
    //std::string genotype_f = "/net/fantasia/home/sarahgra/CHIP_data_GLGC/1KGP3_imputation_results_EUR/new_1KGP3_imputation_results/chr16.dose.vcf.gz";
    std::string genotype_f  = "/net/hunt/home/kotah/prs-server-beta/imputation/imp-server-output/chr16.dose.head10000.vcf.gz"; 
    //std::string genotype_f = "/net/hunt/home/kotah/prs-server-beta/doses/chr16/HUNT_chr16_chunk10_dosages_pruned.dose";
    std::string output_file = "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests/sample-results/imp-server-chr16-scores-X626.8-head10000both-ALLELESWAP.txt";
    unsigned int iid_count,pos,var;
    std::vector<double> dosages;
    std::string chr,ea,oa; 
    Genotypes genotypes(genotype_f);
    std::vector<std::string> indiv_iids; 
    std::cout << "Read in genotypes" << '\n';
    Weights weights;
    weights.read_weight_file(filename);
    std::cout << "Read in weight_file" << '\n';
    std::vector<std::string> samples;
    genotypes.open(samples);
    iid_count = genotypes.get_selected_samples().size();
    Individual_scores scores(weights.num_weights, iid_count);
    scores.IIDs = genotypes.get_selected_samples();
    //indiv_iids = genotypes.get_selected_samples();
    //populate_ofile(output_file,indiv_iids);  
    //Individual_scores scores(weights.num_weights, iid_count);

    var = 0;
    for(Row w_row : weights.rows){
        //print_vec(w_row.weights);
        ea = w_row.alt;
        oa = w_row.ref;
        chr = w_row.chr;
        pos = w_row.pos;
	//std::cout << chr << ":" << pos << '\n';
	if(var % 10000 == 0) 
            std::cout << "Processed " << var << " rows from weight_file" << '\n';
        int it = 0;
        //arma::vec dosage =  genotypes.read_variant(chr,pos,ea,oa); //fixme, uncomment me to test!
        dosages = genotypes.read_variant(chr,pos,ea,oa);
	//dosages.print();
	//print_vec(dosages);
        var++;
        if(dosages.size() == 0)
            continue;
        //std::transform(w_row->weights.begin(),w_row->weights.end(),calculate_prs);
        double weight = 0.00;
        while(it < weights.num_weights){
            std::vector<double> prs_vec;
            weight = w_row.weights[it];
            prs_vec = calculate_prs(weight,dosages);
            std::transform(scores.scores[it].begin(),scores.scores[it].end(),prs_vec.begin(),scores.scores[it].begin(), std::plus<double>());
            //populate_ofile(output_file,scores.scores[it]);
            it++;
        }
    }
   // print_vec(scores.scores[0]);
   print_prs(output_file, scores); 
}
