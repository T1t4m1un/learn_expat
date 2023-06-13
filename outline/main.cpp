#include <expat.h>

#include <cstdlib>

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#define BUFFSIZE 8192
char buffer[BUFFSIZE];

int depth;

static void XMLCALL
start(void *data, const XML_Char *el, const XML_Char **attr) {
    (void)data;

    std::cout << std::string(depth * 2, ' ') << el << " ";

    for (size_t i = 0; attr[i]; i += 2)
        std::cout << attr[i] << "=\"" << attr[i + 1] << "\" ";

    std::cout << '\n';

    depth += 1;
}

static void XMLCALL
end(void *data, const XML_Char *el) {
    (void)data;
    (void)el;

    depth--;
}

int main(int argc, char *argv[]) {
    auto parser =
        std::unique_ptr<
            std::remove_pointer_t<XML_Parser>,
            std::decay_t<decltype(XML_ParserFree)>
        >(XML_ParserCreate(nullptr), XML_ParserFree);
    if (!parser) {
        std::cerr << "Couldn't allocate memory for parser" << std::endl;
        exit(-1);
    }

    XML_SetElementHandler(parser.get(), start, end);

    while (std::cin.peek() != EOF) {
        size_t len;

        len = fread(buffer, 1, BUFFSIZE, stdin);
        if (std::cin.bad()) {
            std::cerr << "Read error" << std::endl;
            exit(-1);
        }

        if (XML_Parse(parser.get(), buffer, len, 0) == XML_STATUS_ERROR) {
            std::cerr << "Parse error at line " << XML_GetCurrentLineNumber(parser.get()) << '\n'
                      << XML_ErrorString(XML_GetErrorCode(parser.get())) << std::endl;
            exit(-1);
        }
    }
    exit(0);
}