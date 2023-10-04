#include "wrapper.cpp"
#include <iostream>
#include <iomanip>
#include <optional>

int main() {
    sql::Connection* conn = est_conn();

    create_db(DATABASE, conn);
    
    std::pair<std::string, std::string> table = {
        "wrapper", // Table name
        "id INTEGER, fname VARCHAR(20), lname VARCHAR(20), dob DATE, PRIMARY KEY(id)" // Schema
    };
    create_table(DATABASE, table, conn);
    // exec_arbitrary_stmt(DATABASE, conn, "alter table wrapper add shit integer", 0);

    sql::ResultSet* res = get_table_schema(DATABASE, table, conn);

    std::cout << std::left << std::setw(10) << "|Field|" <<
                 std::left << std::setw(15) << "|Type|" <<
                 std::left << std::setw(10) << "|Null|" <<
                 std::left << std::setw(10) << "|Key|" <<
                 std::left << std::setw(10) << "|Default|" <<
                 std::left << std::setw(10) << "|Extra|"
    << std::endl;

    while (res->next()) {
        std::cout << \
                std::left << std::setw(10) << res->getString(1) <<
                std::left << std::setw(15) << res->getString(2) <<
                std::left << std::setw(10) << res->getString(3) <<
                std::left << std::setw(10) << res->getString(4) <<
                std::left << std::setw(10) << res->getString(5) <<
                std::left << std::setw(10) << res->getString(6)       
        << std::endl;
    }

    delete res;


    delete_table(DATABASE, table, conn);
    // delete_table(DATABASE, {"shit", ""}, conn);
    delete_db("wrapper_test", conn);

    close_conn(conn);

    return 0;
}