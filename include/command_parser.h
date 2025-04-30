#pragma once
#include <includes.h>

enum Command_Types {
    ERROR = -1,
    CREATE_TABLE = 1,
    INSERT = 2,
    SELECT = 3,
    UPDATE = 4,
    DELETE = 5
};

enum Condition {
    ERROR_COND = -1,
    ALL = 0,
    EQUAL = 1,
    NOT_EQUAL = 2,
    GREATER = 3,
    LESS = 4,
    GREATER_EQUAL = 5,
    LESS_EQUAL = 6
};

struct ParsedCommand {
    Command_Types type = ERROR;
    string table_name;

    // Для CREATE TABLE
    vector<pair<string, string>> new_columns; // {name, type}

    // Для SELECT
    string columns;
    string values;
    string where_column;
    Condition where_condition = ALL;
    string where_value;
};

class SimpleDBParser {
public:
    static string trim(const string& str);

    ParsedCommand parse(const string &command) const;

private:
    std::regex createTableRegex{R"(^CREATE\s+TABLE\s+(\w+)\s*$\s*([^()]*)\s*$\s*;\s*$)"};
    std::regex insertIntoRegex{R"(^INSERT\s+INTO\s+(\w+)\s*$\s*([^()]*)\s*$\s+VALUES\s*$\s*([^()]*)\s*$\s*;\s*$)"};
    std::regex selectRegex{R"(^SELECT\s+(\*|[\w\s,]+)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.*?))?(?:\s+ORDER\s+BY\s+([\w\s,]+))?\s*;\s*$)"};
    std::regex updateRegex{R"(^UPDATE\s+(\w+)\s+SET\s+(.*?)\s+WHERE\s+(.*?)\s*;\s*$)"};
    std::regex deleteRegex{R"(^DELETE\s+FROM\s+(\w+)(?:\s+WHERE\s+(.*?))?\s*;\s*$)"};

    // Не не не, помощь ПОМОЩЬ
    static vector<string> splitIdentifiers(const string& input);
    static Condition parseCondition(const string& op);
    static pair<string, string> extractColumnAndValue(const string& condition, string& op);
};

