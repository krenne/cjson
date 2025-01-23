//i have to go to bed now, but I'll work on this a bit tommorrow.
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

using namespace std;

//from most to least complex.
enum types{
    _array,
    _object,
    _string,
    _double,
    _integer,
    _boolean
};

//element identification data
class ID{
    public:
        bool parsed = false; //determine if it has to been parsed or not
        int index = 0; //index in element data array(if already parsed)
        int position = 0; //file pointer position. used if element is not parsed.
        int type = 0; //type from enum types
};

//Note: This class might not be needed in the end...
//properties are any values that exist inside an object or array
class Property{
    private:
        string data;
        int type;
        vector<Property>* pointer; //if element is array or object save memory address of parsed data 
};

//elements are objects or arrays on the first layer of the file(meaning they wouldn't be nested)
class Element{
    private:
        vector<Property> properties;

    public:
        int type = -1;

        //handle search by key
        void operator[](string str){

        }

        //handle indexing
        void operator[](int str){
            
        }
};


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

//open file in order to set the 
ifstream open(string name){
    string extension;
    bool found = false; //updated if '.' is found
    for(auto &character : name){
        if(character == '.'){
            found = true;
        }
        //check if dot(start of extension) was found and ignore white space
        if (found){
            extension += character; //add extension character
            if (character == ' '){
                cout << "ERROR: File Name Must Not Contain Spaces." << endl; //extension is wrong, empty file pointer will be returned
                cout << '"' << name << '"' << endl;
                cout << "-----\n";
                break;
            }
        }
    }
        
    //return empty file pointer if extension is wrong
    if (extension != ".json"){
        ifstream file; //return empty file pointer/variable
        return file;
    }

    ifstream file(name);
    if(file.is_open()){
        cout << "File opened." << endl;
    }
    return file;
}



void move(int position, ifstream& file){

}

//skip the content of an element
void skip(int type, char &character, ifstream &file){
    //opening and closing counters(program starts on an opening character, so opening = 1)
    int opening = 1, closing = 0; 

    //character intialization
    char ochar; //opening character
    char cchar; //closing character

    if (type == _array){
        ochar = '[';
        cchar = ']';
    } else if(type == _object){
        ochar = '{';
        cchar = '}';
    }

    while (file.get(character)){
        if (character == ' '){
            continue;
        }
        if (character == ochar){
            opening += 1;
        } else if (character == cchar){
            closing += 1;
        }

        //end loop if element was closed
        if (opening == closing) break; 
    }
}

//get content of object or array || file pointer must be updated to starting character of element before being passed to function
string get(int type, int position, ifstream &file){
    //save current position and move to target
    int current = file.tellg();
    file.seekg(position, ios::beg);
 
    //opening and closing counters(function starts on the element's opening character, so opening = 1)
    int opening = 1, closing = 0; 

    //character and string intialization
    char character; //loop character
    char ochar; //opening character
    char cchar; //closing character


    string result;
    
    //set characters to ignore
    if (type == _array){
        ochar = '[';
        cchar = ']';
    } else if(type == _object){
        ochar = '{';
        cchar = '}';
    }

    //get content of element
    while (file.get(character)){
        if (character == ' '){
            continue;
        }

        if (character == ochar){
            opening += 1;
        } else if (character == cchar){
            closing += 1;
        }

        //end loop if element was closed
        if (opening == closing) break;

        if (character == '"'){
            result += character + get_string(file) + '"';
            continue;
        }

        result += character; //otherwise update string

        
    }

    //reset to current position and return result
    file.seekg(current, ios::beg);
    return result;
}


//for start function(below get_group)
enum targets{
    _name,
    _colon,
    _element,
    _comma
};

enum errors{
    ex_colon, //expected colon
    ex_bracket, //expected bracket
    ex_curly, //expected curly brace
    mis_quote, //missing quotation mark
    un_char //unexpected character
};

//element error
class error{
    private:
        string description;
        int last_position = 0;
        int line;

    public:
        //constructor initialize the error class data
        error(int error, ifstream &file){
            //initialize line
            line = 1;
            cout << "-!-!-\n";
            cout << "Error was called.\n";
            get_line(file);
            cout << "Line " << line << ": Error\n";
            cout << "-!-!-\n";
        }

        //count new line characters from last position
        int get_line(ifstream &file){   
            int position = file.tellg(); //save current position
            file.seekg(last_position); //move to last position
            char character; 
            while (file.get(character) && file.tellg() != position){
                if (character == '\n'){
                    line++;
                }
            }
            //save position
            last_position = position;

            return 0;
        }
};

//create a group of 12 elements stored in a hashmap
unordered_map<string, ID> get_group(ifstream &file){
    //main variables for group map
    unordered_map<string, ID> group; 
    ID id;
    int amount = 0;
    const int limit = 12; 

    char character; //character
    string name; //element name
    enum targets target = _name; //current search target

    //ID variables
    int type = 0;
    int position = 0; //file pointer position
    
    string input;

    while(file.get(character) && amount != limit){    
        //ignore white spaces
        if (character == ' ' || character == '\n'){
            continue;
        }

        switch (target){
            case _name:
                if (character == '"'){
                    name = get_string(file);
                    target = _colon;
                }
                break;
            
            case _colon:
                if (character == ':'){
                    target = _element;
                } else{
                    cout << "ERROR: Character Separating Key From Array or Object Should Be A Colon(\":\")\n";
                    cout << "Character Found: '" << character << "'\n";
                    cout << "---------------\n";
                }
                break;

            case _element:           
                if (character == '['){
                    //save data
                    id.position = file.tellg();
                    id.type = _array;
                    cout << "Character: " << character << endl;

                    //skip content and reset target 
                    skip(id.type, character, file);
                } else if (character == '{'){
                    //save data
                    id.position = file.tellg();
                    id.type = _object;
                    cout << "Character: " << character << endl;

                    //skip content and reset target
                    skip(id.type, character, file);
                } else{
                    cout << "ERROR: Expected Opening Square Bracket(\"[\") or Opening Curly Brace(\"{\") After Colon(\":\")\n";
                    cout << "Character Found: '" << character << "'\n";
                    cout << "---------------\n";
                }
                cout << "Name: " << name << endl;
                cout  << "Position: " << id.position << endl;
                cout << "Type: " << id.type << endl;
                cout << "---------\n";

                group[name] = id; //create element 
                target = _comma; //prepare to find comma
                amount++; //update element counter
                continue;
            case _comma:
                if (character == ','){
                    target = _name;
                } else if (character == '}'){
                    cout << "Json Ended\n";
                    amount = limit; //set amount to limit to end the loop
                } else{
                    cout << "Error: Missing Comma\nCharacter: " << character << endl;
                    amount = limit;
                }
                break;
            default:
                cout << "Something Went Wrong...\n";
                break;
        }
    }

    return group; //return the unordered_map
}

//get the main elements of the file in unordered maps of no more than 12 elements each
vector<unordered_map<string, ID>> start(ifstream &file){
    char character;
    vector<unordered_map<string, ID>> groups;

    //get groups
    while (!file.eof()){
        cout << "Group Was Obtained\n";
        groups.push_back(get_group(file));
    }

    return groups;
}

//NOTES: the json class should be initialized with an open("filename.json") function. nothing in it should change after that
//the user will save the output in a variable. but the original json class instance won't change.
//the json class shall instead interact with another class that also has an overloaded subscript operator function.
//this class shall narrow down its output to string, integer or double.
//this class will be made friend so that it can access the json class's private data
//this data will include the general strucuture of the file.

//REMINDER: allthough the above plan might be going on the right direction. it may still have flaws to be addressed.


//check the if key exists in unordered map 
bool check(string key, unordered_map<string, ID> group){
    //return false if key doesn't exist
    if (group.find(key) == group.end()){
        return false;
    }

    //otherwise, return true
    return true;
}

class json{
    //although this way of organizing things makes sense... it is a good idea to think of a way to clear accumulated memory after reaching certain treshold
    private:
        ifstream file;
        vector<unordered_map<string, ID>> elements; //contain identifiers and data for arrays or objects
        vector<unordered_map<string, int>> objects; //contain parsed objects and their indexes in their indexes in data vector
        vector<vector<Element>> data; //contain object and element data
        
    public:
        json(string name){
            cout << "Name: " << name << endl;
            file = open(name);
            elements = start(file);
        }
        
        //handle searches by key
        Element operator[](string key){
            int counter = 0;
            Element var;
            string content;
            if (file.eof()){
                file.clear();
            }
            for (auto group:elements){
                counter++;
                if (check(key, group)){
                    cout << "$$$---$$$---$$$\n";
                    cout << "Element Group: "  << counter << "\n";
                    cout << "Element Position: " << group[key].position << endl;
                    cout << "Element Type: " << group[key].type << endl;

                    //get the content of the element
                    content = get(group[key].type, group[key].position, file);
                    cout << "Element Content: " << content << endl;
                    var.type = group[key].type;
                    break;
                } 
            }
            return var;
        }

        //to check that the file is opened properly
        void printFile(){
            if(!file.is_open()){
                cout << "Error while opening file." << endl;
            }  else{
                char character;
                while(file.get(character)){
                    cout << character << endl;
                }
            }
        }
};

int main(){
    json file("file.json");    
    string input;
    cout << "Enter Element: ";
    cin >> input;
    Element result = file[input];
	return 0;
}
