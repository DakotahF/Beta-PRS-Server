#include <iostream>
#include <fstream> 
#include <algorithm>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <stdexcept>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>  
//#include "Helpers.h" 
#include "weight-formatter/Options.h"   

//REQUIRES c++11  

//helpers...must be moved
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

std::string tokenize(std::string text, char delim, int pos) {
    std::stringstream ss(trim(text)); 
    std::string val; 
    vector<std::string> tokens; 
    while(getline(ss, val, delim)) {
        tokens.push_back(val);
    }
    if(!tokens.empty() && (pos < tokens.size()) && pos > 0)  
        return tokens[pos]; 
    if(pos == -1) 
        return tokens.back();
    return ""; 
}

std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

std::string merge_traits(std::string weight_list, std::string output_dir, int verbose_flag) {
    std::string weights_file, command;
    Weight merged_weights;
    int num_files = 0;
    try {
        command = "cat " + weight_list + " | wc -l";
        num_files = stoi(exec(command)) - 1; //account for header
        merge_weights(merged_weights, weight_list, num_files, verbose_flag);
        weights_file = output_dir + "/merged-weights.txt";
        merged_weights.print_to_output(weights_file);
     }
     catch (const char* msg) {
         std::cerr <<  msg << '\n';
     }
     return weights_file; 
}

std::string prep_ancestry_command(std::string dosage, std::string outputdir) { 
    std::string body,command,ukbbsnps;
    ukbbsnps = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/ancestry-snps.ids";
    std::string ext = ".vcf.gz";
    std::string inputfile = dosage; 
    std::string name = tokenize(inputfile,'/',-1);
    int start_pos = name.find(ext); 
    name.erase(start_pos,ext.size()); 
    std::string foldername = name;
    std::string qcdir = outputdir + "/qcdir";
    std::string resultdir = outputdir + "/results";  
    body = "mkdir " + outputdir + '/' + foldername + '\n' +
                "if [ ! -f " + outputdir + '/' + foldername + '/' + foldername + ".bed ]; then\n" + 
                "     plink-1.9 --vcf " + inputfile + " --make-bed --out " + outputdir + '/' + foldername + '/' + foldername + '\n' +
                "fi\n" + 
                "mkdir " + qcdir  + '/' + foldername + '\n' +
                "if [ ! -f " + qcdir + '/' + foldername + '/' + foldername + ".filtered ]; then\n" +
                "     plink-1.9 --bfile " + outputdir + '/' + foldername + '/' + foldername +
                      " --extract " + ukbbsnps +
                      " --make-bed\
                      --out " + qcdir + '/' + foldername + '/' + foldername + ".filtered\n" +
                "fi\n" + 
                "if [ ! -f " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in ]; then\
                      plink-1.9 --bfile " + qcdir + '/' + foldername + '/' + foldername + ".filtered" + 
                      " --indep-pairwise 50 5 0.2\
                      --out " + qcdir + '/' + foldername + '/' + foldername + ".filtered" + '\n' + /*'\n' + 
                      "cat " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in >> " + 
                        resultdir + "/study-snps.ids" + */
                "fi\n" + 
                "cat " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in >> " + 
                        resultdir + "/study-snps.ids\n" +  
                "if [ ! -f " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned ]; then\n" +  
                "      plink-1.9 --bfile " + outputdir + '/' + foldername + '/' + foldername +
                      " --extract " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in\
                      --make-bed\ 
                      --out " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned\n" + 
                "fi\n" + '\n' + 
                "echo " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned >> " + resultdir + "/cleaned_sample_list.txt\n";
    command = std::string("sbatch --job-name=prune-and-filter --mem=60G --wrap=\"") + body + "\""; 
    std::cout << "COMMMMMMANNNNDDDD : " << command << '\n';
    return command; 
}

std::string submit_jobs(std::string dosages_list, std::string weight_file, std::string output_dir, int verbose_flag,std::string ref_panel="/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.id-sp.panel", std::string ref_data="/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1000genomes.pruned", int ancestry_flag=1, int run_limit=30) { 
    std::string resultdir = output_dir + "/results";
    std::string response, dosage, score_file,ext; 
    std::string command;
    std::string anc_ids; 
    ext = ".vcf.gz"; 
    if (ancestry_flag) { 
        command = "mkdir " +  output_dir + "/results";
        response = exec(command);
        command = "mkdir " + output_dir + "/qcdir";
        response = exec(command);
    }
    ifstream in(dosages_list);
    if(!in) {
        std::cerr << "Input " + dosages_list << " filepath invalid\n";
        exit(1);  
    }
    std::string filename; 
    int start_pos;
    std::string anc_id;
    while(getline(in, dosage)) {
        if (ancestry_flag) {
            //command = "sbatch ./ancestry/prep-study.txt " + dosage + " " + output_dir; //must include full path
            command = prep_ancestry_command(dosage, output_dir);
            response = exec(command);
            anc_id = tokenize(response,' ',-1);
            if(anc_ids.empty() && !anc_id.empty()) 
                anc_ids += anc_id; 
            else if (!anc_id.empty()) 
                anc_ids += ':' + anc_id; 
            else 
                std::cerr << "error submitting ancestry processing for dosage " <<  dosage << '\n';  
            std::cout << "ancestry response : " << response << "for dosage file " << dosage <<'\n'; 
        }
        filename = tokenize(dosage,'/',-1);
        start_pos = filename.find(ext); 
        filename.erase(start_pos, ext.size());
        score_file = output_dir + '/' + filename + ".txt";  
        std::cout <<  "score file for output : "  << score_file << '\n'; 
        command = "sbatch --job-name=prs-calculation --wrap=\"./prs-calc --output " + score_file +  " --dosage " + dosage + "--weight " + weight_file + " --verbose\""; //fixme, CURRENTLY VERBOSE MODE NO MATTER WHAT 
        std::cout << "prs response : " << response << "for dosage file " << dosage <<'\n';
        std::cout << "command : "  << command << '\n'; 
        response = exec(command);
    }
    std::cout << "ANC IDS " << anc_ids << '\n'; 
    command = "sbatch --dependency=afterok:" + anc_ids + " /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/ancestry/run-pca.txt " + resultdir + ' ' + ref_panel + ' ' + ref_data; //check argparsing
    response = exec(command); 
    std::cout << "PCA command (!!!!!) : " << command << '\n'; 
    return response; 
}

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
                std::cout << "dosages_list : " << dosages_list << '\n'; 
                //dp = opendir(input_directory);
                break; 
            case 'o':
                output_dir = optarg;
                std::cout << "output_dir : " << output_dir << '\n'; 
                break; 
            case 'w':
                weight_list = optarg;
                std::cout << "weight_list : " << weight_list << '\n'; 
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
    //Populate and merge weights, FIXME: does not account for pgs-catalog weights or rivas-lab
    std::string weight_file;  
    if (merge_flag) { 
        std::cout << "Merge_weights running\n";
        weight_file = merge_traits(weight_list, output_dir, verbose_flag);
    }
    else {
        std::cout << "Merge weights not initiated\n"; 
        weight_file = weight_list; //if single weight or pre-merged, set input weight fiel equal to weight_list variable FIXME : CHECK IF PROPERLY FORMATTED
    }


    //submit jobs for ancestry-calculation if requested 
    std::string response, merged_file; 
    std::string ref_data = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1000genomes.pruned"; 
    //ref_panel = "/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.id-sp.panel";
    ref_panel = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1KG-v3.ALL.id-sp.panel"; 
    trim(ref_data); 
    trim(ref_panel); 
    response = submit_jobs(dosages_list, weight_file, output_dir, verbose_flag, ref_panel, ref_data, ancestry_flag); //fixme, add a way to detect errors or slurm-step memkill errors
    std::cout << "Finished\n"; 
} 
