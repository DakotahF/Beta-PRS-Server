#include <string>

//basic helper functions

// trim from start
static std::string &ltrim(std::string &s);

// trim from end
static std::string &rtrim(std::string &s); 

// trim from both ends
static std::string &trim(std::string &s); 

// splits input string by delimiter and returns item a position of interest
std::string tokenize(std::string text, char delim, int pos);

//executes system commands
std::string exec(std::string command); 

//merges phenotypes and returns merged weight file
std::string merge_traits(std::string weight_list, std::string output_dir, int verbose_flag);

//formats command for ancestry pre-processing 
std::string prep_ancestry_command(std::string dosage, std::string outputdir);  

//submits jobs for ancestry and prs-calculations
std::string submit_jobs(std::string dosages_list, std::string weight_file, std::string output_dir, int verbose_flag,std::string ref_panel="/net/hunt/home/kotah/prs-server-beta/1000g/1KG-v3.ALL.id-sp.panel", std::string ref_data="/net/hunt/home/kotah/prs-server-beta/ancestry/cran/reference/1000genomes.pruned", int ancestry_flag=1, int run_limit=30); 
