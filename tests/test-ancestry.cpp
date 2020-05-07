#include <string> 
#include <vector> 
#include <tuple>
#include <iostream> 
#include <fstream>
#include <sstream> 

std::tuple<std::string, std::string> parse_line(line) {
    std::stringstream ss(line);
    std::string val, id, superpop;
    int i = 0;
    while(ss >> val) {  
        if (i == 0)
            id = value;
        else if (i == 1)  
            superpop = val; 
        else 
            std::cerr << "error, more than two values in output file detected\n"; 
    }
    std::tuple<std::string, double> values (id, superpop); 
    return values;
}

int compare_scores(std::string good_file, std::string test_file) {
    std::ifstream good(good_file); 
    std::ifstream test(test_file); 
    std::tuple<std::string, double> good_vals, test_vals;
    int line_num = 0;   

    while(!good.eof() || !test.eof()) { 
        std::string g_line,t_line;

        good >> g_line; 
        test >> t_line; 
        
        good_vals = parse_line(g_line); 
        test_vals = parse_line(t_line);
 
        if (get<0>(good_vals) != get<0>(test_vals)) 
            return 1; //error, files do not match 
        else if (get<1>(good_vals) != get<1>(test_vals)) { 
            std::cerr << "Superpopulation for id " << get<0>(good_vals) << " at line " << line_num << "differs from inputted values\n";
            return 1;   
        }
        line_num++; 
    }
    return 0; 
}

int main(int arg, char **argv) {
    if (argc < 2) {
        return 1; 
    } 
}
