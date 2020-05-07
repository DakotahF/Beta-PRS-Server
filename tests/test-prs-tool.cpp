#include <string>
#include <sstream>
#include <tuple>
#include <iostream> 
#include <fstream> 
#include <math.h>


std::tuple<std::string, double> parse_line(std::string line) {
    std::stringstream ss(line);
    std::string val, id;
    double score;
    int i = 0;
    while(ss >> val) {  
        if (i == 0)
            id = val;
        else if (i == 1)  
            score = std::stod(val); 
        else 
            std::cerr << "error, more than two values in output file detected\n"; 
    }
    std::tuple<std::string, double> values (id, score); 
    return values; 
}

int compare_scores(std::string good_file, std::string test_file, double precision) {
    std::ifstream good(good_file); 
    std::ifstream test(test_file); 
    std::tuple<std::string, double> good_vals, test_vals;
    double difference;
    int line_num = 0;   

    while(!good.eof() || !test.eof()) { 
        std::string g_line,t_line;

        good >> g_line; 
        test >> t_line; 
        
        good_vals = parse_line(g_line); 
        test_vals = parse_line(t_line);

        difference = fabs(std::get<1>(good_vals) - std::get<1>(test_vals)); 
        if (std::get<0>(good_vals) != std::get<0>(test_vals)) 
            return 1; //error, files do not match 
        if (difference > precision) { 
            std::cout << "Difference in precision greater than " << precision << " at line " << line_num << '\n';
            return 1;   
        }
        line_num++; 
    }
    return 0; 
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        return 1; 
    }
    double precision;
    std::string reference_results, sample_results; 
    sample_results = argv[0]; 
    reference_results = argv[1]; 
    if (argc == 3) {
        precision = std::stod(argv[2]);
    }
    else {
        precision = 0.200;  
    }

    compare_scores(reference_results, sample_results, precision);
}
