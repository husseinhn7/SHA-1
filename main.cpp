#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "sha.h"
#include <iomanip>



int main(int argc, char* argv[]){
    
    std::string g = get_bin_from_file(argv[1]);
    std::string * ptr = &g;
    preprocessing(ptr);
    std::vector<std::vector<std::bitset<32>>> list_of_chunks;
    std::vector<std::vector<std::bitset<32>>> * list_of_chunks_ptr = &list_of_chunks;
    loop(list_of_chunks_ptr , ptr);
    
    return 0;
}