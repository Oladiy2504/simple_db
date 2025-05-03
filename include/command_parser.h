#pragma once
#include <includes.h>
#include<file_manager.h>

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

    vector<Column> new_columns; // {name, type}

    vector<string> columns;
    vector<string> values;
    string where_column;
    Condition where_condition = ALL;
    string where_value;
};

class SimpleDBParser {
public:
    static string trim(const string &str);

    static Types string_to_type(string &s);

    ParsedCommand parse(const string &command) const;

private:
    std::regex createRegex{
        R"(^CREATE\s+TABLE\s+(\w+)\s*\(\s*([^)]*)\s*\)\s*;?\s*$)",
        std::regex_constants::icase
    };
    std::regex insertRegex{
        R"(^INSERT\s+INTO\s+(\w+)\s*\(\s*(\w+(?:\s*,\s*\w+)*)\s*\)\s+VALUES\s*\(\s*((?:\d+|'.*?'|\".*?\")(?:\s*,\s*(?:\d+|'.*?'|\".*?\"))*)\s*\)\s*;?\s*$)",
        std::regex_constants::icase
    };
    std::regex selectRegex{
        R"(^SELECT\s+(\*|\(\s*\w+(?:\s*,\s*\w+)*\s*\))\s+FROM\s+(\w+)(?:\s+WHERE\s+(\w+)\s*(=|!=|<=|>=|<|>)\s*(\d+|\"[^\"]*\"))?\s*;?\s*$)",
        std::regex_constants::icase
    };
    std::regex updateRegex{R"(^UPDATE\s+(\w+)\s+SET\s+(.*?)\s+WHERE\s+(.*?)\s*;\s*$)"};
    std::regex deleteRegex{R"(^DELETE\s+FROM\s+(\w+)(?:\s+WHERE\s+(.*?))?\s*;\s*$)"};

    // Не не не, помощь ПОМОЩЬ
    static vector<string> splitIdentifiers(const string &input, char delim = ' ');

    static Condition parseCondition(const string &op);

    static pair<string, string> extractColumnAndValue(const string &condition, string &op);
};
