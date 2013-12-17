#!/bin/bash

TEMP_RAW=`cat /sys/bus/w1/devices/28-0000057b5d17/w1_slave | grep "t=" | cut -f 2 -d'='`
TEMP_F=$(echo "scale=2; $TEMP_RAW / 1000" |bc -l)
printf "{ 'name' : 'ds18b20 sensor', 'values': [ {'name':'Temperature', 'value':'%.2f'} ] }\n" $TEMP_F