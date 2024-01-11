#!/bin/bash

SERVER_DEST_PATH=/usr/local/bin/sensor_polling_server
CLIENT_DEST_PATH=/usr/local/bin/poll_sensors

sudo cp serverd $SERVER_DEST_PATH
sudo chmod +x $SERVER_DEST_PATH

sudo cp client $CLIENT_DEST_PATH
sudo chmod +x $CLIENT_DEST_PATH

sudo cp sensor_polling.service /etc/systemd/system/