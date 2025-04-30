#pragma once
#include<includes.h>
#include<file_manager.h>
#include<command_parser.h>

class Table {
    FileManager file;

    string table_name;

    bool condition_check(const string &real_value, const string &value, Condition condition, int type);

    size_t get_column_index(const string &column_name);

public:
    explicit Table(const string& file_path, const string& table_name);

    void create_table(const string& name, const vector<Column> &columns);

    void insert(const vector<string>& values);

    void update(const string &column_name, Condition condition, const string &condition_column, const string &value, const string &condition_value);

    void remove(Condition condition, const string &condition_column, const string &condition_value);

    vector<vector<string>> select(Condition condition, const string &condition_column, const string &condition_value);
};

static bool string_is_integer(const std::string& s);
