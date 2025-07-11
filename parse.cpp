#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

using namespace std;

//global index
int index = 0;

//json string
string jstr;


enum types{
    _boolean,
    _number,
    _string,
    _list,
    _object
};


string get_bool(){
    string boolean;

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

string get_string(){
    string result;
    if(jstr[index] == '"'){
        while(jstr[++index] != '"'){
            if (jstr[index] == '\0') return "";
            result += jstr[index];
        }
    }

    return result;
}

struct test_result{
    string _case;
    string output;
    string expected;
    bool status;
};

bool compare(string expected, string output){
    if (expected == output){
        return true;
    }

    cout << "Expected: " << expected << endl;
    cout << "Received: " << output << endl;

    return false;
}

bool test(string subject, int type, string expected = ""){
    index = 0;
    jstr = subject;

    bool result = false;
    cout << "--\n";

    string output;
    string type_string;

    switch(type){
        case _boolean:
            output = get_bool();
            type_string = "Boolean";
            if (!expected.empty()) break;
    
            if (subject == "true" || subject == "false"){
                expected = subject;
            } else{
                expected = "";
            }
            break;
        case _string:
            output = get_string();
            if (!expected.empty()) break;
            expected = subject.substr(1, subject.length()-2);
            type_string = "String";
            break;
    }

    string status;
    if (compare(expected, output)){
        status = "Passed";
        result = true;
    } else{
        status = "Failed";
        result = false;
    }

    cout << "Test Case: " << subject << endl;
    cout << "Expected: " << expected << endl;
    cout << "Output: " << output << endl;
    cout << "Type: " << type_string << endl;
    cout << "Status: " << status << endl;
    cout << "--\n";

    return result;
}


int main(){
    jstr = "true";

    index = 0;

    test(R"("my string")", _string);

    return 0;
}