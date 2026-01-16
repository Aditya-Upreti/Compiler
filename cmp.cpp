#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <vector>


enum class TokenType{
    _return,
    int_lit,
    semi
};

struct TOKEN {
    TokenType type;
    std::optional<std::string> value;
};



std::vector<TOKEN> tokenize(const std::string& str){
    std::vector<TOKEN> tokens;
    std::string buffer = "";
    for(int i=0;i<str.length();i++){
        if(isalpha(str.at(i))){
            buffer.push_back(str.at(i));
            i++;
            while(isalnum(str.at(i))){
                buffer.push_back(str.at(i));
                i++;
            }
            i--; 
            if(buffer == "return"){ 
                struct TOKEN t = {TokenType::_return};
                tokens.push_back(t);
                buffer.clear();
            }else{
                std::cout << "Shut up you f**king loser...\n";
                exit(EXIT_FAILURE);
            }
        }
        else if(std::isdigit(str.at(i))){
            buffer.push_back(str.at(i));
            i++;
            while(std::isdigit(str.at(i))){
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            struct TOKEN t = {TokenType::int_lit,buffer};
            tokens.push_back(t);
            buffer.clear();
        }
        else if(str.at(i) == ';'){
            struct TOKEN t = {TokenType::semi};
            tokens.push_back(t);
        }
        else if(std::isspace(str.at(i)))  continue;
    }
    return tokens;
}

std::string token_to_asm_converter(const std::vector<TOKEN>& tokens){
    std::stringstream output;
    output << "default rel\nsection .text\n\tglobal main\nmain:\n\tsub rsp,40\n";
    for(int i = 0; i<tokens.size(); i++){
        const TOKEN& token = tokens.at(i);
        if(token.type == TokenType::_return){
            if(i+1 <tokens.size() && tokens.at(i+1).type==TokenType::int_lit ){
                if(tokens.at(i+2).type==TokenType::semi){
                    output << "\tadd rsp, 40\n\tmov rax, " << *tokens.at(i+1).value << "\n\tret";
                }
            }
            
        }
    }
    return output.str();
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "no input file detected...\n expected >> fern <input.fr>";
        return EXIT_FAILURE;
    }
    // std::cout << argv[1] << endl;
    std::string content;
    {
        std::stringstream contents;
        std::fstream input(argv[1], std::ios::in);
        contents << input.rdbuf();
        content= contents.str();
    }

    // tokenize(content);
    // std::cout << token_to_asm_converter(tokenize(content)) << std::endl;
    std::vector<TOKEN> tokens = tokenize(content);
    {
        std::fstream file("./out.asm", std::ios::out);
        file << token_to_asm_converter(tokens);
    }
    system("nasm -f win64 out.asm -o out.o");
    system("gcc out.o -o out");
    // system("./out.exe");
    return EXIT_SUCCESS;
}