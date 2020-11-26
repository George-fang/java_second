#include "IOC.h"

class ClassB{
public:
    int ba;
    int bb;
    int bc;
};

class ClassA{
public:
    int aa;
    int ab;
    int ac;
    ClassB* temp;
};

int main () {
    IOC temp;
    ClassA a{};
    temp.add_class<int>();
    temp.add_class<ClassB>();
    temp.add_class<ClassB*>();
    temp.add_class<ClassA>();
    temp.add_member<ClassB, int>(0);
    temp.add_member<ClassB, int>(4);
    temp.add_member<ClassB, int>(8);
    temp.add_member<ClassA, int>(0);
    temp.add_member<ClassA, int>(4);
    temp.add_member<ClassA, int>(8);
    temp.add_member<ClassA, ClassB*>(16);
    temp.build_all();
    ClassA* tempClassA = temp.get_class<ClassA>();
    ClassB* tempClassB = temp.get_class<ClassB>();
    cout << "tempClassA addr : " << tempClassA << endl;
    cout << "tempClassA.aa addr : " << &tempClassA->aa << endl;
    cout << "tempClassA.ab addr : " << &tempClassA->ab << endl;
    cout << "tempClassA.ac addr : " << &tempClassA->ac << endl;
    cout << "tempClassA->temp addr: " << &tempClassA->temp << endl;
    cout << "tempClassA->temp point to: " << tempClassA->temp << endl;
    cout << "tempClassB addr : " << tempClassB << endl;
    cout << "&tempClassB->ba : " << &tempClassB->ba << endl;
    cout << "&tempClassB->bb : " << &tempClassB->bb << endl;
    cout << "&tempClassB->bc : " << &tempClassB->bc << endl;

    tempClassA->temp->bc = 233;
    cout << tempClassB->bc << endl;
    return 0;
}