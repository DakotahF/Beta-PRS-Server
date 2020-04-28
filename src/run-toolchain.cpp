#include <iostream>
#include <fstream> 
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
//#include "Helpers.h" 
#include "weight-formatter/Options.h"   

//REQUIRES c++11  

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
        std::cout << "weights_file : " << weight_list << '\n'; 
        command = "cat " + weight_list + " | wc -l";
        num_files = stoi(exec(command)) - 1; //account for header
        std::cout << "num weight files to merge : " << num_files << '\n'; 
        merge_weights(merged_weights, weight_list, num_files, verbose_flag);
        std::cout << "merged weights -> SUCCES!\n";
        weights_file = output_dir + "/merged-weights.txt";
        std::cout << "beginning print_to_output\n"; 
        merged_weights.print_to_output(weights_file);
     }
     catch (const char* msg) {
         std::cerr <<  msg << '\n';
     }
     return weights_file; 
}

std::string submit_jobs(std::string dosages_list, std::string weight_file, std::string output_dir, int verbose_flag,std::string ref_panel="/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.id-sp.panel", int ancestry_flag=1, int run_limit=30) { 
    std::string response, dosage;
    std::string command; 
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
    int current_jobs = 0; 
    while(getline(in, dosage)) {
        while(current_jobs > run_limit) {
            std::this_thread::sleep_for(std::chrono::seconds(180));
            current_jobs = system("squeue -u kotah | wc -l");
            std::cout << current_jobs << '\n'; 
        }
        if (ancestry_flag) { 
            command = "sbatch ./ancestry/prep-study.txt " + dosage; //must include full path
            response = exec(command);
            std::cout << "ancestry response : " << response << "for doage file " << dosage <<'\n'; 
        }
        command = "sbatch ./app --output " + output_dir +  " --dosage " + dosage + "--weight " + weight_file + " --verbose";
        std::cout << "prs response : " << response << "for dosage file " << dosage <<'\n';
        response = exec(command);
    }
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
        weight_file = weight_list; //if single weight or pre-merged, set input weight fiel equal to weight_list variable 
    }


    //submit jobs for ancestry-calculation if requested 
    std::string response, merged_file; 
    response = submit_jobs(dosages_list, weight_file, output_dir, verbose_flag, ref_panel, ancestry_flag); //fixme, add a way to detect errors or slurm-step memkill errors
/*    while(curr_running > initial_running) {
        sys.sleep(600); 
    } */ 
    
    //exec("sbatch run-pca.txt --merged_file " + merged_file); //fixme, this needs to be moved to run in a chronological fashion 

    std::cout << "Finished\n"; 
} 
