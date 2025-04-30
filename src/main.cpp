#include<includes.h>
#include <table.h>
#include <command_parser.h>

/*
main принимает ввод команд для баз данных в папке data (см. README)
*/


int main() {
    string line;

    cout << "Petrovich, vrubai nasos!" << endl;

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

        unordered_map<string, Table> tables;

        string file_path = "../data/" + cmd.table_name;

        switch (cmd.type) {
            case CREATE_TABLE:
                if (tables.find(cmd.table_name) == tables.end()) {
                    tables.emplace(cmd.table_name, Table(file_path, cmd.table_name));
                }
                tables.at(cmd.table_name).create_table(cmd.table_name, cmd.new_columns);
                break;

            case SELECT:
                // тут код

            case INSERT:
                // тут код

            case UPDATE:
                // тут код

            case DELETE:
                // тут код

            case ERROR:
            default:
                std::cerr << "Nu ti i tupoi, Petrovich!" << endl;
        }
    }
}