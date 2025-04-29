#include "table.h"

/*
Класс Table является оберткой над FileManager
Он по большей части передает операции методам FileManager, предварительно обрабатывая их, если нужно

TODO: Добавить проверки условий в select и перетащить частично remove из file_manager.cpp
*/

Table::Table(const string &file_path, const string &name,
             const vector<Column> &columns) : file(FileManager(file_path)) {
    file.create_table(name, columns);
}

void Table::insert(const vector<string> &values) {
    file.write_row(values);
}

bool Table::condition_check(const string &curr_value, const string &check_value, Condition condition, int curr_type) {
    auto check_type = static_cast<int>(!data.empty() && all_of(data.begin(), data.end(), ::isdigit));
    if (curr_type != check_type) {
        throw runtime_error("Types mismatch for update");
    }
    if (check_type == 0) {
        if (condition > 0) {
            throw runtime_error("Less or greater condition can not be used with TEXT datatype");
        }
        return curr_value == check_value;
    }
    size_t check_n = stoi(check_value);
    size_t curr_n = stoi(curr_value);
    if (condition == 0) {
        return check_n == curr_n;
    }
    if (condition == 1) {
        return curr_n > check_n;
    }
    return curr_n < check_n;
}


void Table::update(const string &column_name, Condition condition, const string &condition_column, const string &value, const string &condition_value) {
    size_t column_ind = SIZE_MAX;
    size_t condition_column_ind = SIZE_MAX;

    for (size_t i = 0; i < file.columns.size(); ++i) {
        if (file.columns[i].name == column_name) {
            column_ind = i;
        }
        if (file.columns[i].name == condition_column) {
            condition_column_ind = i;
        }
    }

    if (column_ind == SIZE_MAX || condition_column_ind == SIZE_MAX) {
        throw runtime_error("One of the columns not found");
    }

    auto rows = file.select_all();
    for (size_t i = 0; i < rows.size(); ++i) {
        if (condition_check(rows[i][condition_column_ind], condition_value, condition, file.columns[i].type)) {
            rows[i][column_ind] = value;
        }
    }
    if (!rows.empty()) {
        file.update_row(rows);
    }
}

void Table::remove(Condition condition, const string &condition_column, const string &value, const string &condition_value) {
    size_t condition_column_ind = SIZE_MAX;

    for (size_t i = 0; i < file.columns.size(); ++i) {
        if (file.columns[i].name == condition_column) {
            condition_column_ind = i;
        }
    }

    if (condition_column_ind == SIZE_MAX) {
        throw runtime_error("Condition column not found");
    }

    auto rows = file.select_all();
    vector<vector<string>> new_rows;
    copy(begin(rows), end(rows), new_rows);
    for (size_t i = 0; i < rows.size(); ++i) {
        if (condition_check(rows[i][condition_column_ind], condition_value, condition, file.columns[i].type)) {
            new_rows.erase(new_rows.begin() + i);
        }
    }
    if (!new_rows.empty()) {
        file.delete_row(new_rows);
    }
}

vector<vector<string> > Table::select(const string &column_name, Condition condition, const int &num) {
    auto all = file.select_all();
    int id = 0;
    for (int i = 0; i < file.columns.size(); i++) {
        if (column_name == file.columns[i].name) {
            id = i;
            break;
        }
    }
    vector<vector<string> > ans;
    for (auto &i: all) {
        if (condition == 0) {
            try {
                if (stoi(i[id]) == num) {
                    ans.push_back(i);
                }
            } catch (...) {
            }
        }
        if (condition == 1) {
            try {
                if (stoi(i[id]) > num) {
                    ans.push_back(i);
                }
            } catch (...) {
            }
        }
        if (condition == 2) {
            try {
                if (stoi(i[id]) < num) {
                    ans.push_back(i);
                }
            } catch (...) {
            }
        }
    }
    return ans;
}
