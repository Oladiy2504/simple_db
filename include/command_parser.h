#pragma once
#include <includes.h>

enum Types {
    ERROR = -1,
    CREATE_TABLE = 1,
    INSERT = 2,
    SELECT = 3,
    UPDATE = 4,
    DELETE = 5
};

class SimpleDBParser {
public:
    static string trim(const string& str);
    pair<int, smatch> parse(const string& command);

private:
    std::regex createTableRegex{R"(CREATE TABLE (\w+) \((.*?)\);)"};
    std::regex insertIntoRegex{R"(INSERT INTO (\w+) \((.*?)\) VALUES \((.*?)\);)"};
    std::regex selectRegex{R"(SELECT (.*?) FROM (\w+)( WHERE (.*))?;)"};
    std::regex updateRegex{R"(UPDATE (\w+) SET (.*?) WHERE (.*);)"};
    std::regex deleteRegex{R"(DELETE FROM (\w+) WHERE (.*);)"};
};
