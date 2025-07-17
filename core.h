#pragma core_h
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
        }        
        
        char current();

        char next();

        char escape();

        void go_to(int dest);

        void set_test(std::string test_case);

        void print_range(Range range);
};

std::string get_bool(JSTR &jstr, std::string test = "");

std::string get_number(JSTR &jstr, std::string test = "");


std::string get_string(JSTR &jstr, std::string test = "");

Range get_array(JSTR &jstr, std::string test = "");

Range get_object(JSTR &jstr, std::string test = "");