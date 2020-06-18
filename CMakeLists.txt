cmake_minimum_required(VERSION 3.8)
project(parser)

set(CMAKE_C_STANDARD 11)

add_executable(
        parse_print_tree
        src/scan.c
        src/parse.c
        src/print.c
        src/print_test.c
)

add_executable(
        parse_eval
        src/scan.c
        src/parse.c
        src/eval.c
        src/eval_test.c
)

target_link_libraries(parse_eval m)
target_link_libraries(parse_print_tree m)
