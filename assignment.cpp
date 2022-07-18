#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

class searchResults{
    vector<int> matched_lines;
    int bad_c_table[256];
    int p_length;
    int t_length;
    string pattern;
    string text;
    
    void create_bad_character(){ // rmo:- right most occurrences
        int i;
        for (i = 0; i < 256; i++)
            bad_c_table[i] = p_length;

        for (i = p_length - 2; i >= 0; i--)
            bad_c_table[int(pattern[i])] = p_length - i - 1;
    }

    public:
        
        int boyer_moore(){
            int shift_value = 0;
            int j;
            while (shift_value <= (t_length - p_length))
            {
                for (j = p_length - 1; j >= 0; j--){
                    if (pattern[j] != text[shift_value + j])
                        break;
                }
                if (j < 0){
                    cout << "Found at " << shift_value + 1 << endl;
                    if (shift_value + p_length < t_length){
                        shift_value += bad_c_table[int(text[j + p_length + 1])];
                    }
                    else
                        shift_value++;
                }
                else{
                    shift_value += bad_c_table[int(text[shift_value + (p_length - 1)])];
                }
            }
}
        /***********/
        searchResults(string p, string t){
            pattern = p;
            text = t;
            p_length = p.length();
            t_length = t.length();
            create_bad_character();
            boyer_moore();
        }
};


class textFile {
    vector<string> lines;
    int line_count; // stores line count.
    string line; // stores a specific line temporarily.


    public:
        textFile(string file_to_open = "", string string_to_search = ""){
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

        

        vector<int> & search(){
            vector<int> mathed_lines;

        }
};



int main(){
    
    textFile t1("modules.txt");
    t1.print_line(10);
    fstream f1 = t1.file_slice("module_slice_10",10);
    f1<<"haha";
    f1.close();

    searchResults s1("uthsara", "f1255 senith uthsara karunarathne");
    
}
