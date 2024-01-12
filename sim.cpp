#include "ucn_gen_PCG.hpp"
#include "count_ucn.hpp"
#include "rand_distributions.hpp"
#include "make_trees.hpp"
#include "pcg_random.hpp"
#include "seed_source.hpp"
#include <getopt.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <limits>
#include <math.h>
//#include <mpi.h>
#include "TTree.h"
#include "TFile.h"

int main(int argc, char** argv)
{
    int c;
    char* arg_r = NULL;
    char* arg_d = NULL;
    char* arg_f = NULL;
    while ((c = getopt(argc, argv, "r:d:f:")) != -1)
    {
        switch (c)
        {
            case 'r':
            {
                arg_r = optarg;
                std::cout << "rate: ";
                if (arg_r)
                {
                    std::cout << arg_r << std::endl;
                }
                break;
            }
            case 'd':
            {
                arg_d = optarg;
                std::cout << "duration: ";
                if (arg_r)
                {
                    std::cout << arg_d << std::endl;
                }
                break;
            }
            case 'f':
            {
                arg_f = optarg;
                std::cout << "filename: ";
                if (arg_f)
                {
                    std::cout << arg_f << std::endl;
                }
                break;
            }
            case '?':
            {
                std::cerr << "Got unknown option." << std::endl; 
                exit(2);
            }
            default:
            {
                std::cerr << "Got unknown parse returns: " << c << std::endl; 
                abort();
            }
        }
    }
    for (int i = optind; i < argc; i ++)
    {
        std::cout << "Non-option argument ignored: "
                  << argv[i] << std::endl;
    }
    if (!(arg_f && arg_r && arg_d))
    {
        std::cerr << "Missing arguments. "
                  << "./sim -r <rate> -d <duration> -f <output_file>" << std::endl;
        exit(2);
    }
    
    char* filename = arg_f;
    double rate = atof(arg_r);
    double duration = atof(arg_d);

    //int ierr = MPI_Init(&argc, &argv);
    int nproc;
    int rank;
    
    //ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //ierr = MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    __uint128_t seed = (((__uint128_t)seed_source[1]) << 127) | (((__uint128_t)seed_source[0]));
    //One nice thing about the PCG generator is that it has 2^127 streams,
    //which are each independent. Assign one stream per core based on core ID.
    pcg64 r(seed,(unsigned int)rank);

    ucn_gen_PCG generator = ucn_gen_PCG();

    //These are the parameter central values I've identified through the fits.
    generator.mu1 = 3.168308578;
    generator.sigma1 = 0.33346646;
    generator.mu2 = 3.77243909665;
    generator.sigma2 = 0.25119938;
    generator.p_pmt1 = 0.566725424184;
    generator.p_pmt2 = 1.0 - generator.p_pmt1;
    generator.p_shrt = 0.215304093372;
    generator.t_shrt = 153.902059371e-9;
    generator.p_med = 0.38085062563;
    generator.t_med = 1864.84601579e-9;
    generator.p_long = 1.0 - generator.p_shrt - generator.p_med;
    generator.t_long = 16311.0287305e-9;
    generator.t_trunc = std::numeric_limits<float>::infinity();
    
    
    //Generate a uniform rate by simulating a poisson process.
    
    std::vector<double> t0s;
    double t = nextExp(r)/rate;
    while (t < duration)
    {
        t0s.push_back(t);
        t += nextExp(r)/rate; //interarrival time is exponentially distributed
    }
    
    std::vector<evt> raw = generator.gen_evts(r, t0s);

    TFile* f = new TFile(filename,"RECREATE");
    
    std::pair<TTree*,TTree*> trees = make_trees(raw,t0s);
    
    //ierr = MPI_Finalize();

    trees.first->Write();
    trees.second->Write();
    f->Close();

    return 0;
}
