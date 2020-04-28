#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono> 
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

/*void submit_jobs(std::string input_dir, std::string pattern, std::string output_dir) {
    int code, file_count = 0;
    std::string file_list; 
    command = "sbatch ancestry-runner.txt"; 
    code = stoi(exec(command)) - 1; 
    if (code != 0) 
        std::cerr << "Error with ancestry preparation\n";
    command = "wc -l " 
    return code; 
}*/

std::string submit_jobs() {
    int file_count = 0;
    std::string file_list, command, response;
    command = "sbatch anc-runner.txt";
    response = exec(command);
    /*if (response != 0)
        std::cerr << "Error with ancestry preparation\n";*/
    return response;
}

int main() { 
//testing purposes ONLY --> delete thereafter 
    int initial_jobs, post_jobs, time_limit; //fixme, take in time limit
    auto start = std::chrono::high_resolution_clock::now();
    std::string count_command = "squeue -u kotah | wc -l"; 
    initial_jobs = std::stoi(exec(count_command));
    std::cout << "initial job number : " << initial_jobs << '\n'; 
    exec("touch /net/hunt/home/kotah/prs-server-beta/ancestry/cran/results/cleaned_sample_list.txt"); 
    submit_jobs(); 
    post_jobs = std::stoi(exec(count_command));
    std::cout << "current job number : " << post_jobs << '\n'; 
    while(post_jobs > initial_jobs) { //depends on no additional background jobs 
        std::this_thread::sleep_for(std::chrono::seconds(180));
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        /*if(duration >= 54000) { //10 hour run limit 
            std::cerr << "Error, runtime exceeded\n";
            exit(1); 
        }*/ 
        post_jobs = std::stoi(exec(count_command)); 
    }
    exec("sbatch run-pca.txt"); //fixme : run with custom input 
}
