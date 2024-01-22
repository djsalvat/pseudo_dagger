#!/bin/bash
DIR=$1

./build/apps/sim -r 2000 -d 500 -f $DIR/nate_model_2kHz_500s.root
