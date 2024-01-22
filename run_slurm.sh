#!/bin/bash
DIR=$1

./build/apps/sim -r 2000 -d 5000 -f $DIR/nate_model_2kHz_5000s.root
