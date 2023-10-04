#include "wrapper.cpp"
#include <iostream>
#include <iomanip>

int main() {
    /* 
        Establish Connection with the server
        HOST, USER and KEY must be defined in a file called conn_macros.hpp
    */
    sql::Connection* conn = est_conn();

    /* Create a database with name defined in conn_macros.hpp */
    create_db(DATABASE, conn);
    
    /* Create a table pair object where .first = table_name and .second = table_schema */
    std::pair<std::string, std::string> table = {
        "wrapper", // Table name
        "id INTEGER, fname VARCHAR(20), lname VARCHAR(20), dob DATE, PRIMARY KEY(id)" // Schema
    };
    create_table(DATABASE, table, conn);

    /*
        Arbitrary statement execution
        Case 1: Result set IS NOT expected from query:
    */

    // c1_res stores success or failure of the query
    int c1_res = exec_arbitrary_stmt(
        DATABASE, 
        conn, 
        "Insert into wrapper values (1, 'mayank', 'vats', '2003-10-21'); ", 0
    ).second;
    std::cout << "Result of stmt:\nInsert into wrapper values (1, 'mayank', 'vats', '2003-10-21');\n: " << c1_res << std::endl;

    exec_arbitrary_stmt(
        DATABASE, 
        conn, 
        "Insert into wrapper values (1, 'knayam', 'vats', '2003-10-21'); ", 0
    ).second;

    /*
        Arbitrary statement execution
        Case 2: Result set IS expected from query:
    */
    sql::ResultSet* test = exec_arbitrary_stmt(DATABASE, conn, "Select distinct lname from wrapper").first;
    cout_result_set(test);

    // Gets the table structure
    sql::ResultSet* res = get_table_schema(DATABASE, table, conn);
    cout_result_set(res);

    delete res;

    /* Delete the previously created table and database */
    delete_table(DATABASE, table, conn);
    delete_db("wrapper_test", conn);

    /* Close the connection between server and client */
    close_conn(conn);

    return 0;
}