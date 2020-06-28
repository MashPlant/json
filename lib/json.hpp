#pragma once

#include <cstdio>
#include <string_view>
#include <variant>
#include <vector>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

struct JsonValue {
  using Object = std::vector<std::pair<std::string_view, JsonValue>>;

  std::variant<
    std::monostate, // null
    bool,
    double,
    std::string_view, // borrowed string
    std::vector<JsonValue>,
    Object
  > inner;

  JsonValue() = default; // null
  explicit JsonValue(bool v) : inner(v) {}
  explicit JsonValue(double v) : inner(v) {}
  explicit JsonValue(std::string_view v) : inner(v) {}
  explicit JsonValue(std::vector<JsonValue> &&v) : inner(std::move(v)) {}
  explicit JsonValue(Object &&v) : inner(std::move(v)) {}

  bool *as_bool() { return std::get_if<bool>(&inner); }
  double *as_number() { return std::get_if<double>(&inner); }
  std::string_view *as_string() { return std::get_if<std::string_view>(&inner); }
  std::vector<JsonValue> *as_array() { return std::get_if<std::vector<JsonValue>>(&inner); }
  Object *as_object() { return std::get_if<Object>(&inner); }

  // cpp style serialization
  template<typename OS>
  friend OS &operator<<(OS &os, const JsonValue &x) { return x.serialize(os); }

  // helper for operator<<
  template<typename OS>
  OS &serialize(OS &os, u32 indent = 0) const {
    return std::visit([=, &os](auto &&x) -> OS & {
      using T = std::decay_t<decltype(x)>;
      bool first = true;
      u32 nxt = indent + 2;
      if constexpr (std::is_same_v<T, std::monostate>) return os << "null";
      else if constexpr (std::is_same_v<T, bool>) return os << (x ? "true" : "false");
      else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, std::string_view>) return os << x;
      else if constexpr (std::is_same_v<T, std::vector<JsonValue>>) {
        os << "[\n";
        for (auto &&e : x) {
          if (!first) os << ",\n";
          first = false;
          for (u32 i = 0; i < nxt; ++i) os << ' ';
          e.serialize(os, nxt);
        }
        os << '\n';
        for (u32 i = 0; i < indent; ++i) os << ' ';
        return os << ']';
      } else if constexpr (std::is_same_v<T, Object>) {
        os << "{\n";
        for (auto &&[k, v] : x) {
          if (!first) os << ",\n";
          first = false;
          for (u32 i = 0; i < nxt; ++i) os << ' ';
          os << k << ": ";
          v.serialize(os, nxt);
        }
        os << '\n';
        for (u32 i = 0; i < indent; ++i) os << ' ';
        return os << '}';
      }
    }, inner);
  }

  // c style serialization
  void serialize(FILE *f, u32 indent = 0) const {
    std::visit([=](auto &&x) {
      using T = std::decay_t<decltype(x)>;
      bool first = true;
      u32 nxt = indent + 2;
      if constexpr (std::is_same_v<T, std::monostate>) fputs("null", f);
      else if constexpr (std::is_same_v<T, bool>) fputs(x ? "true" : "false", f);
      else if constexpr (std::is_same_v<T, double>) fprintf(f, "%lf", x);
      else if constexpr (std::is_same_v<T, std::string_view>) fprintf(f, "%.*s", (u32) x.size(), x.data());
      else if constexpr (std::is_same_v<T, std::vector<JsonValue>>) {
        fputs("[\n", f);
        for (auto &&e : x) {
          if (!first) fputs(",\n", f);
          first = false;
          fprintf(f, "%*c", nxt, ' ');
          e.serialize(f, nxt);
        }
        // %*c + indent + ' ' prints one space when indent == 0, which is not desirable
        // instead, %*s + indent + "" prints nothing when indent == 0, so we use it
        fprintf(f, "\n%*s]", indent, "");
      } else if constexpr (std::is_same_v<T, Object>) {
        fputs("{\n", f);
        for (auto &&[k, v] : x) {
          if (!first) fputs(",\n", f);
          first = false;
          fprintf(f, "%*c%.*s: ", nxt, ' ', (u32) k.size(), k.data());
          v.serialize(f, nxt);
        }
        fprintf(f, "\n%*s}", indent, "");
      }
    }, inner);
  }
};

struct Token {
  enum Kind : u8 { _Eps, _Eof, _Err, LBrk, RBrk, LBra, RBra, Colon, Comma, Null, True, False, Number, String, } kind;
  std::string_view piece;
  u32 line, col;
};

struct Lexer {
  std::string_view string;
  u32 line, col;

  explicit Lexer(std::string_view string) : string(string), line(1), col(1) {}

  Token next();
};

struct Parser {
  std::variant<JsonValue, Token> parse(Lexer &lexer);
};