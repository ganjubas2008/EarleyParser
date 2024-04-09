#pragma once
#include "grammar.hpp"

bool IsNonTerm(const Object &symbol, const Grammar &grammar) {
  return grammar.non_terminals.find(symbol) != grammar.non_terminals.end();
}

struct State {
  Rule rule;
  size_t rule_pos = 0;
  size_t i = 0;

  bool IsDone() const { return rule_pos == rule.rhs.size(); }

  Object Get() const { return rule.rhs[rule_pos]; }

  bool CanScan(const Object &token) const {
    return (!IsDone() && Get() == token);
  }

  bool CanPredict(const Grammar &grammar) const {
    return (!IsDone() && IsNonTerm(Get(), grammar));
  }

  bool CanComplete() const { return IsDone(); }

  std::string ToString() const {
    return "(" + rule.ToString() + " " + std::to_string(rule_pos) + " | " +
           std::to_string(i) + ")";
  }

  bool IsEmpty() const { return (rule.rhs.size() == 1) && (rule.rhs[0] == ""); }
};

bool operator==(const State &lhs, const State &rhs) {
  return lhs.ToString() == rhs.ToString();
}

std::ostream &operator<<(std::ostream &os, const State &state) {
  os << state.ToString();
  return os;
}

template <> struct std::hash<State> {
  std::size_t operator()(const State &state) const {
    using std::size_t;
    return hash<std::string>()(state.ToString());
  }
};

#define StateSet std::unordered_set<State>

struct EarleyParser {
  Grammar grammar; // no copy
  std::vector<Object> word;

  size_t j = 0;
  std::vector<StateSet> chart;

  EarleyParser(const Grammar &grammar) : grammar(grammar) {}

  bool Scan(size_t idx) {
    bool change = false;
    for (const State &state : chart[idx]) {
      if (state.CanScan(word[idx])) {
        change |= chart[idx + 1]
                      .insert(State{.rule = state.rule,
                                    .rule_pos = state.rule_pos + 1,
                                    .i = state.i})
                      .second;
      }
    }
    return change;
  }

  bool Predict(size_t idx) {
    bool change = false;
    for (const State &state : chart[idx]) {
      if (state.CanPredict(grammar)) {
        for (const auto &rule_rhs : grammar.rules[state.Get()]) {
          change |= chart[idx]
                        .insert(State{.rule = Rule(state.Get(), rule_rhs),
                                      .rule_pos = 0,
                                      .i = idx})
                        .second;
        }
      }
    }
    return change;
  }

  bool Complete(size_t idx) {
    bool change = false;
    for (const State &state : chart[idx]) {
      if (state.CanComplete() || state.IsEmpty()) {
        size_t k = state.i;
        for (const State &prev_state : chart[k]) {
          if (!prev_state.IsDone() && prev_state.Get() == state.rule.lhs) {
            change |= chart[idx]
                          .insert(State{.rule = prev_state.rule,
                                        .rule_pos = prev_state.rule_pos + 1,
                                        .i = prev_state.i})
                          .second;
          }
        }
      }
    }
    return change;
  }

  bool Predict(const std::vector<Object> &word_init) {
    word = word_init;
    chart.clear();
    j = 0;
    return Predict();
  }

  bool Predict() {
    chart.resize(word.size() + 1);
    State state_init{.rule = Rule("$", {"S"}), .rule_pos = 0, .i = 0};
    chart[0].insert(state_init);

    bool change = true;
    while (change) {
      change = false;
      change |= Complete(0);
      change |= Predict(0);
    }
    for (size_t j = 0; j < word.size(); ++j) {
      Scan(j);
      change = true;
      while (change) {
        change = false;
        change |= Complete(j + 1);
        change |= Predict(j + 1);
      }
    }

    State state_terminal{.rule = Rule("$", {"S"}), .rule_pos = 1, .i = 0};
    return (chart[word.size()].find(state_terminal) !=
            chart[word.size()].end());
  }
};
