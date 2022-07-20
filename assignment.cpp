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
    vector<string> &textLines;
    
    void create_bad_character();

    public:
        
        bool boyer_moore();

        textMatcher(string p, vector<string> &lines);

        bool searchMatchingLine(string t);

        int  getNumOfmatches();

        void setMatchedLines(int ln){
            matched_lines.push_back(ln);
        }

        //splits a line of strings into words and returns a vector containing them
        vector<string> split(string str);

        void printStringVector(vector<string> vec, int startingPos, int endPos);

        void printStringVector(vector<string> vec, int startingPos);

        void printMatchedLines(int filter);
};


class textFile {
    vector<string> lines;
    int line_count; // stores line count.
    string line; // stores a specific line temporarily.
    int number_of_results;

    public:
        textFile(string file_to_open);
        //prints the line when the line number is given as a parameter


        void print_line(int line_number);



        //slice the file after a specific line number and name it(returns the opened file)
        fstream file_slice(string file_name,int line_number);

        //seach for matched lines

        textMatcher searchLines(string pattern);

        //splits a line of strings into words and print them in a line
        void split(int line_number);


};



int main(){
    
    textFile t1("modules.txt");
    int command = 1;
    int subCommand = 0;
    string searchPattern;
    string filter;
    
    while(command!=5){
        cout<<"Select an option from the menu below."<<endl;
        cout<<"1. Find lines containing a string."<<endl;
        cout<<"2. Display a specific line."<<endl;
        cout<<"3. Slice the file from a given line number and save the result in a new file."<<endl;
        cout<<"4. Split a line into words and print them."<<endl;  
        cout<<"5. Exit."<<endl;
        cout<<"Your choice: ";
        cin>>command;
        switch (command) {
        case 1:{
            cout<<endl<<endl;
            cout<<"Enter the string you want to find: ";
            cin>>searchPattern;
            cout<<"Select a filter option from below"<<endl;
            cout<<"1. no filter"<<endl;
            cout<<"2. Module code only"<<endl;
            cout<<"3. Module name only"<<endl;
            cout<<"Your choice: ";
            cin>>subCommand;
            textMatcher res1 = t1.searchLines(searchPattern);
            res1.printMatchedLines(subCommand);
            cout<<res1.getNumOfmatches()<<" matches found"<<endl;
            break;
        }      


        case 2:{
            cout<<"Enter the line number you want to print: ";
            cin>>subCommand;
            cout<<"Line "<<subCommand<<": ";
            t1.print_line(subCommand);
            break;
        }

        case 3:{
            cout<<"Enter a line number to divide the file: ";
            cin>>subCommand;
            string fileName;
            cout<<"Enter a name for the new file: ";
            cin>> fileName;
            t1.file_slice(fileName,subCommand);
            break;
        }

        case 4: {
            cout<<"Enter the line number you want to split into words and print: ";
            cin>>subCommand;
            t1.split(subCommand);
            break;
        }
        

        }  
        cout<<endl<<endl<<endl;  
    }

}







/******** textMatcher class methods ********/
void textMatcher:: create_bad_character(){ 
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

bool textMatcher:: boyer_moore(){
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
                    number_of_matches++;
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


textMatcher:: textMatcher(string p, vector<string> &lines):textLines(lines) {
            number_of_matches = 0;
            pattern = p;
            p_length = p.length();
            create_bad_character();
}

bool textMatcher:: searchMatchingLine(string t){
            text = t;
            t_length = t.length();
            return boyer_moore();
}

int textMatcher:: getNumOfmatches(){
            return number_of_matches;
}

vector<string> textMatcher:: split(string str){
            vector<string> words;
            string res = "";;
            if(str == "")
                return words;
            for (char i: str){
        
                if (i==' '){
                    words.push_back(res);
                res = "";
                }
                else
                    res += i;
            }
            words.push_back(res);
            vector<string>::iterator i;
}

void textMatcher:: printStringVector(vector<string> vec, int startingPos, int endPos){
            vector<string>::iterator i;
            for (i=vec.begin()+startingPos;i<vec.begin()+ endPos+1;i++)
                cout<<*i<<" ";
            cout<<endl;
}

void textMatcher:: printStringVector(vector<string> vec, int startingPos){
            vector<string>::iterator i;
            for (i=vec.begin()+startingPos;i<vec.end();i++)
                    cout<<*i<<" ";
            cout<<endl;
}


void textMatcher:: printMatchedLines(int filter){
            if (filter==1){
                vector<string>:: iterator i=textLines.begin();
                vector<int>:: iterator j;
                for(j= matched_lines.begin();j<matched_lines.end();j++){
                    cout<<*(i+*j-1)<<endl;
                }
            }
            else if (filter == 2){
                vector< vector<string> > results;
                vector<string>:: iterator i=textLines.begin();
                vector<int>:: iterator j;
                for(j= matched_lines.begin();j<matched_lines.end();j++){
                    results.push_back(split(*(i+*j-1)));
                }
               
                vector< vector<string> > :: iterator k;
                for(k=results.begin();k<results.end();k++)
                    printStringVector(*k,0,0);
        
                
            }

            else if (filter == 3){
                vector< vector<string> > results;
                vector<string>:: iterator i=textLines.begin();
                vector<int>:: iterator j;
                for(j= matched_lines.begin();j<matched_lines.end();j++){
                    results.push_back(split(*(i+*j-1)));
                }
               
                vector< vector<string> > :: iterator k;
                for(k=results.begin();k<results.end();k++)
                    printStringVector(*k,1);
        
                
            }
}

/******** textFile class methods ********/
textFile:: textFile(string file_to_open = ""){
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

void textFile:: print_line(int line_number){
            cout<< lines[line_number-1] <<endl;
}

fstream textFile:: file_slice(string file_name,int line_number){
            if(line_number<1 | line_number>line_count){
                cout<<"ERROR: Line number out of range. Duplicating the original file."<<endl;
                vector<string>:: iterator line_index;
                fstream newFile;
                newFile.open(file_name, ios::out);
                if(!newFile)
                    return newFile;

                for(line_index=lines.begin();line_index<lines.end();line_index++)
                    newFile<<*line_index<<endl;
                return newFile;
            }
            vector<string>:: iterator line_index;
            fstream newFile;
            newFile.open(file_name, ios::out);
            if(!newFile)
                return newFile;

            for(line_index=lines.begin()+(line_number-1);line_index<lines.end();line_index++)
                newFile<<*line_index<<endl;
            return newFile;
}

textMatcher textFile:: searchLines(string pattern){
            int line_number;
            textMatcher matcher1(pattern, lines);
            number_of_results = 0;
            for(line_number=0;line_number<line_count;line_number++){
                if(matcher1.searchMatchingLine(lines[line_number])){
                    matcher1.setMatchedLines(line_number+1);
                    number_of_results++;
                }
                
            }
            return matcher1;
}

void textFile:: split(int line_number){
            if(line_number<1 | line_number>line_count){
                cout<<"ERROR: line number out of range"<<endl;
                return;
            }
            string str = lines[line_number-1];
            string res = "";;
            if(str == "")
                cout<<res;
            for (char i: str){
        
                if (i==' '){
                    cout<<res<<", ";
                res = "";
                }
                else
                    res += i;
            }
            cout<<res<<endl;
}