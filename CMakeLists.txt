cmake_minimum_required(VERSION 3.22)

project(MooLang VERSION 0.1
  DESCRIPTION "My custom language"
  LANGUAGES C)

add_library(scanner STATIC
  src/scanner/scanner.c
  src/scanner/source.c
  src/scanner/substring.c
  src/scanner/token_array.c)
target_include_directories(scanner PUBLIC ./)

add_library(parser STATIC
  src/parser/parser.c
  src/parser/expression.c
  src/parser/error.c
  src/parser/statement.c
  src/parser/statement_array.c)
target_include_directories(parser PUBLIC ./)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(moolang src/main.c)
target_link_libraries(moolang PUBLIC scanner parser)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")

