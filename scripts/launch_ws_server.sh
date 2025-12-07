#!/usr/bin/bash

cd test/html/
cmake -B build

cd build/
make

./WsServer 127.0.0.1 8081 ../../../data 1

exit;
