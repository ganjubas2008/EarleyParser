#include "test_earley.hpp"

bool TestDir(const std::string& dirname) {
    Grammar grammar = GrammarReader().Read(dirname + "/grammar.txt");
    std::vector<TestWord> testwords = TestWordsReader().Read(dirname + "/test_words.txt");
    EarleyParser ep(grammar);

    bool okay = true;
    for (const auto& testword : testwords) {
        okay &= (ep.Predict(testword.word) == testword.ans);
    }
    return true;
}

TEST_F(EarleyParserTest, TestAddition) {
  ASSERT_TRUE((1 + 1 == 2));
}

TEST_F(EarleyParserTest, TestEarleyManual) {
  Rule r0("S", {"A"});
  Rule r1("A", {"B", "a"});
  Rule r2("B", {"b"});
  Rule r3("A", {"B", "A", "B"});
  Rule r4("A", {"a", "A"});
  Grammar grammar({r0, r1, r2, r3, r4});
  EarleyParser ep(grammar);
  ASSERT_TRUE(ep.Predict({"a", "b", "a"}));
  ASSERT_TRUE(ep.Predict({"b", "a", "a", "b", "a", "b"}));
  ASSERT_TRUE(ep.Predict({"b", "a", "b", "a", "b"}));
  ASSERT_TRUE(ep.Predict({"b", "b", "a", "b"}));

  ASSERT_FALSE(ep.Predict({"b", "a", "b"}));
  ASSERT_FALSE(ep.Predict({"b"}));
  ASSERT_FALSE(ep.Predict({""}));
}

TEST_F(EarleyParserTest, TestEarleyBracketSequences) {
    ASSERT_TRUE(TestDir("../tests/test_bs"));
}

TEST_F(EarleyParserTest, TestEarleyBigBracketSequences) {
    ASSERT_TRUE(TestDir("../tests/test_big_bs"));
}

TEST_F(EarleyParserTest, TestEarleySimple1) {
    ASSERT_TRUE(TestDir("../tests/test_simple1"));
}

TEST_F(EarleyParserTest, TestEarleySimple2) {
    ASSERT_TRUE(TestDir("../tests/test_simple2"));
}
