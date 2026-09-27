// JsonParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
*  The grammer that will be used here ->
* 
* 
J -> { I }
I->P, I | P | EPSILON
LI->Elem, LI | Elem | EPSILON
P->V : Right
Right - > J | V | L | null
Elem - > V | J | null
V->T | F | N | S
T->True
F->False
S->string representation
N->numeric representation
L ->[LI]

Go through the comments on top of each method to quickly understand the flow.
*/


#include <iostream>
#include<string>
#include<fstream>
#include<iterator>

using namespace std;

class JsonParser {
    public:
        JsonParser(string txt) : txt{ txt } { pos = 0; };

        bool parse() {
            skip_space_newline_chars();
            
            if (J()) {
                if (pos >= txt.length())
                    return true;
                else
                    return false;
            }
            else {
                return false;
            }
        }

    private:
        string txt;
        int pos;

        // Safe way to retrieve char without going out of bounds . GPT suggestion :->
        char current() {
            if (pos >= txt.length())
                return '\0';
            else
                return txt.at(pos);
        }

        // My variant of it for same function where we use temperorayr char position variable.
        char current(int pos) {
            if (pos >= txt.length())
                return '\0';
            else
                return txt.at(pos);
        }

        // To check end of string .
        bool eot() {
            //cout << pos << "End check!\n";
            if (pos >= txt.length())
                return true;
            else
                return false;
        }

        // This skips spaces and newlines and takes the pos variable upto the most important point.
        void skip_space_newline_chars() {
            //cout << pos << "Skipping spaces\n";
            while ((!eot()) && (current() == ' ' || current() == '\n'))
                pos++;
        }

        // This is to parse number. One thing to be added in this is support for decimal.
        // Realized while adding this comment lol.
        bool numeric_representation() {
            //cout << pos << "Numeric Repr\n";

            int temp_pos = pos;

            while (isdigit(current(temp_pos)) && current(temp_pos) != '\0') 
                temp_pos++;

            if (temp_pos > pos) {
                pos = temp_pos;
                skip_space_newline_chars();
                return true;
            }

            return false;

            
        }

        // This is to parse valid string representations.
        bool string_representation() {
            //cout << pos << "String Repr\n";
            int temp_pos = pos;
            if (current(temp_pos) == '"') {
                temp_pos++;
                while (current(temp_pos) != '"') {
                    if (current(temp_pos) == '\0')
                        return false;
                    temp_pos++;
                }
                temp_pos++;
                pos = temp_pos;
                skip_space_newline_chars();
                return true;
            }
            return false;

        }
        
        // This is to parse keywords in json like 'true', 'false', 'null'
        bool match_word(string word) {
            //cout << pos << "Matching word --" << word << endl;
            int temp_pos = pos;
            
            while ((temp_pos < txt.length()) && ((temp_pos - pos) < word.length()) && (current(temp_pos) == word.at(temp_pos - pos)))
                temp_pos++;

            if ((temp_pos - pos) == word.length()) {
                pos = temp_pos;
                skip_space_newline_chars();
                return true;
            }

            return false;
        }

        // Object parsing method.
        bool J() {
            //cout << pos << "J --\n";
            if (current() == '{') {
                pos++;
                skip_space_newline_chars();

                if (current() == '}') {
                    pos++;
                    skip_space_newline_chars();
                    return true;
                }
                if (I()) {
                    //cout << "End of object item!!";

                    if (current() == '}') {
                        //cout << "End bracket of object!!";
                        pos++;
                        skip_space_newline_chars();
                        return true;
                    }
                }
            }
            return false;

        };


        // Parses I in the grammer.
        bool I() { 
            //cout << pos << "I --\n";


            

            if (P()) {
                if (current() == ',') {
                    pos++;
                    skip_space_newline_chars();
                    if (I())
                        return true;
                }
                else
                    return true;

            }

            
            return false; 
        };

        // Parses P in the grammer.
        bool P() { 
            //cout << pos << "P --\n";

            if(V())
                if (current() == ':') {
                    pos++;
                    skip_space_newline_chars();
                    if (RHS())
                        return true;

                }

            return false; 
        };

        // Parses V in the grammer. V kind of stands for value. So strings,numbers,bools and nulls come under it but null is skipped in this as it is a bit of a special case.
        // e.g. {'a' : null} is fine but {null : 'a'} is not. 
        bool V() {
            //cout<<pos << "V --\n";

            if (match_word("true"))
                return true;
            if (match_word("false"))
                return true;
            if (numeric_representation())
                return true;
            if (string_representation())
                return true;

            return false;

        
        };

        // This is the list parsing method.
        bool L() {
            //cout << pos << "L --\n";

            if (current() == '[') {
                pos++;
                skip_space_newline_chars();
                if (current() == ']') {
                    pos++;
                    skip_space_newline_chars();
                    return true;
                }
                if(LI())
                    if (current() == ']') {
                        pos++;
                        skip_space_newline_chars();
                        return true;
                    }

            }

            return false;

        };

        // Helps in parsing list elements.
        bool LI() {
            //cout << pos << "LI --\n";


            if (Elem()) {
                if (current() == ',') {
                    pos++;
                    skip_space_newline_chars();
                    if (LI())
                        return true;
                }else
                    return true;
            }


        };

        // Parses list elements.
        bool Elem() {
            //cout << pos << "Elem --\n";

            if (match_word("null"))
                return true;
            if (V())
                return true;
            if (J())
                return true;
            return false;
        };

        // The null case that I highlighted will be resolved with this RHD method.
        bool RHS() {
            //cout << pos << "RHS --\n";

            if (J())
                return true;
            if (V())
                return true;
            if (L())
                return true;
            if (match_word("null"))
                return true;

            return false;
        };


};


int main()
{


    std::ifstream file("D:/test.json");
    if (!file.is_open()) return 1;

    // Read everything directly into the 'fileContent' variable
    std::string fileContent((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    JsonParser j(
        fileContent

    );

    cout << "Parsing the below JSON! - \n" << fileContent << endl;
    if (j.parse())
        cout << "------------------ VALID JSON -------------------";
    else
        cout << "----------------- INVALID JSON ------------------";
    
    return 0;
}


