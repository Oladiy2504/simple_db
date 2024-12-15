#include "table.h"
#include <file_manager.h>

/*
Класс Table является оберткой над FileManager
Он по большей части передает операции методам FileManager, предварительно обрабатывая их, если нужно

TODO:
    Подумать над форматом condition (договориться с ответственным за парсинг о формате условий)
    Реализовать базовые методы работы с базой данных (см table.h)
    Конечно же где-то понадобится расширить функционал FileManager
    *Кроме того, любые сигнатуры функций в table.h условны и могут быть изменены
*/

Table::Table(const string &file_path, const string &name, const vector<Column> &columns) : file(FileManager(file_path)){
    file.create_table(name, columns);
}

vector<vector<string>> Table::select(const string &column_name, const string &condition, const int &num) {
    vector<vector<string>> all = file.select_all();
    int id = 0;
    for (int i = 0; i < file.columns.size(); i++) {
        if (column_name == file.columns[i].name) {
            id = i;
            break;
        }
    }
    vector<vector<string>> ans;
    for (auto & i : all) {
        if (condition == "=") {
            try {
                if (stoi(i[id]) == num) {
                    ans.push_back(i);
                }
            } catch (...) {}
        }
        if (condition == ">") {
            try {
                if (stoi(i[id]) > num) {
                    ans.push_back(i);
                }
            } catch (...) {}
        }
        if (condition == "<") {
            try {
                if (stoi(i[id]) < num) {
                    ans.push_back(i);
                }
            } catch (...) {}
        }
    }
    return ans;
}