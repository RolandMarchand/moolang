cmake_minimum_required(VERSION 3.22)

project(MooLang VERSION 0.1
  DESCRIPTION "My custom language"
  LANGUAGES C)

add_library(scanner STATIC
  include/moolib/scanner/scanner.h
  src/scanner/scanner.c
  src/scanner/token_array.c
  src/scanner/source.c
  src/scanner/substring.c)
target_include_directories(scanner PUBLIC include)

add_executable(moolang src/main.c)
target_link_libraries(moolang PUBLIC scanner)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -g)
