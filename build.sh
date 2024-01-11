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

SOCKET_DIR_FLAG=
if [ ! -z $UNIX_SOCKET_DIR ]
then
    UNIX_SOCKET_DIR=$(realpath ${UNIX_SOCKET_DIR})
    SOCKET_DIR_FLAG="-DUNIX_SOCKET_DIR=\"${UNIX_SOCKET_DIR}\""
fi

g++ $ALT_LIBPATH $ALT_INCPATH $DEBUG_FLAG $SOCKET_DIR_FLAG -std=c++17 server.cc -o server -lwiringPi
g++ $ALT_LIBPATH $ALT_INCPATH $DEBUG_FLAG $SOCKET_DIR_FLAG -std=c++17 serverd.cc -o serverd -lwiringPi
g++ $DEBUG_FLAG $SOCKET_DIR_FLAG -std=c++17 client.cc -o client