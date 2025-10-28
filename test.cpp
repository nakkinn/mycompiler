#include <iostream>
#include <string>
#include <sstream>  //文字列が数字か判定
#include <vector> //可変長配列
#include <fstream> //ファイル操作
#include <cstdlib> //system関数用


class Node {
public:
    virtual ~Node() {}  //仮想デストラクタ

    virtual int getValue(){
        return 0;
    }

    virtual void compile(std::string& s1){}

    virtual void setLeft(Node* left_){}
    virtual void setRight(Node* right_){}
    virtual void setOp(char op_){}

};


class Integer : public Node {
public:
    int value;

    Integer(int value_) : value(value_) {}

    void compile(std::string& s1){
        s1 += "    push " + std::to_string(value) + "\n";
    }

    int getValue() override {
        return value;
    }
};


class BinaryExpression : public Node {
public:
    Node* left;
    Node* right;
    char op;

    BinaryExpression(Node* left_, Node* right_, char op_) : left(left_), right(right_), op(op_) {}

    int getValue() override {
        if (op == '+')  return left->getValue() + right->getValue();
        else if (op == '-') return left->getValue() - right->getValue();
        else if (op == '*') return left->getValue() * right->getValue();
        return 0;
    }

    void compile(std::string& s1){
        left->compile(s1);
        right->compile(s1);
        s1 += "    pop eax\n";
        s1 += "    pop ebx\n";
        if( op == '+') s1 += "    add eax, ebx\n";
        else if( op == '-') s1 += "    sub eax, ebx\n";
        else if( op == '*') s1 += "    imul eax, ebx\n";
        s1 += "    push eax\n";
    }

    void setLeft(Node* left_) override{
        left = left_;
    }

    void setRight(Node* right_) override{
        right = right_;
    }

    void setOp(char op_) override{
        op = op_;
    }

};


Node* parseInteger(const std::vector<std::string>& tokensa, int& indexa){

    Node* node1 = new Integer(std::stoi(tokensa[indexa]));
    indexa++;
    return node1;
}


Node* parserMul(const std::vector<std::string>& tokensa, int& indexa){

    Node* node1 = parseInteger(tokensa, indexa);

    while( indexa < tokensa.size() && tokensa[indexa][0] == '*' ) {
        node1 = new BinaryExpression(node1, nullptr, tokensa[indexa][0]);
        indexa++;
        node1->setRight(parseInteger(tokensa, indexa));
    }

    return node1;

}


Node* parserAddSub(const std::vector<std::string>& tokensa, int& indexa){

    Node* node1 = parserMul(tokensa, indexa);

    while( indexa < tokensa.size() && (tokensa[indexa][0] == '+' || tokensa[indexa][0] == '-') ) {
        node1 = new BinaryExpression(node1, nullptr, tokensa[indexa][0]);
        indexa++;
        node1->setRight(parserMul(tokensa, indexa));
    }

    return node1;

}








bool isInteger(const std::string& s){
    std::istringstream iss(s);
    int x;
    char c;
    return (iss >> x) && !(iss >> c);
}


void execute(const std::string& s){

    std::ofstream outfile("output.asm");  //ファイル生成
    outfile << s;  //ファイルに書き込み
    outfile.close();

    // システムコマンドでアセンブル、リンク、実行
    int ret = system("nasm -f win32 output.asm -o output.obj && gcc -m32 output.obj -o output.exe && output.exe");

}




std::string asm_header = R"(; hello.asm - Windows console app (x86, 32-bit)

global _main
extern _printf

section .data
    fmt db "%d", 10, 0
    x db 100

section .text


print:
    push eax
    push fmt
    call _printf
    add esp, 8
    ret;

_main:
)";

std::string asm_footer = R"(
    call print
    ret)";




int main(int argc, char* argv[]) {


    std::ifstream file(argv[1]);
    if(!file){
        std::cerr << "ファイルが開けませんでした。" << std::endl;
        return 1;
    }


    std::string line1;

    std::getline(file, line1);
    file.close();


    std::vector<std::string> list1;
    std::string tmp1;
    
    for(int i=0; i<line1.size(); i++){
        char c1 = line1[i];

        if(c1 >= '0' && c1 <= '9'){
            tmp1 += c1;
        }else{
            if(tmp1 != ""){
                list1.push_back(tmp1);
                tmp1 = "";
            }
            if(c1 == '+' || c1 == '-' || c1 == '*'){
                std::string s1(1, c1);  //文字を文字列に変換
                list1.push_back(s1);
            }
        }

    }

    if(tmp1 != ""){
        list1.push_back(tmp1);
        tmp1 = "";
    }


    int index1 = 0;
    Node* node1 = parserAddSub(list1, index1);

    std::string s1 = "";
    node1->compile(s1);

    // std::cout << asm_header + s1 + asm_footer << std::endl;


    execute(asm_header + s1 + asm_footer);



    return 0;
}
