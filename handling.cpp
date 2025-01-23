#include <iostream>
#include <fstream>
#include <vector>

//#include <string> 

using namespace std;

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
        vector<int> lines; //contain the position of the new line characters

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
        
        //consider also making it return a string containing the content on the line(
        //count new line characters from last position
        string get_line(ifstream &file){   
            //intitialize variables and count lines
            int position = file.tellg();
            file.seekg(last_position);

            string content; 
            char character;            
            while (file.get(character) && file.tellg() != position){
                if (character == '\n'){
                    line++;
                    content = "";
                }
                content += character; // update the string(it will save the content of the line, stopping at the file position)
            }

            //save position
            last_position = position;

            return content;
        }

};