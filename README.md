# MySQL c++ Wrapper

* Installing MySQL c++ connector on unix-like systems:

  * You need to install `libmysqlcppconn-dev` package for your distro.
  * For ubuntu/pop-os:

  ```bash
  sudo apt install libmysqlcppconn-dev
  ```

## Usage

* `/mysql_cpp_wrapper/test.cpp` file contains examples on how to use the `wrapper.cpp` file.
* `wrapper.cpp` requires a file named `conn_macros.hpp` to work, you can create your own file with your macros, a starter file is in the repo.
* To run your file, say, `test.cpp` use the `build.sh` file.

```bash
chmod +x build.sh
./build.sh test
```

This will store the binaries in a `bin` directory in the project root folder.

* Output:

```bash
$ ./build.sh test
+ g++ -Wall -I/usr/include/cppconn -o ../bin/test test.cpp -L/usr/lib -lmysqlcppconn
+ ../bin/test
- [DEBUG]: Connect(HOST, USER, KEY)
- [INFO] : Connected to `root` at `localhost`
- [DEBUG]: CREATE DATABASE wrapper_test
- [INFO] : Database `wrapper_test` created successfully
- [DEBUG]: CREATE TABLE wrapper(id INTEGER, fname VARCHAR(20), lname VARCHAR(20), dob DATE, PRIMARY KEY(id))
- [INFO] : Table `wrapper` created in `wrapper_test`
- [DEBUG]: EXECUTED STATEMENT: 
	`Insert into wrapper values (1, 'mayank', 'vats', '2003-10-21'); `
- [INFO] : Query successful `Exit Code: 0
	A std::pair<sql::ResultSet*, int> type object is returned
	For queries that return a ResultSet, return {sql::ResultSet, -1}
	For queries returning a bolean object, return {NULL, 0/1}
Result of stmt:
Insert into wrapper values (1, 'mayank', 'vats', '2003-10-21');
: 0
- [DEBUG]: EXECUTED STATEMENT: 
	`Insert into wrapper values (2, 'knayam', 'vats', '2003-10-21'); `
- [INFO] : Query successful `Exit Code: 0
	A std::pair<sql::ResultSet*, int> type object is returned
	For queries that return a ResultSet, return {sql::ResultSet, -1}
	For queries returning a bolean object, return {NULL, 0/1}
- [DEBUG]: EXECUTED STATEMENT: 
	`Select distinct lname from wrapper`
- [INFO] : Query successful `Exit Code: 0
	A std::pair<sql::ResultSet*, int> type object is returned
	For queries that return a ResultSet, return {sql::ResultSet, -1}
	For queries returning a bolean object, return {NULL, 0/1}
|lname|        
vats           

- [DEBUG]: DESCRIBE TABLE wrapper
- [INFO] : Scheme of `wrapper` Returned
|COLUMN_NAME|  |COLUMN_TYPE|  |IS_NULLABLE|  |COLUMN_KEY|   |COLUMN_DEFAULT||EXTRA|        
id             int(11)        NO             PRI                                          
fname          varchar(20)    YES                                                         
lname          varchar(20)    YES                                                         
dob            date           YES                                                         

- [DEBUG]: DROP TABLE wrapper
- [INFO] : Table `wrapper` Deleted from `wrapper_test`
- [DEBUG]: DROP DATABASE wrapper_test
- [INFO] : Database `wrapper_test` Deleted successfully
- [DEBUG]: delete conn
- [INFO] : Disconnected from `root` at `localhost`
```
