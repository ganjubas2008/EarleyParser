project(parser)

set(parser-source-files
    "${PROJECT_SOURCE_DIR}/grammar.cpp"
    "${PROJECT_SOURCE_DIR}/reader.cpp"
    "${PROJECT_SOURCE_DIR}/earley.cpp"
    CACHE INTERNAL "")

set(parser-include-files
    "${Parser_SOURCE_DIR}/include/grammar.hpp"
    "${Parser_SOURCE_DIR}/include/reader.hpp"
    "${Parser_SOURCE_DIR}/include/earley.hpp"
    CACHE INTERNAL "")

add_library(
    parser SHARED ${parser-source-files} ${parser-include-files}
)

add_executable(run_main "${PROJECT_SOURCE_DIR}/main.cpp")
target_link_libraries(run_main parser)
