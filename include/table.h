#pragma once
#include<includes.h>
#include<file_manager.h>
#include <command_parser.h>

class Table {
    FileManager file;

    bool condition_check(const string &real_value, const string &value, Condition condition, int type);

public:
    Table(const string& file_path, const string& name, const vector<Column> &columns);

    void insert(const vector<string>& values);

    void update(const string &column_name, Condition condition, const string &condition_column, const string &value, const string &condition_value);

    void remove(const string& condition);

    vector<vector<string>> select(const string &column_name, Condition condition, const int &num);
};
