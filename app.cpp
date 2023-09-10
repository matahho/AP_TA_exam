#include <iostream>
#include <string>
#include <vector>

using namespace std;


#define AND 1
#define OR 2
#define NOT 3 
#define XOR 4

#define BITWISE 0 
#define ARITHMETIC 1

#define B 2
#define O 8
#define D 10
#define H 16





class Node {
private:
    int index ;
    int parentIndex ;
    string type ; 
    int value ;
    Node* parent ; 
    vector<Node*> children ;


public:
    Node (int i , int p , string t , int v){
        if (t == "input" || t == "operator" ) {
            index = i ;
            parentIndex = p ;
            type = t ;
            value = v ;
        }else {
            throw invalid_argument("Invalid input for node !");
        }
    }

    // Abstract Class
    virtual void correctness () = 0 ;

    int getValue() {return value ;}
    string getType() {return type ;}
    int getIndex() {return index;}
    int getParentIndex(){ return parentIndex;}
    void setParentIndex(int ind) {parentIndex = ind;}
    void setParent(Node* p) {parent = p;}
    Node* getParent() {return parent;}
    void addChildren(Node* c) {children.push_back(c);}
    vector<Node*> getChildern() {return children;}
    bool isLeaf() {return children.size() == 0 ;}

};

class Operator : public Node{
private:
    int mode ;
public:
    Operator(int i , int p , string t , int v )
        :Node (i , p , t , v){
            mode = ARITHMETIC ;
    }
 
    // Abstract Class
    virtual void correctness () = 0 ;

};

class NotOperator : public Operator{
private:
    
public:
    NotOperator(int i , int p , string t , int v )
        :Operator (i , p , t , v){}

    void correctness (){
        if (this->getChildern().size() != 1 )
            throw invalid_argument("Wrong tree structure !");
    }

};




class AndOperator : public Operator{
private:
    
public:
    AndOperator(int i , int p , string t , int v )
        :Operator (i , p , t , v){}

    void correctness (){
        if (this->getChildern().size() != 2 )

            throw invalid_argument("Wrong tree structure !");
    }

};



class OrOperator : public Operator{
private:
    
public:
    OrOperator(int i , int p , string t , int v )
        :Operator (i , p , t , v){}

    void correctness (){
        if (this->getChildern().size() != 2 )
            throw invalid_argument("Wrong tree structure !");
    }

};

class XorOperator : public Operator{
private:
    
public:
    XorOperator(int i , int p , string t , int v )
        :Operator (i , p , t , v){}

    void correctness (){
        // ASSUMPTION : In XOR we can have <2> or more operand  
        if (this->getChildern().size() < 2 )
            throw invalid_argument("Wrong tree structure !");
    }

};




class Operand : public Node{
public:
    Operand(int i , int p , string t , int v )
        :Node (i , p , t , v){
            base = D;
    }
    // Abstract Class
    void correctness (){
        return ;
    }
private:
    int base ;
};


class Tree {
private:
    void adjustParents ();
    void adjustChildren ();
    double calculate (Node* root);
    double operatorResult (Node* op , vector<Node*> vars);
    bool TreeCorrectness ();
    Node* findNodeByIndex (int ind);

public:
    // Here I used POLYMORPHISEM :)
    vector <Node*> stack;
    void push (int i , int p , string t , int value);
    void evaluate ();

};

void Tree::push(int i , int p , string t , int v){
    // Here I used POLYMORPHISEM :) and ABSTRACT class
    if (t == "input"){
        Operand *inp = new Operand(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == "operator" && v == NOT){
        Operator *inp = new NotOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == "operator" && v == AND){
        Operator *inp = new AndOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == "operator" && v == OR){
        Operator *inp = new OrOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == "operator" && v == XOR){
        Operator *inp = new XorOperator(i , p , t , v);
        stack.push_back(inp);
    }
    
}

Node* Tree::findNodeByIndex(int ind){
    for (int i = 0; i < stack.size(); i++){
        if (stack[i]->getIndex() == ind)
            return stack[i];
    }
    return NULL;
}


void Tree::adjustParents(){
    for (int i=0 ; i < stack.size() ; i++){
        if (stack[i]->getIndex() == 0){
            stack[i]->setParent(NULL);
            stack[i]->setParentIndex(-1);
        }
        stack[i]->setParent(findNodeByIndex(stack[i]->getParentIndex())) ;
    }
}


void Tree::adjustChildren(){
    for (int i=0 ; i < stack.size() ; i++){
        for (int j=0 ; j < stack.size() ; j++){
            if (stack[j]->getParent() != NULL && stack[j]->getParent()->getIndex() == stack[i]->getIndex() ){
                stack[i]->addChildren(stack[j]);
            }
        }
    }
}


bool Tree::TreeCorrectness(){
    for (int i=0 ; i < stack.size() ; i++){
        stack[i]->correctness();
    }
    return 1;
}






void Tree::evaluate(){
    try{
        this->adjustParents();
        this->adjustChildren();
        this->TreeCorrectness();
        
    }catch (exception &e) {
        cout << e.what() << endl;
    }
    

}


int main ()
{  
    Tree t = Tree() ;
    t.push(0 , 100 ,"operator" , XOR);
    t.push(1 , 0 , "input" , 100);



    t.evaluate();





} 


