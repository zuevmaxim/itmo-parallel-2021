#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "One argument expected, but " << argc << " found" << std::endl;
        return 1;
    }
    std::string input(argv[1]);
    int words = 0;
    bool haveSymbols = false;
    for (char & c : input) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (haveSymbols) {
                words++;
                haveSymbols = false;
            }
        } else {
            haveSymbols = true;
        }
    }
    if (haveSymbols) {
        words++;
    }
    std::cout << words << std::endl;

    return 0;
}
