#include "Ancestry.h"
#include <math.h> 
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream> 
#include <cassert> 
#include <sstream> 
#include <algorithm> 
#include <tuple>
#include <functional> 
#include <limits> 

//External helper functions, fixme this is hacky code 
void Ancestry_estimator::print_vec(std::vector<double> vec)
{ //fixme, remove me when cleaning or template and place with other helper functions
   if(vec.size() == 0)
        return;
   for(const auto &i:vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
    return;
}


//Helper functions (potentially move to one file with other helpers for clarity) 
double Ancestry_estimator::get_euclidean_distance(std::vector<double> vec1, std::vector<double> vec2) {
    //cassert(vec1.size() == vec2.size()); fixme, implement cassert rather than an if 
    if(vec1.size() != vec2.size()) {
        std::cout << "vec1.size() (CENTROID)  == " << vec1.size() << " vec2.size() (INDIV) == " << vec2.size() << '\n';
        //std::cout << "Error at: get_euclidean_dist vec1.size() != vec2.size()" << '\n';
        exit(1); 
    }
    double dist = 0.00;
    for(int idx=0; idx < vec1.size(); idx++) {
       dist += pow((vec1[idx] - vec2[idx]),2);
    }
    return sqrt(dist);
}


//Ancestry Class methods + constructor definitions
Ancestry_estimator::Ancestry_estimator(std::string ref_fn, std::string sam_fn, std::string out_fn) 
: reference_filename(ref_fn), sample_filename(sam_fn), output_filename(out_fn) {}

void Ancestry_estimator::initialize(int pcs) { //fixme, currently assumes no FID !
    //pcs = 10; //fixme, uncomment me for ++ functionality 
    std::ifstream ref_pop(this->reference_filename.c_str());
    std::string FID,IID,superpop; 
    if (ref_pop.is_open()) {
        std::cout << "ref_pop is open (fx->initialize())" << '\n';
        std::string line; 
        bool header = true; 
        while(getline(ref_pop, line)) {
            if(header) { 
                header = false; 
                continue; 
            } 
            std::stringstream ss(line);
            std::string val;
            int idx = 0;  
            while(getline(ss,val,' ')){ //fixme, code repetition 
                switch(idx) //fixme, reading in whitespace?  
                {
                    case 0: IID=val; break;
                    //case 1: IID=val; break;
                    case 1: superpop=val; break; 
                    default: 
                        //cassert(formatting);
			std::cout << "FIXME, throw formatting error" << '\n';
                }
                idx++;
            }
            //cout << IID << '!' << '\n';
            //this->ref_iids_pop[FID + '_' +  IID] = superpop; //reference-IID
            this->ref_iids_pop[IID] = superpop; 
            if(this->centroids.find(superpop) == this->centroids.end() && !(superpop.empty())) { 
                vector<double> centroid_vec(pcs,0.0);
                tuple <vector<double>,double>  centroid_tup(centroid_vec, 0.0);  
                this->centroids[superpop] = centroid_tup;
                std::cout << "superpop (fx->initialize) : " << superpop << '\n'; 
            }
        }
    }
    ref_pop.close(); 
    //else { cassert fixme}
    return; 
}

void Ancestry_estimator::calculate_centroids(int pcs){
    this->initialize(pcs);
    std::ifstream samples(this->sample_filename);
    std::string FID, IID, val, superpop;
    if (samples.is_open()) {
        std::string line;
        while(getline(samples, line)) {  
            std::stringstream ss(line);
            vector<double> indiv_pcs;
            indiv_pcs.reserve(pcs);
            int idx = 0; 
            std::string val; 
            while(getline(ss,val, '\t')) { //delimiter change? /t plink2 ' '  plink1.9
                switch(idx) { 
                    case 0: FID=val; break;
                    case 1: IID=val; break;  
                    default: 
                        if(this->ref_iids_pop.find(IID) != this->ref_iids_pop.end()){
                            superpop = ref_iids_pop[IID];
                            //std::cout << "ref_iid : " << IID << " , superpop : " << superpop << '\n';
                            indiv_pcs.push_back(std::atof(val.c_str()));
                        }
                        else { continue; }
               }
               idx++;   
            }
            if(this->ref_iids_pop.find(IID) != this->ref_iids_pop.end()) {  
                std::transform((get<0>(this->centroids[superpop])).begin(),(get<0>(this->centroids[superpop])).end(),indiv_pcs.begin(),(get<0>(this->centroids[superpop])).begin(), std::plus<double>());
                (get<1>(this->centroids[superpop]))++;  
            }
        }
        for(auto& cent:centroids){
            std::cout << "centroid superpop " << cent.first << " num_samples : " << get<1>(cent.second) << '\n';
            this->print_vec(get<0>(cent.second)); 
            std::transform((get<0>(cent.second)).begin(), (get<0>(cent.second)).end(), (get<0>(cent.second)).begin(),std::bind(std::divides<double>(), std::placeholders::_1,get<1>(cent.second)));
            this->print_vec(get<0>(cent.second));
            std::cout << '\n';
        }
    }
    samples.close();  
}

std::string Ancestry_estimator::classify_ancestry(std::vector<double> indiv_vec) {
    double closest_dist = std::numeric_limits<double>::max();
    double dist;
    std::string ancestry; 
    for(auto it : this->centroids) {
        dist = this->get_euclidean_distance(get<0>(it.second), indiv_vec); 
        std::cout << "centroid : " << it.first << "dist : " << dist << "closest_dist : " << closest_dist << '\n';
        if(dist < closest_dist) {  
            closest_dist = dist;
            ancestry = it.first; 
        }
    }
    std::cout << "return value :  " << ancestry << '\n';
    return ancestry;
}

void Ancestry_estimator::print_ancestry(){
    std::ofstream ancestry_out; 
    ancestry_out.open(this->output_filename);
    std::ifstream samples(this->sample_filename);
    bool header = true; 
    if (samples.is_open()) {
       std::string line;
       std::string FID, IID, val, anc;
       while(getline(samples,line)) {
           if(header) {
               header = false;
               continue;
           }
           std::stringstream ss(line); 
           vector<double> indiv_pcs;
           indiv_pcs.reserve(this->princ_comps);
           int idx=0; 
           while(getline(ss,val,'\t')) { // '\t'  plink2  ' ' plink1.9
               switch(idx) { 
                   case 0: FID=val; break; 
                   case 1: IID=val; break; 
                   default: 
                       //indiv_pcs.push_back(std::atof(val.c_str())); 
                       if(this->ref_iids_pop.find(IID) == this->ref_iids_pop.end()) { // !=  prints out 1kg only == prints out sample only
                           //std::cout << "enters ref_iids" << '\n';
                           indiv_pcs.push_back(std::atof(val.c_str())); 
                       }
                       else { continue; } 
               }
               idx++; 
           }
           if(indiv_pcs.size() != 0) { 
           	anc = this->classify_ancestry(indiv_pcs); 
                ancestry_out << IID << '\t' << anc << '\n'; 
           }
           //call helper which decides principle ancestry here 
    }
    ancestry_out.close();  
   }
   return; 
}

