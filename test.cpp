//using pointers to pass the original string through the different instances of the example class
#include <iostream>

namespace _windows{
    void print(std::string win_string){
        std::cout << win_string << std::endl; 
        std::cout << "Windows Namespace\n";
        std::cout << "-------------------\n";
    }
}

namespace _linux{
    void print(std::string lin_string){
        std::cout << lin_string << std::endl; 
        std::cout << "Linux Namespace" << std::endl;
        std::cout << "-------------------\n";
    }
}

namespace _mac{
    void print(std::string mac_string){
        std::cout << mac_string << std::endl;
        std::cout << "Mac Namespace\n";
        std::cout << "-------------------\n";
    }
}


int main(){
    _windows::print("Hello Windows!!!");
    _linux::print("Hello Linux!!!");
    _mac::print("Hello Mac!!!");
    return 0;
}