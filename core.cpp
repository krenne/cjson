#include <iostream>
#include <cctype>
#include <string>

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
        
        char current(){
            return content[index];
        }

        char next(){
            ++index;
            if (current() == '\n'){
                ++line;
            }
            return current();
        }

        char escape(){
            switch(next()){
                case '\\':
                    return current();
                case 'n':
                    return '\n';
                case 'b':
                    return '\b';
                case 'f':
                    return '\f';
                case 'r':
                    return '\r';
                case 't':
                    return '\t';
                default:
                    std::cout << "ERROR: " << "'" << current() << "'" << " Is Not A Valid Escaped Character";
                    return '\0';
            }
        }

        void go_to(int dest){
            index = dest;
        }

        //set the variables for a test
        void set_test(std::string test_case){
            content = test_case;
            index = 0;
            line = 1;
        }

        //print the characters of an object/array
        void print_range(Range range){
            go_to(range.start);
            while (index <= range.end){
                std::cout << current(); 
                index++;
            }
            std::cout << std::endl;
        }
};

std::string get_bool(JSTR &jstr, std::string test = ""){
    std::string boolean;
    if (!test.empty()){
        jstr.set_test(test);
    }

    if (jstr.current() == 't') {
        boolean = "true";
    } else if (jstr.current() == 'f') {
        boolean = "false";
    } else {
        std::cout << "ERROR: function not called on 'true' or 'false'." << std::endl;
        return "";
    }

    for (char character : boolean) {
        if (jstr.current() != character) {
            std::cout << "ERROR: Characters Don't Match" << std::endl;
            return "";
        }
        jstr.next();
    }

    return boolean;
}

std::string get_number(JSTR &jstr, std::string test = ""){
    std::string number;
    if (!test.empty()){
        jstr.set_test(test);
    }

    while (jstr.current() != '\0') {
        if (jstr.current() == ' ' || jstr.current() == ',') return number;
        if (isdigit(jstr.current()) || jstr.current() == '-' || jstr.current() == '.'){
            number += jstr.current();
            jstr.next();
            continue;
        } else{
            std::cout << "ERROR: Expected Digit('0-9'), Negative Sign('-') or Floating Point('.')\n";
            std::cout << "Found: " << jstr.current() << std::endl;
            return "";
        }
        number += jstr.current();
        jstr.next();
    }

    //return number if at end of test string | otherwise '\0' shouldn't be reached 
    return (!test.empty()) ? number : "";
}

std::string get_string(JSTR &jstr, std::string test = ""){
    std::string result;

    if (!test.empty()){
        jstr.set_test(test);
    }

    while (jstr.current() != '\0'){
        if (jstr.next() == '"') return result;
        if (jstr.current() == '\\'){
            result += jstr.escape();
            continue;
        }
        result += jstr.current();
    }

    std::cout << "ERROR: Closing Double Quote('\"') Was Not Found";
    return "";
}

Range get_array(JSTR &jstr, std::string test = ""){
    int opening = 0;
    int closing = 0;

    if (!test.empty()){
        jstr.set_test(test);
    }

    int start = jstr.index;
    int end;

    while (jstr.current() != '\0'){
        if (jstr.current() == '[') opening++;
        else if (jstr.current() == ']') closing++;
        if (opening == closing) return {start, jstr.index};
        jstr.next();
    }

    std::cout << "ERROR: Could Not Find Closing Curly Brace -> ']'" << std::endl;
    return {0, 0};
}

Range get_object(JSTR &jstr, std::string test = ""){
    int opening = 0;
    int closing = 0;

    if (!test.empty()){
        jstr.set_test(test);
    }

    int start = jstr.index;
    int end;

    while (jstr.current() != '\0'){
        if (jstr.current() == '{') opening++;
        else if (jstr.current() == '}') closing++;
        if (opening == closing) return {start, jstr.index};
        jstr.next();
    }

    std::cout << "ERROR: Could Not Find Closing Curly Brace -> '}'" << std::endl;
    return {0, 0}; 
}