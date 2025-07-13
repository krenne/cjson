#include <iostream>
#include <cctype>
#include <string>

using namespace std;

int index = 0;
string jstr = "";

string get_bool(string test = ""){
    string boolean;

    if (!test.empty()){
        index = 0;
        jstr = test;
    }

    if(jstr[index] == 't'){
        boolean = "true";
    } else if (jstr[index] == 'f'){
        boolean = "false";
    } else{
        cout << "ERROR: function not called on 'true' or 'false'." << endl;
        return "";
    }

    int current = 0;
    while (boolean[current] != '\0'){
        if (boolean[current] != jstr[index]){
            cout << "ERROR: Characters Don't Match" << endl;
            return "";
        }
        current++;
        index++;
    }

    
    return boolean;
}

string get_number(string test = ""){
    string number;

    if (!test.empty()){
        index = 0;
        jstr = test;
    }

    while (jstr[index] != '\0'){
        if (isdigit(jstr[index])){
            number += jstr[index];
        } else if (jstr[index] == '.'){
            number += jstr[index];
            if (!isdigit(jstr[++index])){
                cout << "Expected Number After '.'\n";
                cout << "Found: " << jstr[index] << endl;
                return "";
            }
            continue;
        } else if (jstr[index] == ' ' || jstr[index] == ','){
            return number;
        } else{
            cout << "Expected Number: \n";
            cout << "Found: " << jstr[index] << endl;
            return "";
        }
        ++index;
    }

    //shouldn't be at end of json string if it isn't a test
    return (!test.empty()) ? number : "";
}

string get_string(string test = ""){
    string result;

    if (!test.empty()){
        index = 0;
        jstr = test;
    }

    if(jstr[index] == '"'){
        while(jstr[++index] != '"'){
            if (jstr[index] == '\0') cout << "what";
            result += jstr[index];
        }
    }

    return result;
}

struct Range{
    int start;
    int end;
};

//get range of the body of an array
Range get_array(string test = ""){
    int opening = 0;
    int closing = 0;

    if (!test.empty()){
        index = 0;
        jstr = test;
    }
    
    int start = index;
    while(jstr[index] != '\0'){
        if (jstr[index] == '[') opening++;
        else if (jstr[index] == ']') closing++;
        if (opening == closing) return {start, index};
        ++index;
    }

    return {0, 0};
}

//get range of the body of an object
Range get_object(string test = ""){
    int opening = 0;
    int closing = 0;

    if (!test.empty()){
        index = 0;
        jstr = test;
    }
    
    int start = index;
    while(jstr[index] != '\0'){
        if (jstr[index] == '{') opening++;
        else if (jstr[index] == '}') closing++;
        if (opening == closing) return {start, index};
        ++index;
    }

    return {0, 0};
}

int main(){
    cout << "Get Boolean: " << get_bool("true") << endl;
    cout << "Get Number: " << get_number("580") << endl;
    cout << "Get String: " << get_string(R"("hello")") << endl;
    Range array_range = get_array(R"([0, 1, 2, "Hello World!!!", 1.23, [3, 2, 1, 0]])");
    cout << "Get Array: {" << array_range.start << ", " << array_range.end << "}\n"; 
    Range object_range = get_object(R"({"hello", {"world", true}})");
    cout << "Get Object: {" << object_range.start << ", " << object_range.end << "}\n";
    return 0;
}
