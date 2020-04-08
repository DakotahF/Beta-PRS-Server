#include <string>
#include <memory>
#include <vector>
#include <limits>
//#include "Genotypes.h"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/reader.hpp"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/armadillo_vector.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Weights.h"
#include "Individuals.h"

using namespace std;


class Genotypes {
protected:
   string file;
   unique_ptr<savvy::vcf::indexed_reader<1>> f;
   bool is_reading;
   bool has_cached;
   savvy::site_info anno;
   savvy::armadillo::dense_vector<double> dosages;
   //vector<float> dosages;
   //arma::vec dosages;
   bool flip;
   vector<string> selected_samples;

public:
   //string file;

   Genotypes(const std::string& v ): file(v), is_reading(false), has_cached(false), flip(false) {}

   void open(const vector<string>& samples) {
      //cout << file << '\n';
      std::cout << "enters open" << '\n';
      selected_samples.clear();
      f = unique_ptr<savvy::vcf::indexed_reader<1>>(new savvy::vcf::indexed_reader<1>(file, {""}, savvy::fmt::ds));
      std::cout << typeid(f).name() << endl;
      if (samples.size() > 0) {
         selected_samples = f->subset_samples({ samples.begin(), samples.end() });
      } else {
         selected_samples = f->samples();
      }
      //std::cout << "f->read in open" << '\n';
      //f->read(anno,dosages); //fixme, should be in read_variant
      is_reading = false;
      has_cached = false;
   }

   void test_fread() {
        std::cout << "Test Fread start " << '\n';
        unsigned int pos; 
        std::string chr,ref,alt; 
   	while(f->good()) { 
            f->read(this->anno,this->dosages);
            std::cout << this->anno.chromosome() << '\n';
            std::cout << this->anno.position() << '\n';
        }
   }

   std::vector<std::__cxx11::basic_string<char> >  get_all_samples() const {
      return savvy::vcf::reader<1>(file, savvy::fmt::gt).samples();
   }

   std::vector<std::__cxx11::basic_string<char> >  get_selected_samples() const {
      return selected_samples;
   }

   std::vector<std::__cxx11::basic_string<char> >  get_chromosomes() const {
      return savvy::vcf::indexed_reader<1>(file, {""}, savvy::fmt::gt).chromosomes();
   }

   arma::vec read_variant(const string& chromosome, unsigned int position, const string& risk_allele, const string& protective_allele) {
      if (is_reading) {
         if ((chromosome.compare(anno.chromosome()) != 0) || (position > anno.position() + 10000)) {
            f->reset_region({ chromosome, position, numeric_limits<int>::max() });
            has_cached = false;
         }
      } else {
         std::cout << anno.chromosome() << ":" << anno.position() << '\n' << "pre-reset";
         f->reset_region({ chromosome, position, numeric_limits<int>::max() });
         std::cout << anno.chromosome() << ":" << anno.position() << '\n' << "post-reset";
         is_reading = true;
         has_cached = false;
      }
      if (!has_cached) {
         f->read(anno, dosages);
      }
      bool position_found = false;
      while (f->good()) {
         cout << "f ptr +" << '\n';
         if (anno.position() < position) {
            f->read(anno, dosages);  
            continue;
         }
         if (anno.position() > position) {
            has_cached = true;
            break;
         }
         position_found = true;
         flip = false;
         if ((risk_allele.compare(anno.ref()) == 0) && (protective_allele.compare(anno.alt()) == 0)) {
            flip = true;
         } else if ((risk_allele.compare(anno.alt()) != 0) or (protective_allele.compare(anno.ref()) != 0)) {
            f->read(anno, dosages);
            continue;
         }
         /*if (flip) {
            dosages -= 2.0;
            dosages *= -1;
         }*/
         has_cached = false;
         return dosages;
      }
      if (position_found) {
         std::cout << "Allele combination not found" << '\n';
         //Rcpp::warning("Allele combination %s/%s not found at position %s:%d", protective_allele, risk_allele, anno.chromosome(), anno.position());
      } /* else {
         Rcpp::warning("No variant at position %s:%d", anno.chromosome(), anno.position());
      } */
      return arma::vec(); // return empty vector if variant was not found
   }
};


int main() {
    std::string filename = "/net/hunt/home/kotah/prs-server-beta/plink-weights/raw/PRS_weights_PheCode_X626.8_SAIGE_b38_5e-2_clumped_r2_0.2.chr16.clumped.cleaned.sorted"; 
    std::string genotype_f = "/net/fantasia/home/sarahgra/CHIP_data_GLGC/1KGP3_imputation_results_EUR/new_1KGP3_imputation_results/chr16.dose.vcf.gz";
    savvy::armadillo::dense_vector<double> dosages;
    Weights weights;
    Genotypes genotypes(genotype_f);
    weights.read_weight_file(filename);
    std::cout << "Read in weight_file" << '\n';
    int test_sample = 5;
    std::vector<std::string> samples;
    std::cout << "samples open" << '\n';
    genotypes.open(samples);
    std::cout << "Genotypes opened & initialized" << 'n';
    Individual_scores scores(weights.num_weights, test_sample);
    unsigned int pos;
    std::string chr,ea,oa;
    int var = 0;
    for(Row w_row : weights.rows){
        ea = w_row.ref;
        oa = w_row.alt;
        chr = w_row.chr;
        pos = w_row.pos;
        cout << "w_row" << '\n';
	if(var % 10000 == 0) 
            std::cout << "Processed " << var << " rows from weight_file" << '\n';
        int it = 0;
        dosages = genotypes.read_variant(chr,pos,ea,oa);
	dosages.print();
	//print_vec(dosages);
        double weight = 0.00;
        while(it < weights.num_weights){
            std::vector<double> prs_vec;
            weight = w_row.weights[it];
            //prs_vec = calculate_prs(weight,dosages);
            //if(it == 0 && var == 0)
                //std::transform(scores.scores[it].begin(),scores.scores[it].end(),prs_vec.begin(),scores.scores[it].begin(), std::plus<double>());
            it++;
        }
        var++;
    }
    /*while(f.good()) {
	f.read(anno,dosages);
	std::cout << anno.chromosome() << ":" << anno.position() << '\n';
        std::cout << "pass" << '\n';
    }*/
}
