#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

//get escaped character written in a string in the json file.
char get_esc(ifstream &file){
    char escaped;
    int length = 3;
    unordered_map<string, int> elements[length];
    file.get(escaped);
    switch(escaped){        
        case '"':
            return '\"';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 't':
            return '\t';
        case '\\':
            return escaped;
        default:
            cout << "Character \\" << escaped << "is not in escaped character list.";
            return ' '; //return whitespace
    }
}

//file pointer should be at a starting quotation when passing this function.
string get_string(ifstream &file){
    char character;
    string result;

    //condition switches character first. so starting quotation '"' is ignored
    while(file.get(character) && (character != '"')){
        if (character == '\\'){
            character = get_esc(file);
            cout << "Get Escaped Function was Called\n";
        }
        result += character;
    }

    return result;
}

//Note: This class might not be needed in the end...
//properties are any values that exist inside an object or array
class Property{
    private:
        string data;
        int position;
        int type;
};

//elements are objects or arrays on the first layer of the file(meaning they wouldn't be nested)
class Element{
    private:
        vector<Property> data;
        int type;

    public:
        //handle search by key
        void operator[](string str){
            cout << "Hello";
        }
        
        //handle indexing
        void operator[](int str){
            
        }
};

//handle the parsing of array or object strings. return a vector containing properties
void parse(int type, string jstr){
    cout << jstr;
}


int main(){
    string test = "\"hello\", 3, 5, \"name\"";
    return 0;
}