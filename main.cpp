#include <string>
#include <iostream>

#include "BinarySearchTree.h"

int main() {
    
    BinarySearchTree<int, std::string> test;
    test.insert(1, "Cat");
    test.insert(2, "Dog");
    test.insert(76, "blade");  
    test.insert(66, "sleeve"); 
    test.insert(83, "fight"); 
    test.insert(27, "powder"); 
    test.insert(93, "carbon"); 
    test.insert(66, "fossil"); 
    test.insert(90, "sport"); 
    test.insert(9, "stride"); 
    test.erase(90);
    for(auto&& [key, value] : test) {
        std::cout << key << ": " << value << std::endl;
    }
    return 0;
}