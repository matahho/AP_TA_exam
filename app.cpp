#include <iostream>
#include <string>
#include <vector>

using namespace std;


#define AND 1
#define OR 2
#define NOT 3 
#define XOR 4



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
public:
    Operator(int i , int p , string t , int v )
        :Node (i , p , t , v){

    }

private:

};


class Tree {
private:
    void adjustParents ();
    void adjustChildren ();
    double calculate (Node* root);
    double operatorResult (Node* op , vector<Node*> vars);
    bool TreeCorrectness ();

public:
    vector <Node> stack;
    void push (Node n );
    Node* findNodeByIndex (int ind);
    void evaluate ();

    void printTree (){
        for (Node i : stack){
            cout << i.getIndex() << " " << i.getParentIndex() << " " << i.getType() << " " << i.getValue()  << "\n"; 
        }
    }

};

void Tree::push(Node n ){
    if (n.getIndex() == 0 )
        n.setParentIndex(-1);
    stack.push_back(n);
}

Node* Tree::findNodeByIndex(int ind){
    for (int i = 0; i < stack.size(); i++){
        if (stack[i].getIndex() == ind)
            return &stack[i];
    }
    return NULL;
}

void Tree::evaluate(){
    this->adjustParents();
    this->adjustChildren();
    this->TreeCorrectness();
    

}


void Tree::adjustParents(){
    for (int i=0 ; i < stack.size() ; i++){
        if (stack[i].getIndex() == 0)
            stack[i].setParent(NULL);
        stack[i].setParent(findNodeByIndex(stack[i].getParentIndex())) ;
    }
}

void Tree::adjustChildren(){
    for (int i=0 ; i < stack.size() ; i++){
        for (int j=0 ; j < stack.size() ; j++){
            if (stack[j].getParent() != NULL && stack[j].getParent()->getIndex() == stack[i].getIndex() ){
                stack[i].addChildren(&stack[j]);
            }
        }
    }
}


bool Tree::TreeCorrectness(){
    for (int i=0 ; i < stack.size() ; i++){
        if (stack[i].getType() == "operator"){
            if (stack[i].getValue() == AND){
                if (stack[i].getChildern().size() != 2)
                    cout << "EROOR" << endl;
            }
            if (stack[i].getValue() == OR){
                if (stack[i].getChildern().size() != 2)
                    cout << "EROOR" << endl;
            }
            if (stack[i].getValue() == NOT){
                if (stack[i].getChildern().size() != 1)
                    cout << "EROOR" << endl;
            }
            if (stack[i].getValue() == XOR){
                if (stack[i].getChildern().size() < 2)
                    cout << "EROOR" << endl;
            }
        }
    }
    return 1;
}



double Tree::calculate(Node* root){
    if (root == NULL )
        return 0;
    if (root->isLeaf())
        return root->getValue();
      
}

double Tree::operatorResult (Node* op , vector<Node*> vars){
    if (op->getType() == "operator" && op->getValue() == AND)
        return 9.9; 
}




int main ()
{  
    Tree t = Tree() ;
    t.push(Node (0 , 100 ,"operator" , NOT));
    t.push(Node (2 , 0 ,"operator" , AND));
    t.push(Node (3 , 2 , "input" , 100));
    t.push(Node (4 , 2 , "input" , 1300));


    t.printTree();
    t.evaluate();




} 


