#!/bin/bash

if [ "$1" == "" ]; then
	DATA='{"jsonrpc":"2.0","method":"status","id":"5"}'
else
	DATA=$1
fi

if [ "$1" == "-v" ] || [ "$2" == "-v" ]; then
	OPTS="-v"
else
	OPTS=""
fi

curl $OPTS --noproxy 127.0.0.1 -H 'Content-Type: application/json' -H 'Accept: Application/json' --data "$DATA" http://127.0.0.1:9999
