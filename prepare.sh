#!/bin/bash

if [ -z $UNIX_SOCKET_DIR ]
then
    UNIX_SOCKET_DIR=/home/.sensors_sock
    echo "Using default path (${UNIX_SOCKET_DIR}) to create dir for unix socket"
fi

UNIX_SOCKET_DIR=$(realpath ${UNIX_SOCKET_DIR})

sudo mkdir -v -p $UNIX_SOCKET_DIR
sudo chmod 777 $UNIX_SOCKET_DIR

if [ ! "$(find /usr/include /usr/local/include -iname 'wiringPi.h')" ]
then
    echo -e "WARNING: There is no header wiringPi.h in system directories."
    echo -e "         You must specify ALT_INCPATH environment variable before running build.sh"
fi

if [ ! "$(find /usr/lib /usr/lib64 /usr/local/lib -iname 'libwiringPi.so')" ]
then
    echo -e "WARNING: There is no library libwiringPi in system directories."
    echo -e "         You must specify ALT_LIBPATH environment variable before running build.sh"
fi