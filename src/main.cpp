#include<includes.h>
#include <table.h>
#include <command_parser.h>

/*
main принимает ввод команд для баз данных в папке data (см. README)

TODO: реализовать ввод команд и их проверку
*/


int main() {
    std::string line;

    std::cout << "Петрович, врубай насос" << std::endl;

    while (true) {
        SimpleDBParser parser;
        std::cout << "simpledb> ";
        std::getline(std::cin, line);

        if (line == "QUIT") {
            std::cout << "Петрович, вырубай насос" << std::endl;
            break;
        }

        if (line.empty()) continue;

        ParsedCommand cmd = parser.parse(line);

        switch (cmd.type) {
            case CREATE_TABLE:
                // делай стол

            case INSERT:
                // делай вставку

            case SELECT:
                // делай выборку

            case UPDATE:
                // делай обновление

            case DELETE:
                // делай удаление

            case ERROR:
            default:
                std::cerr << "Ну ты и тупой, Петрович!" << std::endl;
        }
    }
}