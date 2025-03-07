//i have to go to bed now, but I'll work on this a bit tommorrow.
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "headers/parse.h"

using namespace std;

//check the if key exists in unordered map 
bool check(string key, unordered_map<string, Element> group){
    //return false if key doesn't exist
    if (group.find(key) == group.end()){
        return false;
    }

    //otherwise, return true
    return true;
}

//open file and set the base JSON string
void open(string name){
    //declare extension string and file content
    string extension;
    bool found = false; //updated if '.' is found
    
    ifstream file(name); //open the file indicated by the user
    char character; //loop character

    for(auto &ex_char : name){
        if(ex_char == '.'){
            found = true;
        }
        //check if dot(start of extension) was found and ignore white space
        if (found){
            extension += ex_char; //add extension character
            if (ex_char == ' '){
                cout << "ERROR: File Name Must Not Contain Spaces." << endl; //extension is wrong, empty file pointer will be returned
                cout << '"' << name << '"' << endl;
                cout << "-----\n";
                break;
            }
        }
    }
    
    //return empty string if extension was wrong (to be replaced with error handling once I figure it out)
    if (extension != ".json"){
        cout << "File Must Have '.json' Extension\n";
    }

    //get the file's content if extension is correct
    while (file.get(character)){
        jstr += character;
    }
    
    cout << "Content Address: " << &jstr<< endl; 
}

class json{
    //although this way of organizing things makes sense... it is a good idea to think of a way to clear accumulated memory after reaching certain treshold
    private:
        vector<Property> array; //contains the data for arrays
        unordered_map<string, int> properties; //contain parsed objects and their indexes in their indexes in data vector
        vector<vector<Element>> data; //contain object and element data
        
    public:
        json(string name){
            cout << "Name: " << name << endl;
            open(name);
        }



};

int main(){
    json file("file.json");    
	return 0;
}
