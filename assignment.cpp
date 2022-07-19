#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

class textMatcher{
    bool isProcessed = false;
    int bad_c_table[256];
    int p_length;
    int t_length;
    string pattern;
    string text;
    int number_of_matches;
    vector<int> matched_lines;
    
    void create_bad_character(){ 
        int i;
        for (i = 0; i < 256; i++)
            bad_c_table[i] = p_length;

        for (i =0; i<p_length - 1; i++){
            if(isupper(pattern[i]))
                bad_c_table[int(pattern[i])+32] = p_length - i - 1;
            else
                bad_c_table[int(pattern[i])] = p_length - i - 1;
        }
            
    }

    public:
        
        bool boyer_moore(){
            int shift_value = 0;
            int j;
            while (shift_value <= (t_length - p_length))
            {   
                for (j = p_length - 1; j >= 0; j--){
                    if(isupper(pattern[j]) && !isupper(text[shift_value + j])){
                        if (int(pattern[j]) +32 != int(text[shift_value + j]))
                            break;        
                    }

                    else if(!isupper(pattern[j]) && isupper(text[shift_value + j])){
                        if (int(pattern[j]) != int(text[shift_value + j])+32)
                            break;
                    }
                    else{
                        if (pattern[j] != text[shift_value + j])
                        break;
                    }
                }
                if (j < 0){
                    return true;
                    if (shift_value + p_length < t_length){
                        if(isupper(text[j + p_length + 1]))
                            shift_value += bad_c_table[int(text[j + p_length + 1])+32];
                        else
                            shift_value += bad_c_table[int(text[shift_value+p_length])];
                
                    }
                    else
                        shift_value++;
                }
                else{
                    if(isupper(text[shift_value + (p_length - 1)]))
                        shift_value += bad_c_table[int(text[shift_value + (p_length - 1)])+32];
                    else
                        shift_value += bad_c_table[int(text[shift_value + (p_length - 1)])];
                }
            }
            return false;
        }   

        textMatcher(string p){
            number_of_matches = 0;
            pattern = p;
            p_length = p.length();
            create_bad_character();
        }

        bool searchLine(string t){
            text = t;
            t_length = t.length();
            return boyer_moore();
        }

        int getNumOfmatches(){
            return number_of_matches;
        }

        void setNumOfMatches(int num){
            number_of_matches = num;
        }

        void setMatchedLines(int ln){
            matched_lines.push_back(ln);
        }
};


class textFile {
    vector<string> lines;
    int line_count; // stores line count.
    string line; // stores a specific line temporarily.
    int number_of_results;

    public:
        textFile(string file_to_open = ""){
            fstream file;
            line_count=0;
            file.open(file_to_open, ios::in | ios::out);
            if (!file.is_open())
                    cout<<"Could not open the file"<<endl;
            else {
                //slicing the file line by line and stores it in a vector
                while (getline(file,line)){
                lines.push_back(line);
                line_count++;
                }
            }
            file.close();
            
        }
        //prints the line when the line number is given as a parameter
        void print_line(int line_number){
            cout<< lines[line_number-1] <<endl;
        }



        //slice the file after a specific line number and name it(returns the opened file)
        fstream file_slice(string file_name,int line_number){
            vector<string>:: iterator line_index;
            fstream newFile;
            newFile.open(file_name, ios::out);
            if(!newFile)
                return newFile;

            for(line_index=lines.begin()+(line_number-1);line_index<lines.end();line_index++)
                newFile<<*line_index<<endl;
            return newFile;
        }

        //seach for matched lines

        textMatcher searchLines(string pattern){
            int line_number;
            textMatcher matcher1(pattern);
            number_of_results = 0;
            for(line_number=0;line_number<line_count;line_number++){
                if(matcher1.searchLine(lines[line_number])){
                    matcher1.setMatchedLines(line_number+1);
                    number_of_results++;
                }
                
            }
            matcher1.setNumOfMatches(number_of_results);
            return matcher1;
        }
};



int main(){
    
    textFile t1("modules.txt");
    t1.print_line(3);
    fstream f1 = t1.file_slice("module_slice_10",10);
    f1<<"haha";
    f1.close();


    cout<<"search results:"<<endl;
    textMatcher res1 = t1.searchLines("he entry");
    cout<<res1.getNumOfmatches();
}
