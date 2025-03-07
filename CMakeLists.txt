cmake_minimum_required(VERSION 3.10)
project(ConsoleUtility C)

set(CMAKE_C_STANDARD 99)

add_executable(myutil Main.c polzovatel.c procesi.c logs.c)

