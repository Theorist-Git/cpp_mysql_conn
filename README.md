# Connect xampp MySQL server to c++

* See `xampcpp.pdf` for instructions.

# Execute Instructions

```bash
chmod +x make_env.sh
chmod +x build.sh

./make_env.sh
./build.sh
```

* Output:

```bash
./make_env.sh 
+ sudo systemctl stop mysql
[sudo] password for user: 
+ sudo /opt/lampp/lampp start
Starting XAMPP for Linux 8.2.4-0...
XAMPP: Starting Apache...already running.
XAMPP: Starting MySQL...already running.

./build.sh 
+ g++ -Wall -I/usr/include/cppconn -o conn conn.cpp -L/usr/lib -lmysqlcppconn
+ ./conn
1       Lorem   7       1993-08-26
2       Ipsum   8       1993-08-26
3       Dolor   9       1995-08-26
5       MAYANK  10      2003-10-31

```
