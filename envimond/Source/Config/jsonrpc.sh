#!/bin/bash

REQ1='{"jsonrpc":"2.0","method":"envi.getNumDataSources","params":"","id":"5"}'
REQ2='{"jsonrpc":"2.0","method":"envi.getDataSource","params":"0","id":"6"}'

curl $OPTS --noproxy 127.0.0.1 -H 'Content-Type: application/json' -H 'Accept: Application/json' --data "$REQ1" http://127.0.0.1:9999/jsonrpc
curl $OPTS --noproxy 127.0.0.1 -H 'Content-Type: application/json' -H 'Accept: Application/json' --data "$REQ2" http://127.0.0.1:9999/jsonrpc
