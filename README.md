Implementation of Earley Parser (https://en.wikipedia.org/wiki/Earley_parser).

## Instructions for Running the Code
* To run the code, follow these steps:
    ```bash
    cd <path_to_dir>
    mkdir build
    cd build
    cmake ..
    make
    ../bin/run_main -g <path_to_grammar> -w <path_to_word>
    ```
    TRUE/FALSE will be outputted to the console \\
    The word should be specified as <C><L><O><V><O>

## Instructions for Running and Viewing Tests
* To run the tests, follow these steps:
    ```bash
    cd <path_to_dir>
    mkdir build
    cd build
    cmake ..
    make
    ../bin/test_main
    ```

* To view the tests, navigate to the tests folder, which contains subfolders:
    * test_bs - numerous tests with small bracket sequences and context-free grammar
    * test_big_bs - few tests with large bracket sequences of size 100, demonstrating asymptotic behavior
    * test_simple1, test_simple2 - arbitrary grammars
* Each subfolder contains two files:
    * grammar.txt - contains the grammar in Backus-Naur form
    * test_words.txt - contains pairs Word, bool - where bool = 1 if the word Word is derivable from the respective grammar, and 0 otherwise

The data format is not entirely trivial but extremely convenient.
## Data Format
* All rules have the form Object ::= Word, in this specific case, Object is a string, and Word is a vector<string>,
    * meaning we have rules like "Don't" ::= "stop" "me" "now", or "S" ::= "A" "B", "A" ::= "a", "B" ::= "b"
* Grammars are stored in Backus-Naur form (https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form), all words are stored in the same <format>,
    * (the only request - do not put spaces between characters, i.e., like this <A><B> - good, like this: <A> <B> - bad, otherwise, I won't be able to parse it)
    * there is also a TestWord type, only needed for tests, this object is a pair (Word, bool) - where bool = 1 if the word Word is derivable from the grammar, and 0 otherwise.
    * you can see the input format of grammars in the file tests/test_simple1/grammar.txt, and the input format of words in the file tests/test_simple1/test_words.txt

## Code Architecture
* There are parsing-related objects (Rule, Grammar, Early), located in grammar.cpp and early.cpp
* There are interface objects related to reading grammars and words (Reader and its inheritors GrammarReader and WordReader), located in reader.cpp

## Project Structure
* The tests folder contains subfolders with grammar and word sets, along with a script that verifies that the parser's responses match the answers in the file
* The test_gen folder contains notebooks where tests were generated
* The include and src folders contain headers and source files, respectively

