#pragma once
#include<includes.h>

enum Types {
    TEXT = 0,
    INT = 1
};

struct Column {
    string name;
    Types type;
};

class FileManager {
    string file_path;

    string table_name;

    void read_metadata();

    void write_metadata();

    void check_column_datatype(const string &data, size_t i);

public:
    vector<Column> columns;

    explicit FileManager(const string &file_path);

    void create_table(const string &name, const vector<Column> &columns);

    void write_row(const vector<string> &row);

    void update_row(const string &column_name, const string &new_value, const string &condition_column, const string &condition_value);

    void delete_row(const string &condition_column, const string &condition_value);

    vector<vector<string>> select_all();
};
