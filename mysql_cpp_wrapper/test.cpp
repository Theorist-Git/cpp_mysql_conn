/*
  `wrapper.cpp` auto-includes the necessary SQL packages.
*/
#include "wrapper.cpp"
#include <iostream>

/* Macros for establishing database connection. */
#define HOST "localhost"
#define USER "root"
#define KEY ""
#define DATABASE "wrapper_test"

#define DEBUG_LEVEL 0

int main() {
    /* 
        Establish Connection with the server
        HOST, USER and KEY must be defined in a file called conn_macros.hpp
    */
    sql::Connection* conn = est_conn(HOST, USER, KEY);

    /* Create a database with name defined in conn_macros.hpp */
    create_db(DATABASE, conn);
    
    /* Create a table pair object where .first = table_name and .second = table_schema */
    std::pair<std::string, std::string> table = {
        "wrapper", // Table name
        "id INTEGER, fname VARCHAR(20), lname VARCHAR(20), dob DATE, PRIMARY KEY(id)" // Schema
    };

    create_table(DATABASE, table, conn);

    // Gets the table structure
    sql::ResultSet* res = get_table_schema(DATABASE, table.first, conn);
    cout_result_set(res);

    delete res; // Free memory


    /*
        Arbitrary statement execution
        Case 1: Result set IS NOT expected from query:
    */

    // c1_res stores success or failure of the query
    int c1_res = exec_stmt(
        DATABASE, 
        conn, 
        "Insert into wrapper values (1, 'mayank', 'vats', '2003-10-21'); ", 0 // 0 => result set not expected!
    ).second;

    std::cout << "Result of stmt:\nInsert into wrapper values (1, 'mayank', 'vats', '2003-10-21');\n: " << c1_res << std::endl;

    exec_stmt(
        DATABASE, 
        conn, 
        "Insert into wrapper values (2, 'knayam', 'vats', '2003-10-21'); ", 0
    ).second;

    /*
        Arbitrary statement execution
        Case 2: Result set IS expected from query: (Default behaviour)
    */
    sql::ResultSet* test = exec_stmt(DATABASE, conn, "Select distinct lname from wrapper").first;
    cout_result_set(test);
    delete test;

    /* Delete the previously created table and database */
    delete_table(DATABASE, table, conn);
    delete_db(DATABASE, conn);

    /* Close the connection between server and client */
    close_conn(conn);

    return EXIT_SUCCESS;
}