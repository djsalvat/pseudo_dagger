#include <vector>
#include <math.h>
#include "TTree.h"
#include "ucn_gen_PCG.hpp"
#include "make_trees.hpp"
#include <utility>

std::pair<TTree*,TTree*> make_trees(std::vector<evt>& evts, std::vector<double>& t0s)
{
    //Allocate new TTree
    TTree* tmcs_0 = new TTree("tmcs_0","MCS Tree");
    //Struct to hold each tree branch
    root_entry event;
    //We won't use tag or full
    event.tag = 0;
    event.full = 0;
    
    //Set addresses
    tmcs_0->Branch("channel", (void *)&event.channel, "channel/I");
    tmcs_0->Branch("edge", (void *)&event.edge, "edge/I");
    tmcs_0->Branch("tag", (void *)&event.tag, "tag/I");
    tmcs_0->Branch("full", (void *)&event.full, "full/I");
    tmcs_0->Branch("time", (void *)&event.time, "time/l");
    tmcs_0->Branch("realtime", (void *)&event.realtime, "realtime/D");
    
    //Loop through synthetic data; fill tree for each entry
    for (auto it = evts.begin(); it < evts.end(); it++)
    {
        
        event.channel = it->ch;
        event.edge = it->id;    //edge holds a unique ID for each UCN event.
        event.realtime = it->t;
        event.time = (unsigned long)round(it->t / 800e-12);
        tmcs_0->Fill();
    }

    TTree* ttruth = new TTree("ttruth","Truth Tree");
    double realtime;
    ttruth->Branch("realtime",(void *)&realtime, "realtime/D");

    for (auto it = t0s.begin(); it < t0s.end(); it++)
    {
        realtime = *it;
        ttruth->Fill();
    }
    
    return std::pair<TTree*,TTree*>(tmcs_0,ttruth);
}
