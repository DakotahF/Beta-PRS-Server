#include <string>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <cassert> 
#include <unordered_map>
#include <getopt.h>
#include <stdio.h>
#include "Options.h"

using namespace std;


int main(int argc, char* argv[]) 
{
    int c;
    int verbose_flag = 0;
    char *file = NULL;
    int num_pheno = 0;
    bool pgs_catalog = false;
    int num_files = 0;
    vector<std::string> phenotypes;
    std::string phenotype_list, output_file;
    
    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            {"verbose", no_argument,       &verbose_flag, 1},
            /* These options don’t set a flag.
             We distinguish them by their indices. */
            {"output_file", required_argument,  0, 'o'},
            {"file",     required_argument,       0, 'f'},
            {"num_pheno",  required_argument,       0, 'n'}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "f:n:o:",
                         long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
            case 'f':
                try {
                phenotype_list = optarg;
                if (phenotype_list.empty())
                    throw("Your weight list file cannot be empty. Please see README for formatting instructions.\n");
                }
                catch(const char* msg) {
                    std::cerr << msg << '\n';
                }
                if(verbose_flag)
                    std::cout << "User-inputted phenotype list : " << phenotype_list << '\n';
                break;
            case 'n':
                try {
                    num_files = atoi(optarg);
                    if (num_files == 0) {
                        throw("num_files argument is required and must be of type integer. \n");
                    }
                }
                catch(const char* msg) {
                    std::cerr << msg << '\n';
                }
                break;
            case 'o':
                output_file = optarg;
                break;
            default:
                exit(0);
        }
    }

    //Populate and merge weights, FIXME: does not account for pgs-catalog weights or rivas-lab
    Weight merged_weights;  //fixme, initialize with output file
    try {
        merge_weights(merged_weights, phenotype_list, num_files, verbose_flag);
        merged_weights.print_to_output(output_file);
        //merged_weights.print_to_output("/Users/kotah/Desktop/projects/cpp-prs-tool/weight-formatter/merged-weights/merge-test.txt");
    }
    catch (const char* msg) {
        std::cerr <<  msg << '\n';
    }
}
