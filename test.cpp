#include <iostream>
#include <string>
#include <sstream>  //文字列が数字か判定


class Node {
public:
    virtual ~Node() {}  //仮想デストラクタ

    virtual int getValue(){
        return 0;
    }

    virtual void setLeft(Node* left_){}
    virtual void setRight(Node* right_){}
    virtual void setOp(char op_){}

};


class Integer : public Node {
public:
    int value;

    Integer(int value_) : value(value_) {}

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


Node* parseInteger(const std::string tokensa[], int& indexa, int sizea){

    Node* node1 = new Integer(std::stoi(tokensa[indexa]));
    indexa++;
    return node1;
}


Node* parserMul(const std::string tokensa[], int& indexa, int sizea){

    Node* node1 = parseInteger(tokensa, indexa, sizea);

    while( indexa < sizea && tokensa[indexa][0] == '*' ) {
        node1 = new BinaryExpression(node1, nullptr, tokensa[indexa][0]);
        indexa++;
        node1->setRight(parseInteger(tokensa, indexa, sizea));
    }

    return node1;

}


Node* parserAddSub(const std::string tokensa[], int& indexa, int sizea){

    Node* node1 = parserMul(tokensa, indexa, sizea);

    while( indexa < sizea && (tokensa[indexa][0] == '+' || tokensa[indexa][0] == '-') ) {
        node1 = new BinaryExpression(node1, nullptr, tokensa[indexa][0]);
        indexa++;
        node1->setRight(parserMul(tokensa, indexa, sizea));
    }

    return node1;

}








bool isInteger(const std::string& s){
    std::istringstream iss(s);
    int x;
    char c;
    return (iss >> x) && !(iss >> c);
}


int main() {

    std::string tokens[7] = {"2", "*", "3", "+", "5", "*", "5"};
    int index1 = 0;

    Node* node1 = parserAddSub(tokens, index1, 7);

    std::cout << node1->getValue() << std::endl;

    return 0;
}
