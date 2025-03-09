#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

string jstr; //base json string
int length; //json string length

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
        vector<Property>* array;
        unordered_map<string, Property>* object;
        int start = 0, end = 0; //starting index for nested element
        int type = 0; //remains unchanged if property wasn't obtained correctly
    
    public:
        //simple property
        Property(string prop_str, int prop_type){
            data = prop_str;
            type = prop_type;
        }

        //complex property(nested element)
        Property(int prop_start, int prop_end, int prop_type){
            start = prop_start;
            end = prop_end;
            type = prop_type;
        }

        void print(){
            if (data != ""){
                cout << "Data: " << data << endl;
                cout << "Type: " << type << endl;
            } else{
                cout << "Start: " << start << endl;
                cout << "End: " << end << endl;
                cout << "Type: " << type << endl;
            }
        }
};

//"name": Property, ""

//elements are objects or arrays on the first layer of the file(meaning they wouldn't be nested)
class Element{
    private:
        vector<Property> data; //contains simple properties
        vector<Element> nested; //any parsed nested elements
        int type;
        //data[0] =  Property -> int start, int end,  int type

    public:
        Element(string content){
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

//currently still on the transition from a file oriented to string oriented program
//the parse class shall obtain the content string from the previously parsed element
class Parse{
    private:
        //current index and character
        char character;
        int current; 

        //number and letter lists for is_number() and is_bool()
        char numbers[11] = "0123456789";
        char _true[5] = "true";
        char _false[6] = "false";

    public:
        //parse would only be called from elements
        //set current index and json string
        Parse(int start, int end, int type){     
            current = start;
            if (!length){
                length = jstr.length(); //obtain the length if not already set 
            }
            character = jstr[start];
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
        Property get_range(){
            //declare opening and closing characters
            char ochar, cchar;
            int start = current, end = 0;
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
                Property property(start, end, _error);
                return property;
            }

            while (current < length){
                cout << "Character: " << character << endl;
                if (character == ochar) opening++;
                if(character == cchar) closing++;
                if (opening == closing){
                    end = current;
                    Property property(start, end, type);
                    return property;
                }
                character = jstr[++current];
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
        
        Property get_bool(bool _bool){
            string bstr; //boolean string

            if (_bool){
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

            while (current < length){
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
        
        //return a vecto   r of strings containing the elemtents on the array
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

//element[]->(property)[](property)[](property).type == _int; -> return stoi(property.value);
int main(){
    string _json = R"(["array", 0, 1, "value", ["nested"], {"object":["value"]}])";
    string _bool = R"(true,)";
    string _str = R"("array and")";
    jstr = _json;
    Parse variable(0, jstr.length()-1, _array);
    variable.get_range().print();
    //cout << variable.get_content(_array) << endl;
    return 0;
}