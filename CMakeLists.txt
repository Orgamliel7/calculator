cmake_minimum_required(VERSION 3.20)
project(My_Calculator_Scanner_and_Parser__90_main)

set(CMAKE_CXX_STANDARD 14)

include_directories("Calculator with scanner and parser")

add_executable(My_Calculator_Scanner_and_Parser__90_main
        "Calculator with scanner and parser/calc.cpp"
        "Calculator with scanner and parser/parser.cpp"
        "Calculator with scanner and parser/scanner.cpp"
        "Calculator with scanner and parser/scanner.h"
        "Calculator with scanner and parser/symtab.cpp"
        "Calculator with scanner and parser/symtab.h")
