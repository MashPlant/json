#include <cstdint>
#include <string_view>
#include <vector>
#include <variant>
#include <utility>

#include <cstdlib>
#include "json.hpp"

Token Lexer::next() {
  const static Token::Kind ACC[] = {Token::_Err, Token::Colon, Token::Number, Token::RBra, Token::_Err, Token::_Eps, Token::_Err, Token::Comma, Token::LBra, Token::RBrk, Token::_Err, Token::_Err, Token::LBrk, Token::_Err, Token::_Err, Token::Number, Token::_Err, Token::_Err, Token::String, Token::_Err, Token::_Err, Token::Number, Token::_Err, Token::_Err, Token::_Err, Token::_Err, Token::Null, Token::True, Token::_Err, Token::False,};
  const static u8 EC[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 22, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 19, 20, 19, 18, 0, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 13, 12, 0, 0, 0, 11, 0, 0, 0, 10, 9, 0, 0, 0, 0, 0, 8, 0, 7, 0, 0, 0, 6, 5, 4, 3, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
  const static u8 DFA_EDGE[][23] = {{0, 3, 8, 0, 11, 0, 0, 6, 0, 13, 0, 0, 9, 0, 12, 0, 1, 2, 0, 4, 7, 10, 5, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 2, 15, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, }, {0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 17, 10, 10, 10, 10, 10, 10, 10, 18, 10, }, {0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 22, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 14, 0, 15, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }, };
  u32 l = line, c = col;
  Token::Kind last_acc = Token::_Err;
  u32 state = 0, i = 0;
  while (i < string.size()) {
    u8 ch = string[i];
    u8 ec = EC[ch];
    u8 nxt = DFA_EDGE[state][ec];
    Token::Kind acc = ACC[nxt];
    if (acc != Token::_Err) { last_acc = acc; }
    state = nxt;
    if (nxt == 0) { // dead, should not eat this char
      // we are not using substr here, because it does range check and may throws exception
      std::string_view piece(string.data(), i);
      string = std::string_view(string.data() + i, string.size() - i);
      if (last_acc != Token::_Eps) {
        return Token{last_acc, piece, l, c};
      } else {
        l = line, c = col, last_acc = Token::_Err, state = 0, i = 0;
      }
    } else { // continue, eat this char
      if (ch == '\n') { ++line, col = 1; } else { ++col; }
      ++i;
    }
  }
  // end of file
  std::string_view piece(string.data(), i);
  string = "";
  return last_acc != Token::_Eps && i != 0 ? Token{last_acc, piece, l, c} : Token{Token::_Eof, "", line, col};
}

std::variant<JsonValue, Token> Parser::parse(Lexer &lexer) {
  using StackItem = std::variant<Token, JsonValue, std::vector<JsonValue>, JsonValue::Object>;
  struct Act {
    enum : u8 { Shift, Reduce, Acc, Err } kind;
    u8 val;
  };
  const static u8 PROD[] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, };
  const static Act ACTION[][14] = {{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 1}, {Act::Err, 0}, {Act::Shift, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 3}, {Act::Shift, 4}, {Act::Shift, 5}, {Act::Shift, 6}, {Act::Shift, 7}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 1}, {Act::Reduce, 8}, {Act::Shift, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 3}, {Act::Shift, 4}, {Act::Shift, 5}, {Act::Shift, 6}, {Act::Shift, 7}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 12}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 12}, },{{Act::Err, 0}, {Act::Reduce, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 0}, {Act::Err, 0}, {Act::Reduce, 0}, {Act::Err, 0}, {Act::Reduce, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Reduce, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 2}, {Act::Err, 0}, {Act::Reduce, 2}, {Act::Err, 0}, {Act::Reduce, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Reduce, 1}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 1}, {Act::Err, 0}, {Act::Reduce, 1}, {Act::Err, 0}, {Act::Reduce, 1}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Reduce, 3}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 3}, {Act::Err, 0}, {Act::Reduce, 3}, {Act::Err, 0}, {Act::Reduce, 3}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Reduce, 4}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 4}, {Act::Err, 0}, {Act::Reduce, 4}, {Act::Err, 0}, {Act::Reduce, 4}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Acc, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 10}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 10}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 15}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 7}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 16}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 17}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 18}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 11}, {Act::Err, 0}, {Act::Shift, 19}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Reduce, 5}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 5}, {Act::Err, 0}, {Act::Reduce, 5}, {Act::Err, 0}, {Act::Reduce, 5}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 1}, {Act::Err, 0}, {Act::Shift, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 3}, {Act::Shift, 4}, {Act::Shift, 5}, {Act::Shift, 6}, {Act::Shift, 7}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 1}, {Act::Err, 0}, {Act::Shift, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 3}, {Act::Shift, 4}, {Act::Shift, 5}, {Act::Shift, 6}, {Act::Shift, 7}, },{{Act::Err, 0}, {Act::Reduce, 6}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 6}, {Act::Err, 0}, {Act::Reduce, 6}, {Act::Err, 0}, {Act::Reduce, 6}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 22}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 9}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 9}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 14}, {Act::Err, 0}, {Act::Reduce, 14}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 23}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 1}, {Act::Err, 0}, {Act::Shift, 2}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Shift, 3}, {Act::Shift, 4}, {Act::Shift, 5}, {Act::Shift, 6}, {Act::Shift, 7}, },{{Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Reduce, 13}, {Act::Err, 0}, {Act::Reduce, 13}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, {Act::Err, 0}, },};
  const static u8 GOTO[][6] = {{8, 0, 0, 0, 0, 0, }, {9, 10, 11, 0, 0, 0, }, {0, 0, 0, 13, 14, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {20, 0, 0, 0, 0, 0, }, {21, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, {24, 0, 0, 0, 0, 0, }, {0, 0, 0, 0, 0, 0, }, };
  std::vector<std::pair<StackItem, u8>> stk;
  stk.emplace_back(StackItem{}, 0);
  u32 state = 0;
  Token token = lexer.next();
  while (true) {
    Act act = ACTION[state][token.kind];
    switch (act.kind) {
      case Act::Shift: {
        stk.emplace_back(token, act.val);
        state = act.val;
        token = lexer.next();
        break;
      }
      case Act::Reduce: {
        StackItem __;
        // we are confident that the value must be what we expect, so directly dereference the return value of `std::get_if`
        // we are not using `std::get`, because it performs runtime check, and throws exceptions when fails
        switch (act.val) {
          case 0: {
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue();
            break;
          }
          case 1: {
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(false);
            break;
          }
          case 2: {
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(true);
            break;
          }
          case 3: {
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(atof(_1.piece.data()));
            break;
          }
          case 4: {
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(_1.piece);
            break;
          }
          case 5: {
            [[maybe_unused]] Token _3(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] std::vector<JsonValue> _2(
              std::move(*std::get_if<std::vector<JsonValue>>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(std::move(_2));
            break;
          }
          case 6: {
            [[maybe_unused]] Token _3(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] JsonValue::Object _2(std::move(*std::get_if<JsonValue::Object>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue(std::move(_2));
            break;
          }
          case 7: {
            [[maybe_unused]] std::vector<JsonValue> _1(
              std::move(*std::get_if<std::vector<JsonValue>>(&stk.back().first)));
            stk.pop_back();
            __ = std::move(_1);
            break;
          }
          case 8: {
            __ = std::vector<JsonValue>();
            break;
          }
          case 9: {
            [[maybe_unused]] JsonValue _3(std::move(*std::get_if<JsonValue>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _2(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] std::vector<JsonValue> _1(
              std::move(*std::get_if<std::vector<JsonValue>>(&stk.back().first)));
            stk.pop_back();
            _1.push_back(std::move(_3));
            __ = std::move(_1);
            break;
          }
          case 10: {
            [[maybe_unused]] JsonValue _1(std::move(*std::get_if<JsonValue>(&stk.back().first)));
            stk.pop_back();
            __ = std::vector{std::move(_1)};
            break;
          }
          case 11: {
            [[maybe_unused]] JsonValue::Object _1(std::move(*std::get_if<JsonValue::Object>(&stk.back().first)));
            stk.pop_back();
            __ = std::move(_1);
            break;
          }
          case 12: {
            __ = JsonValue::Object();
            break;
          }
          case 13: {
            [[maybe_unused]] JsonValue _5(std::move(*std::get_if<JsonValue>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _4(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _3(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _2(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] JsonValue::Object _1(std::move(*std::get_if<JsonValue::Object>(&stk.back().first)));
            stk.pop_back();
            _1.push_back({_3.piece, std::move(_5)});
            __ = std::move(_1);
            break;
          }
          case 14: {
            [[maybe_unused]] JsonValue _3(std::move(*std::get_if<JsonValue>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _2(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            [[maybe_unused]] Token _1(std::move(*std::get_if<Token>(&stk.back().first)));
            stk.pop_back();
            __ = JsonValue::Object{{_1.piece, std::move(_3)}};
            break;
          }
          case 15: {
            [[maybe_unused]] JsonValue _1(std::move(*std::get_if<JsonValue>(&stk.back().first)));
            stk.pop_back();
            __ = _1;
            break;
          }

          default:
            __builtin_unreachable();
        }
        u8 nxt = GOTO[stk.back().second][PROD[act.val]];
        stk.emplace_back(std::move(__), nxt);
        state = nxt;
        break;
      }
      case Act::Acc:
        return std::move(*std::get_if<JsonValue>(&stk.back().first));
      case Act::Err:
        return token;
      default:
        __builtin_unreachable();
    }
  }
}