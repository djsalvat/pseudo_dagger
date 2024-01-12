#ifndef MAKE_ROOT_TREE_H
#define MAKE_ROOT_TREE_H

#include <vector>
#include "TTree.h"
#include "ucn_gen_PCG.hpp"

//One member for each branch in the tree.
typedef struct root_entry
{
    int channel;
    int edge;
    int tag;
    int full;
    unsigned long time;
    double realtime;
} root_entry;

std::pair<TTree*,TTree*> make_trees(std::vector<evt> &evts, std::vector<double>& t0s);

#endif /*MAKE_ROOT_TREE*/
