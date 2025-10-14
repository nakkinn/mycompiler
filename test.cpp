#include <iostream>

class Node {
public:
    virtual int returnValue() = 0;  // 純粋仮想関数に変更
    virtual ~Node() {}               // 安全な削除のため仮想デストラクタ
};


class Integer : public Node {
public:
    int value;
    Integer(int value_) : value(value_) {}
    int returnValue() override { return value; }
};


class BinaryExpression : public Node {
public:
    char op;
    Node* left;
    Node* right;

    BinaryExpression(char op_, Node* left_, Node* right_)
        : op(op_), left(left_), right(right_) {}

    int returnValue() override {
        if(op == '+') return left->returnValue() + right->returnValue();
        if(op == '-') return left->returnValue() - right->returnValue();
        if(op == '*') return left->returnValue() * right->returnValue();
        return 0;
    }
};


int main() {
    Integer int1(10);
    Integer int2(20);
    BinaryExpression expr1('+', &int1, &int2);
    BinaryExpression expr2('*', &expr1, &int2);

    std::cout << expr1.returnValue() << std::endl; // 30
    std::cout << expr2.returnValue() << std::endl; // 600

    return 0;
}
