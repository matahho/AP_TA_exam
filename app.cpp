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

#define INPUT "input"
#define OPERATOR "operator"



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
        if (t == INPUT || t == OPERATOR ) {
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
    virtual int calculate (vector<double> children) = 0 ;

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
    void print(){cout << getIndex() << " " << getParent() << " " << getType() << " " << getValue() << " " << getChildern().size() ;}

};


class Operand : public Node{
public:
    Operand(int i , int p , string t , int v )
        :Node (i , p , t , v){
            base = D;
    }
    // Abstract Class
    void correctness (){
        if (this->getChildern().size() != 0)
            throw invalid_argument("Wrong tree structure !");
    }
    int calculate(vector<double> children){return this->getValue();}
private:
    int base ;
};


class Operator : public Node{
protected:
    int mode ;
    vector<double> childrenValues (){
        vector<double> values ; 
        for (int i = 0 ; i < getChildern().size() ; i++)
            values.push_back(getChildern()[i]->getValue());
        return values;
    }
public:
    Operator(int i , int p , string t , int v )
        :Node (i , p , t , v){
            mode = ARITHMETIC ;
    }
 
    // Abstract Class
    virtual void correctness () = 0 ;
    virtual int calculate(vector<double> children) = 0 ;

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

    int calculate (vector<double> children){
        return (-children[0]);
    
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

    int calculate (vector<double> childern){
        return (childern[0] * childern[1]);

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

    int calculate (vector<double> childern){
        return (childern[0] + childern[1]);
    }

};


double median (vector<double> children){

    int n = children.size(); 
    if (n % 2 == 0) {
        nth_element(children.begin() , children.begin() + n / 2 , children.end());

        nth_element(children.begin() , children.begin() + (n - 1) / 2 , children.end());
  

        return (double)(children[(n - 1) / 2]+ children[n / 2]) / 2.0;
    }
  

    else {
        nth_element(children.begin() , children.begin() + n / 2 , children.end());
        return (double)children[n / 2];
    }
}





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


    int calculate (vector<double> childern){
        return (median(childern));

    }


};






class Tree {
private:
    void adjustParents ();
    void adjustChildren ();
    bool TreeCorrectness ();
    Node* findNodeByIndex (int ind);

public:
    // Here I used POLYMORPHISEM :)
    vector <Node*> stack;
    void push (int i , int p , string t , int value);
    void evaluate ();
    
    int calculate(Node* root);


    void printTree(){
        for (Node* i : stack)
            cout << i->getIndex() << " " << i->getParentIndex() << " " << i->getType() << " " <<i->getValue() << " " << i->getChildern().size() << endl;
    }

};

void Tree::push(int i , int p , string t , int v){
    // Here I used POLYMORPHISEM :) and ABSTRACT class
    if (t == INPUT){
        Operand *inp = new Operand(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == OPERATOR && v == NOT){
        Operator *inp = new NotOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == OPERATOR && v == AND){
        Operator *inp = new AndOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == OPERATOR && v == OR){
        Operator *inp = new OrOperator(i , p , t , v);
        stack.push_back(inp);
    }
    else if (t == OPERATOR && v == XOR){
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



int Tree::calculate(Node* root){
    if (root == NULL)
        throw invalid_argument("No tree Found !");

    if (root->isLeaf())
        return root->getValue();
    
    vector<double> res ;
    for (Node* child : root->getChildern()){
                res.push_back(calculate(child));
            
    }

    return root->calculate(res);




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
    t.push(0 , -1 , OPERATOR , OR);
    t.push(1 , 0 , INPUT , 5);
    t.push(2 , 0 , OPERATOR , AND);

    t.push(3 , 2 , INPUT , 2);
    t.push(4 , 2 , OPERATOR , NOT);

    t.push(5 , 4 , OPERATOR , OR);

    t.push(6 , 5 , INPUT , 2);
    t.push(7 , 5 , INPUT , 3);

    
    


    t.evaluate();
    t.printTree();

    cout << "\n\n\n";
    cout << t.calculate(t.stack[0]) << endl;



} 


