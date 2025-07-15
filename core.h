#pragma core_h
#include <iostream>

#ifndef json_variables

#define json_variables
extern std::string jstr;
extern int index;

#endif

std::string get_bool(std::string test = "");

std::string get_number(std::string test = "");

std::string get_string(std::string test = "");

struct Range{
    int start;
    int end;
};


Range get_array(std::string test = "");

Range get_object(std::string test = "");