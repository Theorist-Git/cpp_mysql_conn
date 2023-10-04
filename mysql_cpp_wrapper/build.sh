#!/usr/bin/sh

set -xe

g++ -Wall -I/usr/include/cppconn -o ../bin/$1 $1.cpp -L/usr/lib -lmysqlcppconn
../bin/$1