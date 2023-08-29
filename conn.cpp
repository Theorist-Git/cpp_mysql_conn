#include <stdlib.h>
#include <iostream>

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

#define DATABASE "Sailor-reserves-boats"
#define HOST "localhost"
#define USER "root"
#define KEY ""

using namespace std;

int main(void)
{
    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::PreparedStatement *prep_stmt;
        sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(HOST, USER, KEY);

        /* Connect to the MySQL test database */
        con->setSchema(DATABASE);

        stmt = con->createStatement();
        res = stmt->executeQuery("select * from Sailor");
        while (res->next())
        {
            /* Access column data by alias or column name */
            cout << res->getString(1) << "\t";
            cout << res->getString(2) << "\t";
            cout << res->getString(3) << "\t";
            cout << res->getString(4) << "\t";
            cout << endl;
        }

        prep_stmt = con->prepareStatement(
            "INSERT INTO Sailor (SID, SNAME, RATING, DOB) VALUES (?, ?, ?, ?)"
        );

        prep_stmt->setInt(1, 4);
        prep_stmt->setString(2, "MAYANK");
        prep_stmt->setInt(3, 10);
        prep_stmt->setDateTime(4, "2003-10-21");

        prep_stmt->execute();

        delete res;
        delete prep_stmt;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
}