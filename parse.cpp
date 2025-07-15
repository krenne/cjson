#include <iostream>
#include <fstream>
#include "core.h"

using namespace std;

void open(string filepath){
    ifstream file(filepath, ios::in | ios::ate);
    if (!file.is_open()){
        cout << "ERROR: Failed To Open File" << endl;
        exit(1);
    }

    std::streamsize file_size = file.tellg();
    file.seekg(0, ios::beg);
    jstr.resize(file_size);
    file.read(&jstr[0], file_size);

    return;
}

//set start higher than end if range is not given | function treats array as base json element
void parse_array(Range range = {1, 0}){
    int index = 7;
    cout << "Character: " << jstr[index] << endl;
    cout << "Output: " << get_bool() << endl;
}

int main(){
    open("file.json");
    cout << jstr << endl;
    parse_array();
}