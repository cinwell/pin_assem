#!/bin/bash

sh ./make.sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
#--where to save trace files
TRACE_DIR=$(pwd)/gen.trace
[ -d $TRACE_DIR ] || mkdir -p $TRACE_DIR
#--PIN root path
PIN=/afs/cs.pitt.edu/usr0/xianeizhang/private/bench/SATIAGO_SIMU/pin-2.13-62732-gcc.4.4.7-linux/pin
#--pin tool location
TracerPin=$(pwd)/src/obj-intel64/pinassem.so
#--workload location
BENCH_DIR=$(pwd)/Annotate

cd $BENCH_DIR
$PIN -t $TracerPin -output $TRACE_DIR/test_anno -- ANNO #&
cd ..

#wait

echo "done ..."

