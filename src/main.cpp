#include<includes.h>
#include <table.h>
#include <command_parser.h>

/*
main принимает ввод команд для баз данных в папке data (см. README)
*/


int main() {
    string line;

    cout << "Petrovich, vrubai nasos!" << endl;

    unordered_map<string, Table> tables;

    while (true) {
        SimpleDBParser parser;
        cout << "simpledb> ";
        getline(cin, line);

        if (line == "QUIT") {
            std::cout << "Petrovich, virubai nasos!" << endl;
            break;
        }

        if (line.empty()) {
            continue;
        }

        ParsedCommand cmd = parser.parse(line);

        string file_path = "../data/" + cmd.table_name;

        if (!tables.contains(cmd.table_name)) {
            tables.emplace(cmd.table_name, Table(file_path, cmd.table_name));
        }

        if (cmd.type == CREATE_TABLE) {
            tables.at(cmd.table_name).create_table(cmd.table_name, cmd.new_columns);
        } else if (cmd.type == SELECT) {
            auto result = tables.at(cmd.table_name).select(cmd.where_condition, cmd.where_column, cmd.where_value);
            for (auto &row: result) {
                for (auto &col: row) {
                    cout << col << ' ';
                }
                cout << '\n';
            }
        } else if (cmd.type == INSERT) {
            tables.at(cmd.table_name).insert(cmd.values);
        }
    }
}
