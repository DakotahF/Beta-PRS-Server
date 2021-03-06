#include <vector>
#include <tuple>
#include <string>
#ifndef Individuals_h
#define Individuals_h


class Individual_scores
{
protected :
    int num_weights; //rows
    int num_ids; //columns
    std::vector<std::string> weights;
public:
    std::vector<std::string> IIDs;
    Individual_scores() = default;
    Individual_scores(int num_weights, int num_ids);
    std::vector<std::vector<long double> > scores;
};


class Individual_row
{
public:
    int cols;
    Individual_row(int c);
    std::vector<long double> return_row();
};

#endif /* Individuals_h */
