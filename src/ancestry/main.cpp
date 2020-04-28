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
    if (sample_fn.empty() || reference_fn.empty() || out_fn.empty() || pcs == 0) 
    {
        std::cerr << "Weight, dosage, and output file arguments cannot be empty\n"; 
        exit(0); 
    }
    Ancestry_estimator anc_object(reference_fn, sample_fn, out_fn); 
    anc_object.calculate_centroids(pcs); 
    anc_object.print_ancestry(); 
}

/*int main() { 
   //string sample_fn = "/net/hunt/home/kotah/prs-server-beta/ancestry/job-0/MERGED2-PCA-15.eigenvec";
   //string sample_fn = "/net/hunt/home/kotah/prs-server-beta/ancestry/job-0/MERGED2-PCA-2.eigenvec";
   string sample_fn = "/net/hunt/home/kotah/prs-server-beta/ancestry/hunt-sample/1kg-chr1-MERGED-2PCA.eigenvec"; 
   string ref_fn = "/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.id-sp.panel";
   //string ref_fn = "/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.panel"; 
   string out_fn = "/net/hunt/home/kotah/prs-server-beta/ancestry/ancestry-tool/chr1-2pcs.txt";
   int pcs = 2; ; 
   Ancestry_estimator anc_object(ref_fn, sample_fn, out_fn);
   anc_object.calculate_centroids(pcs);
   anc_object.print_ancestry();
}*/
