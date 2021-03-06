#include <iostream>
#include <fstream> 
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include "weight-formatter/Options.h"  
#include "Helpers.h"

//using namespace std; 
//basic helper functions

// trim from start
static std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

// splits input string by delimiter and returns item a position of interest
std::string tokenize(std::string text, char delim, int pos) {
    std::stringstream ss(trim(text)); 
    std::string val; 
    std::vector<std::string> tokens; 
    while(getline(ss, val, delim)) {
        tokens.push_back(val);
    }
    if(!tokens.empty() && (pos < tokens.size()) && pos > 0)  
        return tokens[pos]; 
    if(pos == -1) 
        return tokens.back();
    return ""; 
}

//executes system commands
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

//merges phenotypes and returns merged weight file
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

//formats command for ancestry pre-processing 
std::string prep_ancestry_command(std::string dosage, std::string outputdir, std::string ref_dir) { 
    std::string body,command,ukbbsnps;
    ukbbsnps = ref_dir + "/ancestry-snps.ids"; //FIXME, HARDCODED
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
                      --out " + qcdir + '/' + foldername + '/' + foldername + ".filtered" + '\n' +
                      "cat " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in >> " + 
                        resultdir + "/study-snps.ids\n" + 
                "fi\n" + 
                "if [ ! -f " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned ]; then\n" +  
                "      plink-1.9 --bfile " + outputdir + '/' + foldername + '/' + foldername +
                      " --extract " + qcdir + '/' + foldername + '/' + foldername + ".filtered.prune.in\
                      --make-bed\ 
                      --out " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned\n" + 
                "fi\n" + '\n' + 
                "echo " + qcdir + '/' + foldername + '/' + foldername + ".filtered.pruned >> " + resultdir + "/cleaned_sample_list.txt\n";
    command = std::string("sbatch --job-name=prune-and-filter --mem=60G --wrap=\"") + body + "\"";
    std::cout << command << '\n'; //removeme
    return command; 
}

//submits jobs for ancestry and prs-calculations
std::string submit_jobs(std::string dosages_list, std::string weight_file, std::string output_dir, int verbose_flag, std::string ref_panel, std::string ref_data, std::string ref_dir, int ancestry_flag, int run_limit) { 
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
    std::ifstream in(dosages_list);
    if(!in) {
        std::cerr << "Input " + dosages_list << " filepath invalid\n";
        exit(1);  
    }
    std::string filename; 
    int start_pos;
    std::string anc_id;
    while(getline(in, dosage)) {
        if (ancestry_flag) {
            command = prep_ancestry_command(dosage, output_dir, ref_dir);
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
        command = "sbatch --job-name=prs-calculation --wrap=\"./prs-calc --output " + score_file +  " --dosage " + dosage + " --weight " + weight_file;  
        std::cout << command << '\n'; //removeme
        if(verbose_flag) { 
            command += " --verbose\"";
            response = exec(command);
            std::cout << response << " submmitted for dosage file : " << dosage << '\n'; 
        }
        else { 
            command += "\""; 
            response = exec(command);
        }
    }
    command = "sbatch --dependency=afterok:" + anc_ids + " /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/src/ancestry/run-pca.txt " + resultdir + ' ' + ref_panel + ' ' + ref_data;
    response = exec(command); 
    return response; 
}

