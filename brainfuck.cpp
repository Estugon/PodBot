#include <stdio.h>
#include <string.h>
#include <iostream>
#include <regex>
#include <vector>


using namespace std;


class Brainfuck
{
    
    public:
        static vector<int> cells;
        static int ptr;
        static string interpret(string input) {
            regex br("[^<>+\-.\,\\[\\]]");
            input = regex_replace(input, br, "");
            for (int i = 0; i < input.length(); i++) {
                cells.push_back(0);
            }

            Brainfuck::ptr = 0;

            string out = "";

            for (int i = 0; input[i] != '\0'; i++) {

                switch(input[i]) {
                case '+':
				    Brainfuck::cells[Brainfuck::ptr]++;
				    break;
			    case '-':
				    Brainfuck::cells[Brainfuck::ptr]--;
    				break;
		    	case '<':
			    	Brainfuck::ptr--;
    				break;
	    		case '>':
		    		Brainfuck::ptr++;
			    	break;
    			case '.':
	    			out += (char) Brainfuck::cells[Brainfuck::ptr];
		    		break;
    			case ',':
	    			return "Currently not supported! Working on it!";
                case '[':
                    {
                        int eoL = getEndOfLoop(input, i);
                        if (eoL == -1) {
                            return string("Error! Loop at Position `%d` is never closed!", i);
                        }
                        out += Brainfuck::loop(input, eoL, i);
                        i = eoL;
                        break;
                    }
                }
            }
            Brainfuck::cells.clear();
            Brainfuck::ptr = 0;
            return out;
        }
    private:
        
        static string loop(string index, const int loopStop, const int loopStart) {
            string out = "";
            int i = loopStart +1;

            while (true) {
                if (i == loopStop && cells[ptr] == 0) {
                    return out;
                }
                switch(index[i]) {
                case '+':
                    Brainfuck::cells[Brainfuck::ptr]++;
                    break;
                case '-':
                    Brainfuck::cells[Brainfuck::ptr]--;
                    break;
                case '<':
                    Brainfuck::ptr--;
                    break;
                case '>':
                    Brainfuck::ptr++;
                    break;
                case '.':
                    out += (char) Brainfuck::cells[Brainfuck::ptr];
                    break;
                case ',':
                    return "`,` operator currently not supported! Working on it!";
                case '[':
                    {
                        int eoL = getEndOfLoop(index, i);
                        if (eoL == -1) {
                            return string("Error! Loop at Position `%d` is never closed!", i);
                        }
                        out += loop(index, eoL, i);
                        i = eoL;
                        break;
                    }
                case ']':
                    if (i == loopStop) {
                        if (cells[ptr] == 0) {
                            return out;
                        } else {
                            i = loopStart;
                        }
                    }
                    break;
                }
                i++;
            }
            return out;
        }
        static int getEndOfLoop(string code, int index) {
            int i = index + 1;
            int depth = 0;
            while (true) {
                if (code[i] == '[') {
                    depth++;
                } else if (code[i] == ']') {
                    if (depth == 0) {
                        return i;
                    } else {
                        depth--;
                    }
                } else if (code[i] == '\0') {
                    return -1;
                }
                i++;
            }
        }
};

vector<int> Brainfuck::cells;
int::Brainfuck::ptr = 0;
