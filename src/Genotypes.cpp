#include <string>
#include <memory>
#include <vector>
#include <limits>
#include "Genotypes.h"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/reader.hpp"
#include "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/cget/include/savvy/armadillo_vector.hpp"

Genotypes::Genotypes(const std::string v): file(v), is_reading(false), has_cached(false), flip(false) {}

void Genotypes::open(const std::vector<std::string>& samples) {
  selected_samples.clear();
  f = std::unique_ptr<savvy::vcf::indexed_reader<1>>(new savvy::vcf::indexed_reader<1>(file, {""}, savvy::fmt::ds));
  if (samples.size() > 0) {
     selected_samples = f->subset_samples({ samples.begin(), samples.end() });
  } else {
     selected_samples = f->samples();
  }
  is_reading = false;
  has_cached = false; 
}

std::vector<std::string> Genotypes::get_all_samples() const {
  return savvy::vcf::reader<1>(file, savvy::fmt::gt).samples();
}

std::vector<std::__cxx11::basic_string<char> > Genotypes::get_selected_samples() const {
  return selected_samples;
}

std::vector<std::__cxx11::basic_string<char> > Genotypes::get_chromosomes() const {
  return savvy::vcf::indexed_reader<1>(file, {""}, savvy::fmt::gt).chromosomes();
}

std::vector<long double> Genotypes::read_variant(const std::string& chromosome, unsigned int position, const std::string& risk_allele, const std::string& protective_allele) {
    if (is_reading) {
     if ((chromosome.compare(anno.chromosome()) != 0) || (position > anno.position() + 10000)) {
	f->reset_region({ chromosome, position, std::numeric_limits<int>::max() });
        has_cached = false;
     }
  } else { 
     f->reset_region({ chromosome, position, std::numeric_limits<int>::max() });
     is_reading = true;
     has_cached = false;
  }
  if (!has_cached) { 
     f->read(anno, dosages);
  }
  bool position_found = false;
  int counter = 0; 
  while (f->good()) { 
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
     if (flip) {
        dosages -= 2.0;
        dosages *= -1;
     }
     has_cached = false;
     std::vector<long double> dose_vec; 
     dose_vec = arma::conv_to<std::vector<long double>>::from(dosages);
     return dose_vec;
  }
  if (position_found) {
      std::cout << "Allele combination " << protective_allele << ":" << risk_allele << " not found at position " << anno.chromosome() << ":" << anno.position() << '\n';
      //protective_allele, risk_allele, anno.chromosome(), anno.position());
  } 
  std::vector<long double> vec;
  return vec; // return empty std::vector if variant was not found
}
