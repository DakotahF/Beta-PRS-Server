/*
    CS-11, main.cpp
    Purpose: Runs weight-merging commands and generates properly-formatted weight file, submits ancestry-processing jobs,             ancestry-calculation job, and prs-calculation jobs. Does not chunk dosages, so this may be done beforehand               as of right now. 

    @author Dakotah Feil
    @version 1.1 05/01/20 
*/
#include <iostream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include "Helpers.h" 
#include "weight-formatter/Options.h"   

//REQUIRES c++11  

int main(int argc, char *argv[]) {
    std::string dosages_list, weight_list, output_dir, ref_panel; 
    int verbose_flag = 0; 
    int merge_flag = 0; 
    int ancestry_flag = 0; 
    int run_limit = 15; 
    int c; 

    while(1) 
    {
        static struct option long_options[] = 
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"merge", no_argument, &merge_flag, 1},
            {"ancestry", no_argument, &ancestry_flag, 1},
            {"dosages", required_argument, 0, 'd'}, //requires dosage_list.txt
            {"output", required_argument, 0, 'o'},
            {"weights", required_argument, 0, 'w'}, 
            {"run_limit", required_argument, 0, 'r'},
            {"ref_panel",  required_argument, 0, 'p'}
        };
        int option_index = 0; 
        c  = getopt_long(argc, argv, "d:o:w:r:p:", long_options, &option_index);
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
            case 'd':
                dosages_list = optarg;
                break; 
            case 'o':
                output_dir = optarg;
                break; 
            case 'w':
                weight_list = optarg;
                break; 
            case 'r': 
                run_limit = atoi(optarg);
                break; 
            case 'p': 
                ref_panel = optarg;
                break;   
            default:
                std::cerr << c << " not a valid argument\n"; 
                exit(0);
         }
    }
    
    //merge weights from weight-list.txt if not already generated
    //Populate and merge weights, FIXME: does not account for rivas lab covid19 weights as of yet
    std::string weight_file;  
    if (merge_flag) { 
        if (verbose_flag)
            std::cout << "Merge_weights running\n";
        weight_file = merge_traits(weight_list, output_dir, verbose_flag);
    }
    else { 
        weight_file = weight_list; //if single weight or pre-merged, set input weight file equal to weight_list variable FIXME : CHECK IF PROPERLY FORMATTED and throw error if not 
    }
    
    if(verbose_flag) 
        std::cout << "Weight merging finished, weightfile at "  << weight_file << '\n'; 
    //submit jobs for ancestry-calculation if requested 
    std::string response, merged_file; 
    std::string ref_data = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1000genomes.pruned";
    ref_panel = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1KG-v3.ALL.id-sp.panel"; 

    //submit ancestry jobs as well as prs-calculation jobs
    response = submit_jobs(dosages_list, weight_file, output_dir, verbose_flag, ref_panel, ref_data, ancestry_flag);
    if(verbose_flag) { 
        std::cout << "Submission script finished, jobs  currently running. \ 
           Check directory " << "" << " for ancestry results.\n Check directory " << "" << " for prs results\n";
    }
} 
