#include <getopt.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>

#include "earley.hpp"
#include "reader.hpp"
using namespace std;

class ArgumentParser {
public:
  string grammar_file;
  string word_file;

  void ParseArguments(int argc, char **argv) {
    int opt;
    int option_index;

    while ((opt = getopt_long(argc, argv, "g:w:", kLongOptions,
                              &option_index)) != -1) {
      switch (opt) {
      case 'g':
        grammar_file = optarg;
        break;
      case 'w':
        word_file = optarg;
        break;
      default:
        PrintUsage(cerr);
        throw invalid_argument("");
      }
    }
    ValidateArguments(grammar_file, word_file);
  }

private:
  static constexpr option kLongOptions[] = {
      {"grammar-file", required_argument, NULL, 'g'},
      {"word-file", required_argument, NULL, 'w'},
      {0, 0, 0}};

  static void PrintUsage(ostream &os) {
    static const string kUsage = "Usage: run [-g <GRAMMAR_FILE>]";
    os << kUsage << endl;
  }

  static void ValidateArguments(const string &grammar_file,
                                const string &word_file) {
    if (grammar_file.empty()) {
      PrintUsage(cerr);
      throw invalid_argument("Grammar file is not given");
    }
    if (word_file.empty()) {
      PrintUsage(cerr);
      throw invalid_argument("Word file is not given");
    }
  }
};

int main(int argc, char *argv[]) {
  ArgumentParser argparser;
  argparser.ParseArguments(argc, argv);

  Grammar grammar = GrammarReader().Read(argparser.grammar_file);
  vector<Object> word = WordReader().Read(argparser.word_file);

  EarleyParser parser(grammar);

  cout << (parser.Predict(word) ? "TRUE" : "FALSE");

  return 0;
}
