#ifndef core_hpp
#define core_hpp

#include <iostream>

struct Range{
    int start;
    int end;
};

class JSTR{
    private:
        std::string content;   
    public:
        int index;
        int line;

        JSTR(std::string test = ""){
            index = 0;
            line = 1;
            if (!test.empty()){
                content = test;
            }
        }        
        
        int length();

        char current();

        char next();

        char escape();

        void set_test(std::string test_case);

        void print_range(Range range);
        
        void open(std::string filepath);
};

std::string get_null(JSTR &jstr, std::string test = "");

std::string get_bool(JSTR &jstr, std::string test = "");

std::string get_number(JSTR &jstr, std::string test = "");

std::string get_string(JSTR &jstr, std::string test = "");

Range get_array(JSTR &jstr, std::string test = "");

Range get_object(JSTR &jstr, std::string test = "");

#endif