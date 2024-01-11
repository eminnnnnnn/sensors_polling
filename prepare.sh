#!/bin/bash

if [ -z $UNIX_SOCKET_DIR ]
then
    UNIX_SOCKET_DIR=/home/.sensors_sock
    echo "Using default path (${UNIX_SOCKET_DIR}) to create dir for unix socket"
fi

UNIX_SOCKET_DIR=$(realpath ${UNIX_SOCKET_DIR})

sudo mkdir -v -p $UNIX_SOCKET_DIR
sudo chmod 777 $UNIX_SOCKET_DIR

wiringOP_INSTALLED=1

if [ ! "$(find /usr/include /usr/local/include -iname 'wiringPi.h')" ]
then
    wiringOP_INSTALLED=0
    echo -e "WARNING: There is no header wiringPi.h in system directories."
    echo -e "         You must specify ALT_INCPATH environment variable before running build.sh or install wiringOP"
fi

if [ ! "$(find /usr/lib /usr/local/lib -iname 'libwiringPi.so')" ]
then
    wiringOP_INSTALLED=0
    echo -e "WARNING: There is no library libwiringPi in system directories."
    echo -e "         You must specify ALT_LIBPATH environment variable before running build.sh  or install wiringOP"
fi

if [ $wiringOP_INSTALLED == 0 ]
then
    read -p "Install wiringOP library? (y/n):" need_install
    if [ $need_install == "y" ]
    then
        git clone https://github.com/orangepi-xunlong/wiringOP
        cd wiringOP
        ./build clean
        ./build
        cd ..
    fi
fi