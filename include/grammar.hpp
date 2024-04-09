#pragma once
#include <bits/stdc++.h>

#define Object std::string
#define Word std::vector<Object>

const std::string kImplies = "::=";
const std::string kDelimiter = "|";
const std::string kLeftBound = "<";
const std::string kRightBound = ">";

struct Rule {
  Object lhs;
  Word rhs;
  Rule() = default;
  Rule(Object lhs, const Word &rhs) : lhs(lhs), rhs(rhs) {}

  std::string ToString() const {
    std::string str = lhs + kImplies;
    for (const auto &obj : rhs) {
      str += kLeftBound + obj + kRightBound;
    }
    return str;
  }
};

struct Grammar {
  std::unordered_set<Object> non_terminals;
  std::unordered_map<Object, std::vector<Word>> rules;

  Grammar() = default;
  Grammar(const std::vector<Rule> &rules_vector) { // no copy
    for (const Rule &rule : rules_vector) {
      non_terminals.insert(rule.lhs);
      rules[rule.lhs].push_back(rule.rhs);
    }
  }

  Grammar(const Grammar &other) { // no copy
    non_terminals = other.non_terminals;
    rules = other.rules;
  }

  void AddRule(const Rule &rule) {
    non_terminals.insert(rule.lhs);
    rules[rule.lhs].push_back(rule.rhs);
  }
};
