#include "file_manager.h"

/*
В начале файла хранятся все метаданные в следующем порядке:
-Длина названия таблицы (size_t)
-Название таблицы (string)
-Количество столбцов (size_t)
-Длина названия первого столбца (size_t)
-Название первого столбца (string)
-Тип данных в первом столбца (Types)
...
*/

FileManager::FileManager(const string &file_path) {
    this->file_path = file_path;
    ifstream file(file_path, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file while initializing file manager");
    }
    if (file.peek() != EOF) {
        read_metadata();
    }
    file.close();
}

// Чтение мета-данных из файла
void FileManager::read_metadata() {
    ifstream file(file_path, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading metadata");
    }

    size_t length;
    file.read(reinterpret_cast<char *>(&length), sizeof(length));
    table_name.resize(length);
    file.read(&table_name[0], length);

    size_t column_count;
    file.read(reinterpret_cast<char *>(&column_count), sizeof(column_count));
    columns.resize(column_count);
    for (size_t i = 0; i < column_count; ++i) {
        file.read(reinterpret_cast<char *>(&length), sizeof(length));
        columns[i].name.resize(length);
        file.read(&columns[i].name[0], length);

        Types type;
        file.read(reinterpret_cast<char *>(&type), sizeof(type));
        columns[i].type = type;
    }
    file.close();
}

// Запись мета-данных в файл
void FileManager::write_metadata() {
    ofstream file(file_path, ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing metadata");
    }
    size_t length = table_name.length();
    file.write(reinterpret_cast<char *>(&length), sizeof(length));
    file.write(table_name.data(), length);

    size_t column_count = columns.size();
    file.write(reinterpret_cast<char *>(&column_count), sizeof(column_count));
    for (size_t i = 0; i < column_count; ++i) {
        file.write(reinterpret_cast<char *>(&length), sizeof(length));
        file.write(columns[i].name.data(), length);
        size_t type = columns[i].type;
        file.write(reinterpret_cast<char *>(&type), sizeof(&type));
    }
    file.close();
}

// Создание таблицы
void FileManager::create_table(const string &name, const vector<Column> &columns) {
    if (!columns.empty()) {
        throw runtime_error("Trying to overwrite existing table");
    }
    table_name = name;
    this->columns = columns;
    write_metadata();
}

// Проверка того, что переданные данные соответствуют ожидаемым для столбца
void FileManager::check_column_datatype(const string &data, size_t i) {
    Types type = columns[i].type;
    auto real_type = static_cast<Types>(!data.empty() && all_of(data.begin(), data.end(), ::isdigit));
    if (type != real_type) {
        string expected_data = type == 1 ? "INT" : "TEXT";
        string real_data = real_type == 1 ? "INT" : "TEXT";
        throw runtime_error("Wrong column data type: " + real_data + ". Expected: " + expected_data);
    }
}

// Запись строки с данными в конец файла
void FileManager::write_row(const vector<string> &row) {
    if (row.size() != columns.size()) {
        throw runtime_error("Row size does not match table columns count");
    }
    ofstream file(file_path, ios::binary | ios::app);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing row");
    }
    for (size_t i = 0; i < row.size(); ++i) {
        check_column_datatype(row[i], i);
        size_t length = row[i].length();
        file.write(reinterpret_cast<char *>(&length), sizeof(length));
        file.write(row[i].data(), length);
    }
    file.close();
}

// Возвращает все строки таблицы (в порядке добавления)
vector<vector<string> > FileManager::select_all() {
    if (columns.empty()) {
        throw runtime_error("No columns found in file!");
    }

    ifstream file(file_path, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for selecting");
    }

    file.seekg(0, ios::end);
    size_t file_length = file.tellg();
    size_t metadata_size = sizeof(size_t) + table_name.size();
    for (const auto &col: columns) {
        metadata_size += sizeof(size_t);
        metadata_size += col.name.size();
        metadata_size += sizeof(Types);
    }
    file.seekg(static_cast<streamoff>(metadata_size), ios::beg);

    vector<vector<string> > rows;
    while (file.tellg() < static_cast<streampos>(file_length)) {
        vector<string> row;
        for (size_t i = 0; i < columns.size(); ++i) {
            size_t length;
            file.read(reinterpret_cast<char *>(&length), sizeof(length));

            string value(length, '\0');
            file.read(&value[0], length);
            row.push_back(value);

            Types type;
            file.read(reinterpret_cast<char *>(&type), sizeof(type));
        }
        rows.push_back(row);
    }
    return rows;
}

// Находит строки с указанным значением в столбце условия и заменяет значение в обновляемом столбце на новое
void FileManager::update_row(vector<vector<string>> &rows) {
    ofstream file(file_path, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing updated rows");
    }

    file.seekp(0, ios::end);
    size_t file_length = file.tellp();
    size_t metadata_size = sizeof(size_t) + table_name.size();
    for (const auto &col: columns) {
        metadata_size += sizeof(size_t);
        metadata_size += col.name.size();
        metadata_size += sizeof(Types);
    }
    file.seekp(static_cast<streamoff>(metadata_size), ios::beg);

    for (const auto &row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            check_column_datatype(row[i], i);
            size_t length = row[i].length();
            file.write(reinterpret_cast<char *>(&length), sizeof(length));
            file.write(row[i].data(), length);
        }
    }
    file.close();
}

// Читает все строки, фильтрует те, которые соответствуют условию, и записывает оставшиеся строки обратно в файл
void FileManager::delete_row(const string &condition_column, const string &condition_value) {
    if (columns.empty()) {
        throw runtime_error("No columns found in the table!");
    }

    size_t condition_index = SIZE_MAX;
    for (size_t i = 0; i < columns.size(); ++i) {
        if (columns[i].name == condition_column) {
            condition_index = i;
            break;
        }
    }

    if (condition_index == SIZE_MAX) {
        throw runtime_error("Condition column not found");
    }

    vector<vector<string>> rows = select_all();

    vector<vector<string>> filtered_rows;
    for (const auto &row : rows) {
        if (row[condition_index] != condition_value) {
            filtered_rows.push_back(row);
        }
    }

    ofstream file(file_path, ios::binary | ios::trunc);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing remaining rows");
    }

    for (const auto &row : filtered_rows) {
        for (const auto &data : row) {
            size_t length = data.length();

            file.write(reinterpret_cast<char *>(&length), sizeof(length));
            file.write(data.data(), length);
        }
    }
    file.close();
}

