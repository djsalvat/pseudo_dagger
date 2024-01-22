# pseudo_dagger
Pseudodata generator for UCNtau dagger. Uses Nate's original algorithm for now.

## Files
* `submit_slurm.sh` and `run_slurm.sh` are used for job submission on IU's Quarry.
* Main executable in `src/sim.cpp`
* `count_ucn.*` unimplemented at the moment, only generating raw data
* `ucn_gen_PCG.*` are the meat and potatoes of Nate's algorithm
* Other files relate to random number generation for distributions

## Build

ROOT installation required. MPI will be required if `src/sim.cpp` is modified for it.

Simply build with
```
make
```

## Execution

```
./build/apps/sim -r <rate in Hz> -d <duration in s> -f <output root filename>
```
