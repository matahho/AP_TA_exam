#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>


using namespace std;


#define AND 1
#define OR 2
#define NOT 3 
#define XOR 4

#define BITWISE 1
#define ARITHMETIC 2

#define B 2
#define O 8
#define D 10
#define H 16

#define INPUT "input"
#define OPERATOR "operator"



class Node {
protected:
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
    virtual int calculate (vector<int> children) = 0 ;
    virtual void reset(int newValue) = 0 ;
    virtual void convert() = 0 ;

    int getValue() {return value ;}
    string getType() {return type ;}
    int getIndex() {return index;}
    int getParentIndex(){ return parentIndex;}
    void setParentIndex(int ind) {parentIndex = ind;}
    void setParent(Node* p) {parent = p;}
    void setChildren(vector<Node*> list) {
        for (int i = 0; i < list.size() ; i++)
            children.push_back(list[i]);
    }
    Node* getParent() {return parent;}
    void addChildren(Node* c) {children.push_back(c);}
    void removeChildren(int ind){
        for (int i =0 ; i < getChildern().size() ; i++)
            if (getChildern()[i]->getIndex() == ind)
                children.erase(children.begin()+i);
    }
    vector<Node*> getChildern() {return children;}
    bool isLeaf() {return children.size() == 0 ;}
    void print(){cout << getIndex() << " " << getParent() << " " << getType() << " " << getValue() << " " << getChildern().size() ;}


};


bool checkNumberChar(int num,vector<char> ch) {
    std::string numChar = std::to_string(num);

    for (int i = 0; i < numChar.size(); i++) {
        bool found = false;
        for (int j = 0; j < ch.size(); j++) {
            if (numChar[i] == ch[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false; 
        }
    }

    return true;
}


class Operand : public Node{
public:
    Operand(int i , int p , string t , int v )
        :Node (i , p , t , v){
            base = D;
    }
    // Abstract Class
    
    virtual void convert() = 0 ;

    void correctness (){
        if (this->getChildern().size() != 0)
            throw invalid_argument("Wrong tree structure !");

    }

    int calculate(vector<int> children){return this->getValue();}
    void reset (int newValue){value = newValue;}

private:
    int base ;
    // vector<char>possibleChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

};


class DecimalOperand : public Operand{
public:
    DecimalOperand(int i , int p , string t , int v )
        :Operand (i , p , t , v){
            base = B;
    }


    void convert() {
        // if (!checkNumberChar(this->getValue() , possibleChar))
        //     throw invalid_argument("Invalid base transfomation !");
        int count = stoi(to_string(value));
        value = count ;
    }

private:
    int base ;
    // vector<char>possibleChar = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

};


class BinaryOperand : public Operand{
public:
    BinaryOperand(int i , int p , string t , int v )
        :Operand (i , p , t , v){
            base = B;
    }

    void convert() {
        // if (!checkNumberChar(this->getValue() , possibleChar))
        //     throw invalid_argument("Invalid base transfomation !");
        bitset<32> binary(value);
        int count = binary.count();
        value = count ;
    }

private:
    int base ;
    // vector<char>possibleChar = {'0', '1'};

};





class Operator : public Node{
protected:
    int mode ;
    vector<int> childrenValues (){
        vector<int> values ; 
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
    virtual int calculate(vector<int> children) = 0 ;

    void reset(int newValue) {return ;}
    void convert() {return ;}


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

    int calculate (vector<int> children){
        return (-children[0]);
    
    }

};


class NotBitwise: public NotOperator{
private:
    
public:
    NotBitwise(int i , int p , string t , int v )
        :NotOperator (i , p , t , v){}
    

    int calculate (vector<int> children){
        return (~children[0]);
    
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

    int calculate (vector<int> childern){
        return (childern[0] * childern[1]);

    }

};


class AndBitwise : public AndOperator{
private:
    
public:
    AndBitwise(int i , int p , string t , int v )
        :AndOperator (i , p , t , v){}

    int calculate (vector<int> childern){
        return (childern[0] & childern[1]);

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

    int calculate (vector<int> childern){
        return (childern[0] + childern[1]);
    }

};


class OrBitwise : public OrOperator{
private:
    
public:
    OrBitwise(int i , int p , string t , int v )
        :OrOperator (i , p , t , v){}

    int calculate (vector<int> childern){
        return (childern[0] | childern[1]);
    }

};




double median (vector<int> children){

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


    int calculate (vector<int> childern){
        return (median(childern));

    }


};

int exclusive(vector<int> children){
    int result = 0;
    for (int i = 0 ; i < children.size() ; i++)
        result = result ^ children[i];
    return result;
}


class XorBitwise : public XorOperator{
private:
    
public:
    XorBitwise(int i , int p , string t , int v )
        :XorOperator (i , p , t , v){}


    int calculate (vector<int> childern){
        return (exclusive(childern));
    }


};





class Tree {
private:
    void adjustParents ();
    void adjustChildren ();
    bool TreeCorrectness ();


public:
    Node* findNodeByIndex (int ind);
    // Here I used POLYMORPHISEM :)
    vector <Node*> stack;
    void push (int i , int p , string t , int value);
    void remode (int ind , int mode);
    void reset (int ind , int val);
    void evaluate ();
    void removeNodeByIndex (int ind);
    int calculate(Node* root);
    void rebase (int ind , string newBase);


    void printTree(){
        for (Node* i : stack){
            cout << i->getIndex() << " " << i->getParentIndex() << " " << i->getType() << " " <<i->getValue() << " " << i->getChildern().size() << endl;
            for (Node* j : i->getChildern()){
                cout << "\nchild";j->print();cout <<endl; 
            }
        }
    }

};

void Tree::reset(int ind , int val){
    Node* nod = findNodeByIndex(ind);
    if (nod->getType() == OPERATOR)
        throw invalid_argument("Reset is not avalibale on operator");
    nod->reset(val);
}


void Tree::rebase(int ind , string newBase){
    Node* theNode = findNodeByIndex(ind);


    if (theNode->getType() == OPERATOR)
        throw invalid_argument("Rebase is not avalibale on operator");


    if (newBase == "-b"){
        
        Operand *inp = new BinaryOperand(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->convert();
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        inp->getParent()->removeChildren(theNode->getIndex());
        inp->getParent()->addChildren(inp);
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return ;
    }

    if (newBase == "-d"){
    
        Operand *inp = new DecimalOperand(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->convert();
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        inp->getParent()->removeChildren(theNode->getIndex());
        inp->getParent()->addChildren(inp);
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return ;
    }


}




void Tree::removeNodeByIndex(int ind){
    for (int i = 0; i < stack.size(); i++){
        if (stack[i]->getIndex() == ind){
            stack.erase(stack.begin() + i);
            return ;
        }
    }
    throw invalid_argument("No Index Found");
}



void Tree::push(int i , int p , string t , int v){
    // Here I used POLYMORPHISEM :) and ABSTRACT class
    if (t == INPUT){
        Operand *inp = new DecimalOperand(i , p , t , v);
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

void Tree::remode(int ind , int mode){
    Node* theNode = findNodeByIndex(ind);


    if (theNode->getType() == INPUT)
        throw invalid_argument("Invalid Syntax On Operand");


    if (theNode->getValue() == NOT && mode == ARITHMETIC){
        Operator *inp = new NotOperator(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return ;
    }

    if (theNode->getValue() == NOT && mode == BITWISE){
        Operator *inp = new NotBitwise(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }

    if (theNode->getValue() == AND && mode == ARITHMETIC){
        Operator *inp = new AndOperator(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }

    if (theNode->getValue() == AND && mode == BITWISE){
        Operator *inp = new AndBitwise(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);

        return;
    }


    if (theNode->getValue() == OR && mode == ARITHMETIC){
        Operator *inp = new OrOperator(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }

    if (theNode->getValue() == OR && mode == BITWISE){
        Operator *inp = new OrBitwise(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }



    if (theNode->getValue() == XOR && mode == ARITHMETIC){
        Operator *inp = new XorOperator(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }

    if (theNode->getValue() == XOR && mode == BITWISE){
        Operator *inp = new XorBitwise(theNode->getIndex() , theNode->getParentIndex() , theNode->getType() , theNode->getValue());
        inp->setParent(theNode->getParent());
        inp->setChildren(theNode->getChildern());
        removeNodeByIndex(ind);
        stack.push_back(inp);
        return;
    }


}




Node* Tree::findNodeByIndex(int ind){
    for (int i = 0; i < stack.size(); i++){
        if (stack[i]->getIndex() == ind)
            return stack[i];
    }
    throw invalid_argument("No Index Found");
}


void Tree::adjustParents(){
    for (int i=0 ; i < stack.size() ; i++){
        if (stack[i]->getIndex() == 0){
            stack[i]->setParent(NULL);
            stack[i]->setParentIndex(-1);
        }else 
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
    
    vector<int> res ;
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

// void tuneTree(Tree& t){

//     int numberOfNodes = 0;
//     cin >> numberOfNodes;
//     int ind = 0 , par = 0 ; string val , typ ; 
    
//     for (int n=0 ; n < numberOfNodes ; n++){
//         cin >> ind >> par >> typ >> val ; 
//         if (val == "OR"){val = to_string(OR);}
//         if (val == "AND"){val = to_string(AND);}
//         if (val == "NOT"){val = to_string(NOT);}
//         if (val == "XOR"){val = to_string(XOR);}

//         t.push(ind , par , typ , stoi(val));
//     }

// }


// void commandLine (Tree& t){
//     tuneTree(t);


//     t.evaluate();

//     t.printTree();

//     cout << "\n\n\n";
//     t.stack[0]->print();
    
//     //cout << t->calculate(t->stack[0]) << "\n\n\n" << endl;

// }



int findTheMode (string m){
    int mode = 0 ;
    if (m == "-b")
        return BITWISE;
    if (m == "-a")
        return ARITHMETIC;
    throw invalid_argument("Invalid Mode. Supported Modes: -b, -a");

}



int main (){  
    Tree t = Tree() ;
    


    int numberOfNodes = 0;
    cin >> numberOfNodes;
    int ind = 0 , par = 0 ; string val , typ ; 
    
    for (int n=0 ; n < numberOfNodes ; n++){
        cin >> ind >> par >> typ >> val ; 
        if (val == "OR"){val = to_string(OR);}
        if (val == "AND"){val = to_string(AND);}
        if (val == "NOT"){val = to_string(NOT);}
        if (val == "XOR"){val = to_string(XOR);}

        t.push(ind , par , typ , stoi(val));
    }

    t.evaluate();

    string cmd ; 
    while (cin >> cmd){
        if (cmd == "evaluate" ){
            string bas ;
            cin >> bas ;
            
            
            int result = t.calculate(t.findNodeByIndex(0)) ;
            cout << result << endl;
        }
        if (cmd == "remode"){
            string mod ; int ind;
            cin >> mod >>ind ;
            int mode = findTheMode(mod);
            

            t.remode(ind , mode);
        }
        if (cmd == "reset"){
            int ind , val;
            cin >> ind >> val;


            t.reset(ind , val);

        }
        if (cmd == "rebase"){
            string base ; 
            int ind;
            cin >> base >> ind;


            t.rebase(ind , base);
        }

    }       



} 


