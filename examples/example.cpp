#include <iostream>
#include <cassert>
#include "json.hpp"

int main() {
  Lexer l(R"({
  "glossary": {
    "title": "example glossary",
    "GlossDiv": {
      "title": "S",
      "GlossList": {
        "GlossEntry": {
          "ID": "SGML",
          "SortAs": "SGML",
          "GlossTerm": "Standard Generalized Markup Language",
          "Acronym": "SGML",
          "Abbrev": "ISO 8879:1986",
          "GlossDef": {
            "para": "A meta-markup language, used to create markup languages such as DocBook.",
            "GlossSeeAlso": [
              "GML",
              "XML"
            ]
          },
          "GlossSee": "markup"
        }
      }
    }
  }
})");
  auto res = Parser{}.parse(l);
  JsonValue *v = std::get_if<0>(&res);
  assert(v != nullptr);
  std::cout << (*v->as_object())[0].first << std::endl;
  std::cout << *v << std::endl;
}