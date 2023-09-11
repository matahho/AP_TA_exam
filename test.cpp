

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
            return false; // If a character is not found in ch, return false immediately.
        }
    }

    return true;
}


int main (){
    vector<char> a = {'1' , '0'};
    cout << checkNumberChar(1010 ,a );



    


}