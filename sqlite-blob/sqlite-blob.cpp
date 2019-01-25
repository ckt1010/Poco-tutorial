#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdio.h>

using namespace Poco::Data::Keywords;
using namespace std;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Person
{
    string name;
    string address;
    int         age;
};

int main(int argc, char** argv)
{
    std::ifstream input( "./1.jpg", std::fstream::binary);


    // copies all data into buffer
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    Poco::Data::BLOB data((std::vector<uint8_t> (buffer)));
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();

    // create a session
    Session session("SQLite", "sqlite-blob.db");

    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    session << "CREATE TABLE Person (Name VARCHAR(30), Photo BLOB)", now;

    string name = "test";

    session << "INSERT INTO Person VALUES(?, ?)",
        use(name),
        bind(Poco::Nullable<Poco::Data::BLOB>(data)), now;

    name = "test1";

    session << "INSERT INTO Person VALUES(?, ?)",
        use(name),
        bind(Poco::Nullable<Poco::Data::BLOB>(data)), now;

    name = "test2";

    session << "INSERT INTO Person VALUES(?, ?)",
        use(name),
        bind(Poco::Nullable<Poco::Data::BLOB>(data)), now;

    Statement select(session);
    std::ostringstream oss;
    Poco::Data::CLOB buffer2;
    select << "SELECT * FROM Person",
    into(name),
    into(buffer2),
    range(0, 1);
    uint32_t count = 3;
    char fileName[100];
    while (!select.done())
    {
        count++;
        select.execute();
        snprintf(fileName, 100, "%u.jpg", count);
        std::ofstream out( fileName, std::fstream::binary);
        cout << buffer2.size() << endl;
        cout << buffer2.rawContent() << endl;
        out.write(buffer2.rawContent(), buffer2.size());
    }
    return 0;
}
