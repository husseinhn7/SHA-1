#include <iostream>
#include <bitset>
#include <fstream>
#include "sha.h"
#include <vector>
#include <iomanip>


template<typename T>
void print(T var){
    std::cout << var << '\n';
}

std::bitset<32> A = 0b01100111010001010010001100000001;
std::bitset<32> B = 0b11101111110011011010101110001001;
std::bitset<32> C = 0b10011000101110101101110011111110;
std::bitset<32> D = 0b00010000001100100101010001110110;
std::bitset<32> E = 0b11000011110100101110000111110000;



std::string add_zeros(int number_of_zeros){
    std::string zeros = "";
    for  (int i = 0  ; i < number_of_zeros ; i++)
    {
        zeros = zeros + "0";
        
    }
           return zeros;
 }

std::string get_bin_from_file(std::string path){
    std::string binary_format = "" ; 
    std::ifstream inputFile(path , std::ios::binary);
     if (!inputFile) {
        std::cerr << "Failed to open the input file." << std::endl;
        return "1";
    }
    
     char byte;
     while (inputFile.get(byte)) {
        std::bitset<8> bits(byte);
        binary_format = binary_format + bits.to_string();
    }

    inputFile.close();
    return binary_format;

};


void preprocessing(std::string *msg){
    std::string formatted_msg = ""   ; 
    int msg_length = msg->length();
    // print(msg_length);
    std::bitset<32> msg_bin_length = (std::bitset<32>)msg_length ;
    std::string len = msg_bin_length.to_string();
    if (msg_length < 448){
        *msg =  *msg + "1" + add_zeros(447 - msg_length + 64 - len.length() ) + len ;
    }
    else {
        *msg =  *msg + "1" + add_zeros(447 - msg_length%512 + 64 - len.length() ) + len ;
    }
    // print(msg->length());
    // print(len);
};

void add_80_words(std::vector<std::bitset<32>>  *words){
    
    for (int i = 16 ; i < 80 ; ++i){
        std::bitset<32> new_element = (*words)[i-3] xor (*words)[i-8] xor (*words)[i-14] xor (*words)[i-16] ;
        std::string str = new_element.to_string();
        str = str.substr(1,31) + str[0];
        words->push_back((std::bitset<32>)str);
    }
};


std::vector<std::bitset<32>>  separate(std::string chunk){
    std::vector<std::bitset<32>> list_of_words ;
    for(int i = 0 ; i < 512 ; i+=32){
        std::bitset<32> binary(chunk.substr(i,32));
        list_of_words.push_back(binary );
    }
    add_80_words(&list_of_words);
    return list_of_words;
};




std::bitset<32> lrotate(std::bitset<32> num, int i){
    std::string str_num = num.to_string();
    std::string rotated_str_num =  str_num.substr(i, 32 - i)+ str_num.substr(0,i);
    std::bitset<32> bin_num(rotated_str_num);
    return bin_num;
};

void function_1(std::bitset<32> word){
    std::bitset<32> f ;
    std::bitset<32> k = 0b01011010100000100111100110011001;
    f = (B & C) | (~B & D);
    std::bitset<32> temp = f.to_ulong() + k.to_ulong() + E.to_ulong() + lrotate(A , 5).to_ulong() + word.to_ulong();
    E = D;
    D = C;
    C = lrotate(B , 30);
    B = A;
    A = temp;
};

void function_2(std::bitset<32> word){
    std::bitset<32> f ;
    std::bitset<32> k = 0b01101110110110011110101110100001;
    f = (B ^ C) ^ D;
    std::bitset<32> temp = f.to_ulong() + k.to_ulong() + E.to_ulong() + lrotate(A , 5).to_ulong() + word.to_ulong();
    E = D;
    D = C;
    C = lrotate(B , 30);
    B = A;
    A = temp;
};

void function_3(std::bitset<32> word){
    std::bitset<32> f ;
    std::bitset<32> k = 0b10001111000110111011110011011100;
    f = ((B & C) | (B & D) ) | (C & D);
    std::bitset<32> temp = f.to_ulong() + k.to_ulong() + E.to_ulong() + lrotate(A , 5).to_ulong() + word.to_ulong();
    E = D;
    D = C;
    C = lrotate(B , 30);
    B = A;
    A = temp;
};

void function_4(std::bitset<32> word){
    std::bitset<32> f ;
    std::bitset<32> k = 0b11001010011000101100000111010110;
    f = (B ^ C) ^  D;
    std::bitset<32> temp = f.to_ulong() + k.to_ulong() + E.to_ulong() + lrotate(A , 5).to_ulong() + word.to_ulong() ;
    E = D;
    D = C;
    C = lrotate(B , 30);
    B = A;
    A = temp;
};

std::string hex(long num){
    std::stringstream stream;
    stream << std::hex << num;
    std::string hex_str(stream.str());
    if (hex_str.length() == 7){
        return "0" + hex_str;
    }
    return hex_str;
};

void main_loop(std::vector<std::bitset<32>> *words){
    int z = 0 ;
    for(int i=0 ; i < 80 ; ++i){
        // print((*words)[i]);
        if (i<20){
            function_1((*words)[i]);
        }

        else if ( i > 19 & i < 40){
            function_2((*words)[i]);
        }

        else if ( i > 39 & i < 60){
            function_3((*words)[i]);
        }

        else if ( i > 59 & i < 80){
            function_4((*words)[i]);
        }
        z++;
    }
    // print("+++++++++++++++++");
    // print(z);
    // z=0;
};



void loop(std::vector<std::vector<std::bitset<32>>> * list, std::string * full_data){
    
    std::bitset<32> H0 = 0b01100111010001010010001100000001;
    std::bitset<32> H1 = 0b11101111110011011010101110001001;
    std::bitset<32> H2 = 0b10011000101110101101110011111110;
    std::bitset<32> H3 = 0b00010000001100100101010001110110;
    std::bitset<32> H4 = 0b11000011110100101110000111110000;
    int index = 0;
    print(full_data->length());
    print(*full_data);
    print("=============================================");
    for (int i =0 ; i < full_data->length() ; i+=512){
        std::string word = full_data->substr(i , 512);
        std::vector<std::bitset<32>> s_word =  separate(word);
        list->push_back(s_word);
        index++;
        print(full_data->substr(i , 512));
        print("=============================================");
    }

    print(index);
    for (std::vector<std::bitset<32>>& vector : *list){
        main_loop(&vector);
    }
    std::bitset<32> h0 = H0.to_ulong() + A.to_ulong();
    std::bitset<32> h1 = H1.to_ulong() + B.to_ulong();
    std::bitset<32> h2 = H2.to_ulong() + C.to_ulong();
    std::bitset<32> h3 = H3.to_ulong() + D.to_ulong();
    std::bitset<32> h4 = H4.to_ulong() + E.to_ulong();
    std::string result = hex(h0.to_ulong()) + hex(h1.to_ulong()) +  hex(h2.to_ulong()) +  hex(h3.to_ulong()) + hex(h4.to_ulong());
    print(result);
};