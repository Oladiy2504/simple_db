#include "table.h"

/*
Класс Table является оберткой над FileManager
Он по большей части передает операции методам FileManager, предварительно обрабатывая их, если нужно
*/

bool string_is_integer(const string &s) {
    if (s.empty()) {
        return false;
    }

    int start = 0;
    if ((s[0] == '+' || s[0] == '-') && s.size() > 1) {
        start = 1;
    }
    return std::all_of(s.begin() + start, s.end(),
                       [](unsigned char c) { return isdigit(c); }
    );
}


Table::Table(const string &file_path, const string& table_name) : file(FileManager(file_path)), table_name(table_name) {}

void Table::create_table(const string &name, const vector<Column> &columns) {
    file.create_table(name, columns);
}


void Table::insert(const vector<string> &values) {
    file.write_row(values);
}

bool Table::condition_check(const string &curr_value, const string &check_value, Condition condition, int curr_type) {
    int check_type = string_is_integer(check_value);
    if (curr_type != check_type) {
        throw runtime_error("Types mismatch for update");
    }
    if (check_type == 0) {
        if (condition > 1) {
            throw runtime_error("Less or greater condition can not be used with TEXT datatype");
        }
        return condition == ALL || curr_value == check_value;
    }
    size_t check_n = stoi(check_value);
    size_t curr_n = stoi(curr_value);
    if (condition == EQUAL) {
        return check_n == curr_n;
    }
    if (condition == GREATER) {
        return curr_n > check_n;
    }
    if (condition == LESS) {
        return curr_n < check_n;
    }
    return true;
}

size_t Table::get_column_index(const string &column_name) {
    size_t column_ind = SIZE_MAX;

    for (size_t i = 0; i < file.columns.size(); ++i) {
        if (file.columns[i].name == column_name) {
            column_ind = i;
            break;
        }
    }

    if (column_ind == SIZE_MAX) {
        throw runtime_error(column_name + " column not found");
    }

    return column_ind;
}


void Table::update(const string &column_name, Condition condition, const string &condition_column, const string &value,
                   const string &condition_value) {
    size_t column_ind = get_column_index(column_name);
    size_t condition_column_ind = get_column_index(condition_column);

    vector<size_t> row_indices;
    auto rows = file.select_all();
    for (size_t i = 0; i < rows.size(); ++i) {
        if (condition_check(rows[i][condition_column_ind], condition_value, condition,
                            file.columns[condition_column_ind].type)) {
            row_indices.push_back(i);
        }
    }
    if (!row_indices.empty()) {
        file.update_rows(column_ind, value, row_indices); // Pass the row indices
    }
}

void Table::remove(Condition condition, const string &condition_column, const string &condition_value) {
    size_t condition_column_ind = get_column_index(condition_column);

    auto rows = file.select_all();
    vector<vector<string> > new_rows;
    copy(rows.begin(), rows.end(), new_rows.begin());
    for (size_t i = 0; i < rows.size(); ++i) {
        if (condition_check(rows[i][condition_column_ind], condition_value, condition, file.columns[i].type)) {
            new_rows.erase(new_rows.begin() + i);
        }
    }
    if (!new_rows.empty()) {
        file.overwrite_rows(new_rows);
    }
}

vector<vector<string> > Table::select(Condition condition, const string &condition_column,
                                      const string &condition_value) {
    auto rows = file.select_all();
    size_t condition_column_ind = get_column_index(condition_column);

    if (condition == ALL) {
        return rows;
    }

    vector<vector<string> > filtered_rows;
    for (size_t i = 0; i < rows.size(); ++i) {
        if (condition_check(rows[i][condition_column_ind], condition_value, condition, file.columns[i].type)) {
            filtered_rows.push_back(rows[i]);
        }
    }
    return filtered_rows;
}
