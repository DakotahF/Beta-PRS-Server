/**
    CS-11, main.cpp
    Purpose: Calculate ancestry for sampleset given .eigenvec merged with 1000genomes
    * Currently only compatible with 1000 genomes reference panel found @ ________

    @author Dakotah Feil
    @version 1.1 05/01/20 
*/
#include <getopt.h>
#include <iostream> 
#include "Ancestry.h"

using namespace std; 

int main(int argc, char* argv[]) {
    std::string sample_fn, reference_fn, out_fn;
    int pcs = 0; 
    int verbose_flag = 0;
    int c;  

    while(1) 
    {
        static struct option long_options[] =  
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"sample", required_argument,	0, 's'},
            {"ref", required_argument,	0, 'r'},
            {"out", required_argument,	0, 'o'}, 
            {"pcs", required_argument, 0, 'p'}
        };

        int option_index = 0; 
        c  = getopt_long(argc, argv, "s:r:o:p:", long_options, &option_index); 

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
            case 's':
                sample_fn = optarg;
                break;  
            case 'r':
                reference_fn = optarg; 
                break;  
            case 'o': 
                out_fn = optarg; 
                break;
            case 'p': 
                pcs = std::stoi(optarg);
                break; 
            default:
                std::cout << "enters default and exits \n"; 
                exit(0);  
        }
    }
    std::cout << "sample_fn : " << sample_fn << '\n'; 
    std::cout << "reference_fn : " << reference_fn << '\n';
    std::cout << "out_fn : " << out_fn << '\n';
    if (sample_fn.empty() || reference_fn.empty() || out_fn.empty() || pcs == 0) 
    {
        std::cerr << "Sample, reference, and output file arguments cannot be empty\n"; 
        exit(0); 
    }
    Ancestry_estimator anc_object(reference_fn, sample_fn, out_fn); 
    anc_object.calculate_centroids(pcs); 
    anc_object.print_ancestry(); 
}

