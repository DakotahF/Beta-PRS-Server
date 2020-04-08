//
//  Genotypes.h
//  PRS-Tool-cpp
//
//  Created by Dakotah Feil on 2/17/20.
//  Copyright © 2020 Dakotah Feil. All rights reserved.
//
#include <string>
#include <vector>
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/reader.hpp"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/armadillo_vector.hpp"
#ifndef Genotypes_h
#define Genotypes_h

class Genotypes {
protected:
    std::string file;
    std::unique_ptr<savvy::vcf::indexed_reader<1>> f;
    bool is_reading;
    bool has_cached;
    savvy::site_info anno;
    savvy::armadillo::dense_vector<double> dosages;
    bool flip;
    std::vector<std::string> selected_samples;
    
public:
    Genotypes(const std::string v );
    
    void open(const std::vector<std::string>& samples);
    
    std::vector<std::__cxx11::basic_string<char> > get_all_samples() const;
    
    std::vector<std::__cxx11::basic_string<char> > get_selected_samples() const;
    
    std::vector<std::__cxx11::basic_string<char> > get_chromosomes() const;
    
    std::vector<double> read_variant(const std::string& chromosome, unsigned int position, const std::string& risk_allele, const std::string& protective_allele);
};

#endif /* Genotypes_h */
