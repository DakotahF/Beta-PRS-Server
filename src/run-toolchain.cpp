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
#include "weight-formatter/Options.h"
//#include <experimental/filesystem>   

//REQUIRES c++11
//namespace fs = std::experimental::filesystem;  

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

int main(int argc, char *argv[]) {
    std::string input_list, weight_list, output_dir; 
    int verbose_flag = 0; 
    int run_limit = 15; 
    int c;
    //struct direct *entry = nullptr; 
    //DIR *dp = nullptr; 

    while(1) 
    {
        static struct option long_options[] = 
        {
            {"verbose", no_argument, &verbose_flag, 1},
            {"data", required_argument, 0, 'd'},
            {"output", required_argument, 0, 'o'},
            {"weights", required_argument, 0, 'w'}, 
            {"run_limit", required_argument, 0, 'r'}
        };
        int option_index = 0; 
        c  = getopt_long(argc, argv, "d:o:w:r:", long_options, &option_index);
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
            case 'd':
                input_list = optarg;
                //dp = opendir(input_directory); 
            case 'o':
                output_dir = optarg;
            case 'w':
                weight_list = optarg;
            case 'r': 
                run_limit = atoi(optarg); 
            default:
                std::cerr << c << " not a valid argument\n"; 
                exit(0);
         }
    }
      
    /*
    //merge weights from weight-list.txt if not already generated
    //Populate and merge weights, FIXME: does not account for pgs-catalog weights or rivas-lab
    std::string weights_file, command;
    Weight merged_weights;
    num_files = 0; 
    try {
        command = "cat " + weights_file + " | wc -l";
        num_files = stoi(exec(command)) - 1; //account for header
        merge_weights(merged_weights, weight_list, num_files, verbose_flag);
        weights_file = output_dir + "/weight-list.txt";
        merged_weights.print_to_output(weights_file);
        //merged_weights.print_to_output("/Users/kotah/Desktop/projects/cpp-prs-tool/weight-formatter/merged-weights/merge-test.txt");
    }
    catch (const char* msg) {
        std::cerr <<  msg << '\n';
    }

    std::ifstream dosages(input_list.c_str()); 
    if (dosages.is_open()) {
        std::string dosage_file,response; 
        int count = 0; 
        while(getline(dosages, dosage_file) && count <= run_limit) {
            count = stoi(exec(jobnum_cmd));
            std::cout << count << '\n'; 
            while(count > run_limit) {
                usleep(600); 
                count = system("squeue -u kotah | wc -l");
            }
            //command = "sbatch ./a.out --output_dir " + output_dir + " --dosage_file " +  dosage_file + "--weight_file " + weights_file;  
            //response = system(command.c_str()); 
        } 
    }
    if (dp != nullptr) {
        while((entry = readdir(dp)))
            printf(entry->d_name); 
    }

    for (auto& p: fs::directory_iterator(input_directory)) { 
        std::cout << p.path() << '\n'; 
    }*/
} 
