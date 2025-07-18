#include <iostream>
#include <fstream>
#include "core.hpp"

int JSTR::length(){
    return content.length();
}

void JSTR::print_range(Range range) {
    JSTR::index = range.start;
    while (index <= range.end) {
        std::cout << current();
        index++;
    }
    std::cout << std::endl;
};

void JSTR::open(std::string filepath){
    std::ifstream file(filepath, std::ios::in | std::ios::ate);
    if (!file.is_open()){
        std::cout << "ERROR: Failed To Open File" << std::endl;
        exit(1);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    JSTR::content.resize(size);
    
    file.read(&content[0], size);
    file.close();
}   

std::string get_value(JSTR& jstr, std::string test = ""){
    if (!test.empty()){
        jstr.set_test(test);
    }

    std::string value;

    if (isdigit(jstr.current()) || jstr.current() == '-'){
        value = get_number(jstr);
        std::cout << value << std::endl;
        return value;
    }

    switch(jstr.current()){
        case 't':
        case 'f':
            value = get_bool(jstr);
            std::cout << value << std::endl;
            return value;
        case '"':
            value = get_string(jstr);
            std::cout << value << std::endl;
            return value;
        case 'n':
            value = get_null(jstr);
            std::cout << value << std::endl;
            return value;
        case '[':
            {
                Range range = get_array(jstr);
                jstr.print_range(range);
                return "range";
            }
            break;
        case '{':
            {
                Range range = get_object(jstr);
                jstr.print_range(range);
                return "range";
            }
            break;
        default:
            std::cout << "ERROR: '" << jstr.current() << "' Is Not Part Of A Valid JSON Value" << std::endl;
            std::cout << "Line: " << jstr.line << std::endl;
    }

    return "";
}

void parse_array(JSTR& jstr, Range range = {1, 0}){
    //if range.start is 1('true') and range.end is 0('false'), then we are parsing the base array
    if (range.start && !range.end){
        jstr.index = 0; //should be 0 in this case, but better be safe
        range = get_array(jstr);  
    }

    jstr.index  = range.start;
    jstr.line = 1;
    jstr.next(); //move to the first character after the opening bracket '['

    std::string value;
    bool expect_value = true;
    while (jstr.index < range.end){
        if (!expect_value && jstr.current() == ','){
            expect_value = true;
            jstr.next();
        }

        if (jstr.current() == ' ' || jstr.current() == '\n' || jstr.current() == '\t'){
            jstr.next();
            continue;
        }
 
        if (expect_value){
            get_value(jstr);
            if (jstr.current() == ',') {jstr.next(); continue;};
            expect_value = false;
            jstr.next();
        } else{
            std::cout << "ERROR: Expected Comma ','\nFound: '" << jstr.current() << "' (Line: " << jstr.line << ")" << std::endl;
            return;
        }
    }
}

int main(){
    JSTR jstr;
    jstr.open("file.json");
    parse_array(jstr);
}   