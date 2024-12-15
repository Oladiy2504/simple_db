#include "table.h"

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
