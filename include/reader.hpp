#pragma once
#pragma once
#include "grammar.hpp"

struct Reader {
  static std::vector<std::string> SplitString(const std::string &input,
                                              const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = input.find(delimiter, start)) != std::string::npos) {
      if (end != start) {
        tokens.push_back(input.substr(start, end - start));
      } else {
        tokens.push_back("");
      }
      start = end + delimiter.length();
    }
    if (start != input.length()) {
      tokens.push_back(input.substr(start));
    } else {
      tokens.push_back("");
    }
    return tokens;
  }

  static std::string TrimString(const std::string &input) {
    size_t start = input.find_first_not_of(" \t\n\r\f\v" + kLeftBound);
    size_t end = input.find_last_not_of(" \t\n\r\f\v" + kRightBound);
    if (start == std::string::npos) {
      return ""; // Input std::string contains only empty characters
    }
    return input.substr(start, end - start + 1);
  }
};

struct GrammarReader : Reader {
  static std::vector<Rule> ReadRules(const std::string &input) {
    std::vector<Rule> rules;
    std::vector<std::string> lhsrhss = SplitString(input, kImplies);
    Object lhs = TrimString(lhsrhss[0]);
    std::string rhss = lhsrhss[1];
    for (const auto &word : SplitString(rhss, kDelimiter)) {
      auto trimmed_word = TrimString(word);
      Word rhs = SplitString(trimmed_word, static_cast<std::string>(kRightBound + kLeftBound));
      rules.push_back(Rule(lhs, rhs));
    }
    return rules;
  }

  static Grammar Read(const std::string &filename) {
    Grammar grammar;
    std::ifstream myfile(filename);
    std::string input_line;
    if (myfile.is_open()) {
      while (myfile.good()) {
        std::getline(myfile, input_line);
        if (!input_line.empty()) {
          for (const auto &rule : ReadRules(input_line)) {
            grammar.AddRule(rule);
          }
        }
      }
    } else {
      throw std::invalid_argument("Invalid file\n");
    }
    return grammar;
  }
};

struct WordReader: Reader {
  static Word Read(const std::string &filename) {
    std::ifstream myfile(filename);
    if (!myfile.is_open()) {
      throw std::invalid_argument("Invalid file\n");
    }
    std::string input_line;
    std::getline(myfile, input_line);
    return SplitString(TrimString(input_line), kRightBound + kLeftBound);
  }
};

struct TestWord {
  Word word;
  bool ans;
};

struct TestWordsReader : Reader {
  static TestWord ReadTestWord(const std::string &input) {
    std::vector<std::string> lhsrhs = SplitString(input, kImplies);
    Word lhs = SplitString(TrimString(lhsrhs[0]), kRightBound + kLeftBound);
    Object rhs = TrimString(lhsrhs[1]);
    return TestWord{.word = lhs, .ans = static_cast<bool>(rhs[0] - '0')};
  }

  std::vector<TestWord> Read(const std::string &filename) {
    std::vector<TestWord> testwords;
    std::ifstream myfile(filename);
    std::string input_line;
    if (myfile.is_open()) {
      while (myfile.good()) {
        std::getline(myfile, input_line);
        if (!input_line.empty()) {
          testwords.push_back(ReadTestWord(input_line));
        }
      }
    } else {
      throw std::invalid_argument("Invalid file\n");
    }
    return testwords;
  }
};