#include <iostream>
#include <vector>

std::string get_bin_from_file(std::string);

void preprocessing(std::string * );

std::string add_zeros(int);

std::vector<std::bitset<32>> separate(std::string );

void add_80_words(std::vector<std::bitset<32>> *);

std::bitset<32> lrotate(std::bitset<32> , int);

void function_1();

void function_2();

void function_3();

void function_4();

void main_loop(std::vector<std::bitset<32>> *);

std::string hex(long );

void loop(std::vector<std::vector<std::bitset<32>>> * , std::string *);