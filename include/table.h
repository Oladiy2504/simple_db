#pragma once
#include<includes.h>
#include<file_manager.h>

class Table {
    FileManager file;

public:
    Table(const string& file_path, const string& name, const vector<Column> &columns);

    void insert(const vector<string>& values);

    void update(const string& condition, const vector<string>& new_values);

    void remove(const string& condition);

    vector<vector<string>> select(const string& condition);
};
