#!/bin/bash

if [ ! -z $ALT_LIBPATH ]
then
    ALT_LIBPATH="-Wl,-rpath ${ALT_LIBPATH} -L ${ALT_LIBPATH}"
fi

if [ ! -z $ALT_INCPATH ]
then
    ALT_INCPATH="-I ${ALT_INCPATH}"
fi

DEBUG_FLAG=
if [ ! -z $DEBUG_BUILD ]
then
    DEBUG_FLAG=-g
fi

g++ $ALT_LIBPATH $ALT_INCPATH $DEBUG_FLAG -std=c++17 server.cc -o server -lwiringPi
g++ $DEBUG_FLAG -std=c++17 client.cc -o client