#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

string jstr;

/*
"{"array":["string", 0, 2, 3.14, true], "object":{}}" | 9:36 -> '["string", 0, 2, 3.14, true]' 
*/

//from least to most complex.
enum types{
    _error,
//--_-----
    _boolean,
    _integer,
    _double,
    _string,
    _array,
    _object
};

//Note: This class might not be needed in the end...
//properties are any values that exist inside an object or array
class Property{
    private:
        string data; //data of simple values
        int start = 0, end = 0; //starting index for nested element
        int index;
        int type = 0; //remains unchanged if property wasn't obtained correctly
    
    public:
        //property string and start values are optional so that the user can give just one of them 
        Property(string prop_str, int prop_type){
            data = prop_str;
            type = prop_type;
        }

        Property(int prop_start, int prop_type){
            index = prop_start;
            type = prop_type;
        }

        void print(){
            if (data != ""){
                cout << "Data: " << data << endl;
                cout << "Type: " << type << endl;
            } else{
                cout << "Index: " << index << endl;
                cout << "Type: " << type << endl;
            }
        }


};

//"name": Property, ""

//elements are objects or arrays on the first layer of the file(meaning they wouldn't be nested)
class Element{
    private:
        string* jstr; //pointer to base json string
        vector<Property> data; //contains simple properties
        vector<Element> nested; //any parsed nested elements
        int type;
        //data[0] =  Property -> int start, int end,  int type

    public:
        Element(string* content){
            jstr = content;
        }

        //handle search by key
        void operator[](string str){
            cout << "Hello";
        }
        
        //handle indexing
        void operator[](int str){
            
        }
};

//1. go through the original string, avoid creating new copies
//2. store the data string and type of simple values using the 'Property' class
//3. store the index of the starting charcter of nested elements for future access

//How To Have All Elements Point To The Same String???

//currently still on the transition from a file oriented to string oriented program
//the parse class shall obtain the content string from the previously parsed element
class Parse{
    private:
        string jstr; //json string(refrence based on content parameter)
        char character;
        int length; //json's string length
        int current; //current index

        //number and letter lists for is_number and is_letter functions
        char numbers[11] = "0123456789";
        char _true[5] = "true";
        char _false[6] = "false";

    public:
        //parse would only be called from elements
        //set current index and json string
        Parse(int type, string content){    
            jstr = content; 
            length = jstr.length();
            current = 0;
            character = jstr[current];
            cout << "Character: " << character << endl;
        }
        
        bool is_number(char character){
            for(int index = 0; index < 10; ++index){
                if (character == numbers[index]){
                    return true;
                }
            }

            return false;
        }
        
        /*IMPORTANT: this function shouldn't be called until the array or object is requested
                   --so it might be a good idea to make it output a string again.
                   --that way a 'set' function inside the property class could update the string with the output
        */
        //get content of array or object
        Property get_content(){
            //declare opening and closing characters
            char ochar, cchar;
            int type = 0;

            //declare opening and closing character counters
            int opening = 0, closing = 0; 

            string content;

            if (character == '['){
                ochar = '[';
                cchar = ']';
                type = _array;
            } else if(character == '{'){
                ochar = '{';
                cchar = '}';
                type = _object;
            } else {
                cout << "ERROR: Expected '{' Or '['\n";
                cout << "FOUND: '" << character << "'" << endl;
                Property property("", _error);
                return property;
            }

            while (current < length){
                character = jstr[current];
                content += character;
                if (character == ochar) opening++;
                if(character == cchar) closing++;
                if (opening == closing){
                    Property property(content, type);
                    return property;
                }
                current++;
            }

            cout << "ERROR: Element Was Not Closed" << endl;
            Property property(content, _error);
            return property;
        }

        //get escaped character written in a string in the json file.
        char get_esc(){
            current++;
            //get the next character(letter after escape symbol '\')
            switch(jstr[current]){        
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
                    return '\\';
                default:
                    cout << "Character \\" << jstr[current] << "is not in escaped character list.";
                    return ' '; //return whitespace
            }
        }

        //should be called when current character is a number...
        Property get_number(){
            char character = jstr[current]; 
            bool isDouble = false; //assumes that the number type is integer until floating point is found
            string nstr; //number string

            while (current < length){
                if (character == '.'){
                    //if _double is already true
                    if (isDouble){
                        //if double isn't false but floating point was found -> ERROR
                        cout << "ERROR: Number Cannot Have More Than One Floating Point" << endl;
                        break;
                    }      

                    //else
                    isDouble = true;
                    nstr += character;
                    character = jstr[++current];

                    //check for valid character after floating point
                    if (is_number(character)){
                        nstr += character;
                    } else{
                        cout << "ERROR: Expected Number After Floating Point\n";
                        cout << "FOUND: " << character << endl;
                    }
                } else if (is_number(character)){
                    nstr += character;
                } else{
                    break; //stop when something apart from a dot or a number is found
                }

                character = jstr[++current];
            }

            if (isDouble){
                Property property(nstr, _double);
                cout << "Double: " << nstr << endl;
                return property;
            } 
                
            //assume its an integer by default
            Property property(nstr, _integer);
            cout << "Integer: " << nstr << endl;
            return property;
        }
        
        Property get_bool(bool type){
            string bstr; //boolean string

            if (type){
                for(int index = 0; index < 4; ++index){
                    bstr += character;
                    character = jstr[++current];
                }
            } else{
                for (int index = 0; index < 5; ++index){
                    bstr += character;
                    character = jstr[++current];
                }
            }

            if (bstr == _true || bstr == _false){
                Property property(bstr, _boolean);
                return property;   
            }

            cout << "ERROR: Expected Boolean" << endl;
            cout << "FOUND: " << bstr << endl;

            Property property("", _error);
            return property;
        }
        
        //call function at opening quotation
        Property get_string(){
            string result;
            char character;
            cout << "Index: " << current << endl;

            while (current < jstr.length()){
                //update index and check for closing quotation
                character = jstr[++current]; 
                if (character == '"'){
                    Property property(result, _string);
                    return property; //return string
                }

                //get normal or escaped character and update string
                if (character == '\\'){
                    result += get_esc(); //get escaped character if '\' is found
                } else{
                    result += character; //otherwise, add current character
                } 
            }

            //replace by an actual error later
            cout << "ERROR: Missing Quotation Mark" << endl;
            Property property(result, _error);
            return property;
        }

        //unordered_map<string, string>
        void get_object(string &content){
            
        }

        //return a vector of strings containing the elemtents on the array
        vector<Property> get_array(string &content){
            vector<Property> array;
            Property result("", 0);
            types type;
            

            char character;

            for (int index = 0; index < content.length(); index++){
                character = content[index];     
                
                if (character == '['){
                    
                }
                if (character == ']'){
                    return array;
                }

                if (character == ','){
                    
                }
                
            }    

            return array ;
        }

};