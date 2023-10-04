/*
A wrapper around the MySQL cpp connector
Copyright (C) 2023 <Mayank Vats>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <optional>
#include <iomanip>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "conn_macros.hpp"

void err_catch(sql::SQLException &e) {

    /*
    Handles all the SQLExceptions that may occur in any function
    [ERROR](s) are displayed only when DEBUG_LEVEL macro is defined to be <= 2
    */

    if (DEBUG_LEVEL <= 2) {
        std::cout << RED <<  "- [ERROR]: SQLException in " << __FILE__;
        std::cout << RED << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << RED << "- [ERROR]: " << e.what();
        std::cout << RED << " (MySQL error code: " << e.getErrorCode();
        std::cout << RED << ", SQLState: " << e.getSQLState() << " )" << RESET << std::endl;
    }
}

void cout_result_set(sql::ResultSet* res) {

    /*
    Prints a sql::ResultSet* type object to stdout
    Utilizes <iomanip> library to generate tables.
    Refer:
        - [https://www.delftstack.com/howto/cpp/create-table-in-cpp/]
        - [https://cplusplus.com/reference/iomanip/]
    */

    sql::ResultSetMetaData* rsmd = res->getMetaData();
    size_t col_count = rsmd->getColumnCount();
    size_t i = 1;

    // Print out all the columns
    while (i <= col_count) {
        std::string col_name = "|" + rsmd->getColumnName(i++) + "|";
        std::cout << std::left << std::setw(15) << col_name;
    }
    std::cout << std::endl;
    // Reset iterator to base value ≡ 1
    i = 1;
    
    // Print rows
    // NOTE: All fields are printed as strings.
    while (res->next()) {
        while (i <= col_count) {
            std::cout << std::left << std::setw(15) << res->getString(i++);
        }
        std::cout << std::endl;
        i = 1;
    }
    std::cout << std::endl;  
}

sql::Connection* est_conn() {

    /* Establishes connection to an SQL server via macros defined in conn_macros.hpp */

    try {
        sql::Driver* driver;
        sql::Connection* conn;

        /* Create a connection */
        driver = get_driver_instance();
        conn = driver->connect(HOST, USER, KEY);

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: Connect(HOST, USER, KEY)" << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Connected to `" << USER << "` at `" << HOST << "`" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        return conn;
    }
    catch (sql::SQLException &e) {
        err_catch(e);
        exit(EXIT_FAILURE);
    }
}

int close_conn(sql::Connection* conn) {

    /* Closes connection to an SQL server. */

    try {
        delete conn;
        
        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: delete conn" << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Disconnected from `" << USER << "` at `" << HOST << "`" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        return EXIT_SUCCESS;
    }
    catch (sql::SQLException &e) {
        err_catch(e);

        return EXIT_FAILURE;
    }
}

int create_db(std::string db, sql::Connection* conn) {

    /* Creates a database on the connected SQL server. */

    sql::Statement *stmt;

    try {
        stmt = conn->createStatement();
        stmt->execute("CREATE DATABASE " + db);

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: CREATE DATABASE " << db << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Database `" << db << "` created successfully" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        delete stmt;

        return EXIT_SUCCESS;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return EXIT_FAILURE;
    }
}

int delete_db(std::string db, sql::Connection* conn) {

    /* Deletes a database on the connected SQL server. */

    sql::Statement *stmt;

    try {
        stmt = conn->createStatement();
        stmt->execute("DROP DATABASE `" + db + "`");

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: DROP DATABASE " << db << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Database `" << db << "` Deleted successfully" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        delete stmt;

        return EXIT_SUCCESS;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return EXIT_FAILURE;
    }
}

int create_table(std::string db, std::pair<std::string, std::string> table, sql::Connection* conn) {
    /*
    Tables are parsed as pair objects where .first = table_name and .second = table_schema

    Eg: 
    std::pair<std::string, std::string> table = {
        "wrapper", // Table name
        "id INTEGER, fname VARCHAR(20), lname VARCHAR(20), dob DATE, PRIMARY KEY(id)" // Schema
    };

    */
    sql::Statement* stmt;

    try {
        stmt = conn->createStatement();
        conn->setSchema(db);
        stmt->execute("CREATE TABLE " + table.first + "(" + table.second + ")");

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: CREATE TABLE " + table.first + "(" + table.second + ")" << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Table `" << table.first << "` created in `" << db << "`" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        delete stmt;

        return EXIT_SUCCESS;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return EXIT_FAILURE;
    }
}

sql::ResultSet* get_table_schema(
    std::string db, 
    std::pair<std::string, std::optional<std::string>> table,
    sql::Connection* conn
) {

    /*
    Equivalent to query: DESCRIBE table_name;
    Tables are still parsed as pair objects where .first = table_name
    but .second = table_schema is an optional parameter a perfectly valid arg for table could be:

    Eg: 
    std::pair<std::string, std::optional<std::string>> table = {
        "wrapper", // Table name
        std::nullopt // Schema
    };

    It is NOT necessary to know the table_schema for this function

    */

    sql::Statement* stmt;

    try {
        sql::ResultSet* res;
        stmt = conn->createStatement();

        conn->setSchema(db);
        res = stmt->executeQuery("DESCRIBE " + table.first);

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: DESCRIBE TABLE " + table.first << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Scheme of `" << table.first << "` Returned" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        delete stmt;

        return res;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return NULL;
    }
}

int delete_table(std::string db, std::pair<std::string, std::string> table, sql::Connection* conn) {

    /* Deletes a table on the connected SQL server in the specified database. */

    sql::Statement* stmt;

    try {
        stmt = conn->createStatement();
        conn->setSchema(db);
        stmt->execute("DROP TABLE " + table.first);

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: DROP TABLE " + table.first << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Table `" << table.first << "` Deleted from `" << db << "`" << 
                RESET << std::endl;
                break;
            default:
                break;
        }

        delete stmt;

        return EXIT_SUCCESS;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return EXIT_FAILURE;
    }
}

std::pair<sql::ResultSet*, int> exec_arbitrary_stmt(
    std::string db, 
    sql::Connection* conn, 
    std::string query,
    int result_set_expected = 1
) {

    /*
    Executes any SQL query directly.
    The query parameter has raw SQL queries.

    c++ SQL connector has two types of execute methods:
        1) executeQuery() => Returns sql::ResultSet* type object for queries like SELECT, DESCRIBE etc.
        2) execute() => Returns boolean type object for queries like ALTER, CREATE etc.

    The parameter result_set_expected is used to decide between these methods.
    Since return value could be either of sql::ResultSet* and int, the function returns a pair object

    IF result_set_expected = 1: EXECUTE executeQuery() command             [NO USEFUL INT REturn] 
      RETURN std::pair<sql::ResultSet*, int> = {stmt->executeQuery(query), -1};
                                                      ^^^^^^^^^^^^
                                                      [returns sql::ResultSet]   
                                                             
    ELSE EXECUTE execute() command
        RETURN std::pair<sql::ResultSet*, int> = {NULL, stmt->execute(query)};
                                                              ^^^^^^^
                                                              [returns bool(typecasted to int)]

    */

    sql::Statement* stmt;

    try {
        std::pair<sql::ResultSet*, int> res;
        stmt = conn->createStatement();
        conn->setSchema(db);

        if (result_set_expected) {
            res = {stmt->executeQuery(query), -1};
        } else {
            res = {NULL, stmt->execute(query)};
        }

        switch (DEBUG_LEVEL) {
            case 0:
                std::cout << "- [DEBUG]: EXECUTED STATEMENT: \n\t`" << query << "`" << std::endl;
            case 1:
                std::cout << GREEN << "- [INFO] : Query successful `" << "Exit Code: " << EXIT_SUCCESS << 
                "\n\tA std::pair<sql::ResultSet*, int> type object is returned\n" <<
                "\tFor queries that return a ResultSet, return {sql::ResultSet, -1}\n" <<  
                "\tFor queries returning a bolean object, return {NULL, 0/1}" << RESET << std::endl;
                break;
            default:
                break;
        }   

        delete stmt;

        return res;

    } catch(sql::SQLException &e) {
        err_catch(e);
        delete stmt;

        return {NULL, -1};
    }
}

