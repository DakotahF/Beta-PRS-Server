#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map> 
#include <tuple>
#ifndef Ancestry_h
#define Ancestry_h

using namespace std;

class Ancestry_estimator { 
protected: 
    std::string reference_filename; 
    std::string sample_filename;
    std::string output_filename; 
    void initialize(int pcs);  
    std::unordered_map<std::string,std::string> ref_iids_pop; //from ref
    std::vector<std::string> super_pops = {"AFR","AMR","EAS","EUR","SAS"};
public:
    unsigned int princ_comps;
    std::unordered_map<std::string, tuple <std::vector<double>, double> > centroids;  
    Ancestry_estimator(std::string reference_filename, std::string sample_filename, std::string output_filename);
    double get_euclidean_distance(std::vector<double> vec1, std::vector<double> vec2); 
    void calculate_centroids(int pcs); 
    void print_ancestry();
    void check_input(); 
    std::string classify_ancestry(std::vector<double> indiv_vec);
    void print_vec(std::vector<double> vec);
};


#endif
