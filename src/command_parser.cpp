#include <command_parser.h>


string SimpleDBParser::trim(const string& str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    const size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}


pair<int, smatch> SimpleDBParser::parse(const string& command) {

    const string trimmedCommand = trim(command.substr(command.find('>') + 1));
    smatch match;

    if (regex_match(trimmedCommand, match, createTableRegex)) {
        // создание новой таблицы
        return make_pair(1, match);
    }

    if (regex_match(trimmedCommand, match, insertIntoRegex)) {
        // вставить значение
        return make_pair(2, match);
    }

    if (regex_match(trimmedCommand, match, selectRegex)) {
        // поиск записи
        return make_pair(3, match);
    }

    if (regex_match(trimmedCommand, match, updateRegex)) {
        // обновление записи
        return make_pair(4, match);
    }

    if (regex_match(trimmedCommand, match, deleteRegex)) {
            // удаление записи
            return make_pair(5, match);
    }

    return make_pair(-1, match);
};