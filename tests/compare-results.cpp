#include <string> 
#include <unordered_map> 
#include <iostream>
#include <sstream> 
#include <fstream>  
#include <algorithm> 

using namespace std; 

void populate_map(unordered_map<std::string, string> &data, string reference, int id_col = 1, int sp_col = 9) {
     ifstream in(reference); 
     if(!in) {
         std::cerr << "invalid reference file path\n"; 
         exit(1);  
     }
     std::string line;
     bool header = true; 
     while(getline(in, line)) {
         if(header) { 
             header = false;
             continue; 
         }
         std::stringstream ss(line);
         std::string value, id, sp;  
         int col_num = 0;
 
         while (getline(ss, value, '\t')) {
             if(col_num == id_col) {
                 id = value;
                 //std::cout << id << '\n';
                 col_num++;  
                 continue; 
             }
             if(col_num == sp_col) {
                 sp = value; 
                 //std::cout << sp << '\n'; 
                 col_num++; 
                 continue; 
             }
             col_num++;
         }
         if (data.find(id) == data.end()) 
             data[id] = sp; 
         /*else
             std::cout << "error, multiple ids\n";*/
     } 
     return; 
}
 
void check_id(unordered_map<std::string, std::string> &ref_data, const string id, string superpop, vector<std::string> &missing_ids,  vector<std::string> &nonmatch_vals) { 
    if(ref_data.find(id) != ref_data.end()) {
        std::string sp_ref = ref_data[id]; 
        transform(sp_ref.begin(), sp_ref.end(), sp_ref.begin(), ::toupper);
        transform(superpop.begin(), superpop.end(), superpop.begin(), ::toupper);
        if(sp_ref != superpop) {
            if(sp_ref != "EUR" && superpop == "EUR") { 
                nonmatch_vals.push_back(id);
                std::cout << "ref superpop : " << sp_ref << " & sample_superpop : " << superpop << " do not match\n";
                //std::cout << id << '\n'; 
                return; 
            }
            if(sp_ref == "EUR" && superpop != "EUR") {
                nonmatch_vals.push_back(id); 
                std::cout << "ref superpop : " << sp_ref << " & sample_superpop : " << superpop << " do not match\n";
                return; 
            }
        }
    }
    else {
       missing_ids.push_back(id);  
    }
} 

int main(int argc, char * argv[]) {
    string reference = "/net/hunt/home/troychow/projects/CHIP_MGI_GWAS_after_26march2019/QC_pipeline/MGI_data/MGI_ancestry/study_pc_Cau_EUR.txt";
    unordered_map<string, string> ref_data;
    vector<std::string> nonmatch_vals; 
    vector<std::string> missing_ids;
    string sample = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/results/MERGED-NOAPPROX.ancestry.txt";
    //string sample = "/net/hunt/home/kotah/prs-server-beta/ancestry/cran/results/MERGED-3pcs.merged.ancestry.txt";
    int id_col, superpop_col; 
    populate_map(ref_data, reference);
    ifstream in(sample);
    bool header = true;  
    if(!in) 
        std::cerr << "invalid sample file path"; 
    std::string line;
    int line_count = 0; 
    while(getline(in, line)) {
      line_count++; 
      if (header) {
          header = false;
          continue;
      }
      string id,fid,superpop;
      std::stringstream ss(line); 
      std::string value;
      int col_num = 0;  
      while(ss >> value) 
      {
          if(col_num == 0) {//does not account for fid
              col_num++; 
              id = value;
              continue; 
          }
          if(col_num == 1) {
              col_num++; 
              superpop = value; 
              continue;  
          }
      }
      check_id(ref_data,id,superpop,missing_ids,nonmatch_vals);
    }
    int total_correct = line_count - (missing_ids.size() + nonmatch_vals.size()); 
    double percent_corr = (double)total_correct/line_count;
    double percent_missing = (double)missing_ids.size()/line_count; 
    double percent_mismatch = (double)nonmatch_vals.size()/line_count; 
    std::cout << "------SUMMARY------\n"; 
    std::cout << "Total missing ids : " << missing_ids.size() << '\n'; 
    std::cout << "Total non-matching values : " << nonmatch_vals.size() << '\n'; 
    std::cout << "Accuracy percentage : " << percent_corr << '\n'; 
    std::cout << "Missing percentage : " << percent_missing << '\n'; 
    std::cout << "Mismatch percentage : " << percent_mismatch << '\n'; 
}
