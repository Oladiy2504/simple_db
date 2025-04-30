#include <command_parser.h>

/*
TODO: Расширить парсер, пока что не очень работоспособен и не совсем согласуется с остальными модулями
*/

// Удаление пробелов в начале и конце строки
string SimpleDBParser::trim(const string& str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) {
        return "";
    }
    const size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Разделение строки на идентификаторы
vector<string> SimpleDBParser::splitIdentifiers(const string& input) {
    vector<string> result;
    if (input.empty()) return result;

    const string trimmed = trim(input);
    istringstream ss(trimmed);
    string token;

    while (getline(ss, token, ',')) {
        result.push_back(trim(token));
    }
    return result;
}

Condition SimpleDBParser::parseCondition(const string& op) {
    if (op == "=") return EQUAL;
    if (op == ">") return GREATER;
    if (op == "<") return LESS;
    if (op == "!=") return NOT_EQUAL;
    if (op == ">=") return GREATER_EQUAL;
    if (op == "<=") return LESS_EQUAL;
    return ERROR_COND; // Unknown condition
}

// Разделение строки на значения
pair<string, string> SimpleDBParser::extractColumnAndValue(const string& condition, string& op) {
    const regex conditionPattern(R"((\w+)\s*(=|!=|>|<|>=|<=)\s*('.*?'|".*?"|\S+))");

    if (smatch match; regex_search(condition, match, conditionPattern)) {
        if (match.size() >= 4) {
            op = match[2];
            string value = match[3];
            if ((value.front() == '\'' && value.back() == '\'') ||
                (value.front() == '"' && value.back() == '"')) {
                    if (value.length() >= 2) {
                        value = value.substr(1, value.length() - 2);
                    }
                } else {
                    value = trim(value);
                }
            return make_pair(trim(match[1]), value);
        }
    }
    op = "";
    return make_pair("", "");
}

ParsedCommand SimpleDBParser::parse(const string &command) const {
    ParsedCommand result;
    smatch match;
    const string trimmedCommand = trim(command);
    if (command.empty()) {
        return result;
    }

    // создание таблицы
    if (regex_match(trimmedCommand, match, createTableRegex)) {
        result.type = CREATE_TABLE;
        result.table_name = match[1];
        vector<string> columns = splitIdentifiers(match[2]);
        for (const string& column : columns) {
            vector<string> values = splitIdentifiers(column);
            if (values.size() == 2) {
                result.new_columns.push_back(make_pair(values[0], values[1]));
            }
        }
        return result;
    }

    // вставка данных
    if (regex_match(trimmedCommand, match, insertIntoRegex)) {
        if (match.size() > 2) {
            result.type = INSERT;
            result.table_name = match[1];
            result.columns = match[2];
            result.values = match[3];
        }
        return result;
    }

    // выборка данных
    if (regex_match(trimmedCommand, match, selectRegex)) {
        if (match.size() > 3) {
            result.type = SELECT;
            result.table_name = match[2];

            if (match[1] == "*") {
                result.columns = "*";
                result.where_condition = ALL;
            } else {
                result.columns = match[1];
            }
            string conditionOp;
            auto conditionInfo = extractColumnAndValue(match[3], conditionOp);
            result.where_column = conditionInfo.first;
            result.where_value = conditionInfo.second;
            result.where_condition = parseCondition(conditionOp);
        }
        return result;
    }

    // обновление данных
    if (regex_match(trimmedCommand, match, updateRegex)) {
        if (match.size() > 4) {
            result.type = UPDATE;
            result.table_name = match[1];
            result.columns = match[2];
            result.values = match[3];

            string conditionOp;
            auto conditionInfo = extractColumnAndValue(match[4], conditionOp);
            result.where_column = conditionInfo.first;
            result.where_value = conditionInfo.second;
            result.where_condition = parseCondition(conditionOp);
        }
        return result;
    }

    // удаление данных
    if (regex_match(trimmedCommand, match, deleteRegex)) {
        if (match.size() > 2) {
            result.type = DELETE;
            result.table_name = match[1];
            string conditionOp;
            auto conditionInfo = extractColumnAndValue(match[2], conditionOp);
            result.where_column = conditionInfo.first;
            result.where_value = conditionInfo.second;
            result.where_condition = parseCondition(conditionOp);
        }
        return result;
    }

    // Если ни один из шаблонов не совпал, возвращаем ошибку
    return result;
};