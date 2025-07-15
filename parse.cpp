#include <iostream>
#include <fstream>
#include "core.h"

using namespace std;

void open(string filepath){
    ifstream file(filepath, ios::binary | ios::ate);
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

void parse_array(){
    index = 7;
    cout << "Character: " << jstr[index] << endl;
    cout << "Output: " << get_bool() << endl;
}

int main(){
    open("file.json");
    cout << jstr << endl;
    parse_array();
}