#!/bin/bash

#set -e
#make -e obj-intel64/pinatrace.so

#path to makefile
dir="./src"

#get the argument
cmd=$1

#do 'make clean'
function do_clean
{
    echo "make clean"
    cd $dir
    make clean
    cd - > /dev/null
}

#do 'make'
function do_make
{
    echo "make"
    cd $dir
    make
    cd - > /dev/null
}

#do prompt
function do_info
{
    printf "usages:\n1)./make.sh\n2)./make.sh clean\n"
}

if [ "$cmd" = "clean" ]; then
    do_clean
elif [ -z $cmd ]; then
    do_make
else
    do_info
fi

