# Description
Client-server application to poll sensors on OrangePi PC Board

The communication is local so application uses unix socket.

The server runs in privileged mode in the background and waits for requests.
The remote monitoring service (e.g. Zabbix) must use a client application to get value from sensor.

# Build and install
Run _deploy.sh_ to install from scratch (requires privileged mode):
```shell
./deploy.sh
```
Set environment variables to specify alternatives:
```shell
# alternative paths of wiringPi library
export ALT_LIBPATH=<path/to/wiringPi.so/dir>
export ALT_INCPATH=<path/to/wiringPi.h/dir>
# directory where the socket file will be created
export UNIX_SOCKET_DIR=<path>
# build with debug info
export DEBUG_BUILD=1
```