My first C++ project. 
A small json parser to use on other programs.

Disclaimer: 
I am not trying to make a perfect parser. 
Making a json parser should be relatively simple and give me an idea of how parsers work. 

Ultimately, I just want this project to be functional.

json file("filename.json") -> openfile(name); start() -> {"object1":ID, "array":ID, "object2":ID};

file["object1"] -> ID; ID ->  file_position, int type, bool parsed, 

elements_list(vector<Element>) -> [element0, element1, element3] -> element0.properties(vector<Property>), element.type = _array;

object_list(unordered_map<string name, Property>)

Property -> string value, int type, vector<Property>* pointer; //if pointer is null then the element hasn't been parsed or cannot be parsed(because of type or error)

file["object1"] -> ID; ID contains: element file position, int type, bool parsed,