#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

void display_help() {
    std::cout << "A simple program that preprocesses plain UTF-8 text for encryption using classical ciphers\n";
    std::cout << "Usage: ./prepare <plaintext directory> <output directory> \n";
}

std::string normalize(const std::string & text) {
    std::string normalized("");
    unsigned char control_byte = 0x00;
    unsigned char push_char = 0x00;

    for( const unsigned char &c : text ) {
        push_char = 0x00;        
        // Handle diacritic
        if (control_byte == 0xc3) {
            push_char = c;
            if (0x9F < push_char && push_char < 0xC0){
                push_char -= 0x20;
            }

            // A diacritics
            if (0x7F < push_char && push_char < 0x86){
                push_char = 'a';
            }
            // Ç
            else if (push_char == 0x87){
                push_char = 'c';
            }
            // E diacritics
            else if (0x87 < push_char && push_char < 0x8c){
                push_char = 'e';
            }
            // I diacritics
            else if (0x8b < push_char && push_char < 0x90){
                push_char = 'i';
            }
            // Ð
            else if (push_char == 0x90){
                push_char = 'd';
            }
            // Ñ
            else if (push_char == 0x91){
                push_char = 'n';
            }
            // O diacritics
            else if (0x91 < push_char && push_char < 0x97 
                    || push_char == 0x98
                    ){
                push_char = 'o';
            }
            // U diacritics
            else if (0x98 < push_char && push_char < 0x9D){
                push_char = 'u';
            }
            // Ý
            else if (push_char == 0x9D){
                push_char = 'y';
            }
            else{
                push_char = 0x00;
            }
        }
        // Lower if not diacritic
        else if (0x40 < c && c < 0x5B) {
            push_char = c + 0x20;
        }
        else if (0x60 < c && c < 0x7B) {
            push_char = c;
        }
        
        if ( push_char != 0x00 ){
            normalized += push_char;
        }
        control_byte = c;
    }

    return normalized;
}

void prepare_classic(const std::string & input_file, const std::string & output_file) {
    std::filesystem::path p{input_file};
    int size = std::filesystem::file_size(p);

    std::ifstream istream(input_file, std::ios::binary);
    std::streambuf* raw_buffer = istream.rdbuf();

    char* block = new char[size];
    raw_buffer->sgetn(block, size);

    std::cout << "INPUT:\n" << normalize(std::string(block)) << "\n";

    delete[] block;
}

int main(int argc, char* argv[]){

    if (argc != 3) {
        std::cerr << "[ERROR] No arguments provided\n\n";
        display_help();
        exit(1);
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    prepare_classic(input_file, output_file);

    return 0;
}