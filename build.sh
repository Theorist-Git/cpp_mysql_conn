#!/usr/bin/sh

set -xe

g++ -Wall -I/usr/include/cppconn -o conn conn.cpp -L/usr/lib -lmysqlcppconn
./conn