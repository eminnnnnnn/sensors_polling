#!/bin/bash

sudo mkdir -v -p /home/.sensors_sock
sudo chmod 777 /home/.sensors_sock

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