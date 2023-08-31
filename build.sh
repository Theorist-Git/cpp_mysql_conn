#!/usr/bin/sh

set -xe

sudo systemctl stop mysql
sudo /opt/lampp/lampp start
g++ -Wall -I/usr/include/cppconn -o conn conn.cpp -L/usr/lib -lmysqlcppconn
./conn