#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ios>
#include <limits>
#include <stdlib.h>


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
    fstream results_file;
    
    void create_bad_character();

    public:
        
        bool boyer_moore(); //boyer moore horsepool algorithm

        textMatcher(string p, vector<string> &lines);
                                           
                                           // drives the boyer_moore() function
        bool searchMatchingLine(string t);//check whether the given line contains the pattern or not.
                                        

        int  getNumOfmatches();//returns number of matching lines

        void setMatchedLines(int ln);
        vector<string> split(string str);//splits a line of strings into words and returns a vector containing them

        //to print
        void printStringVector(vector<string> vec, int startingPos, int endPos);//prints a string vector when from a starting point 
                                                                                //to a ending point
        void printStringVector(vector<string> vec, int startingPos);//prints a string vector when from a starting point
                                                                    //to the end of the vector.
        void printMatchedLines(int filter);//matched lines and filter out resultts.

        //to save
        void saveStringVector(vector<string> vec, int startingPos, int endPos);
        void saveStringVector(vector<string> vec, int startingPos);
        void saveMatchedLines(string fileName,int filter);
};


class textFile {
    vector<string> lines;//stores each line of the file 
    int line_count; // stores line count.
    string line; // stores a specific line temporarily.


    public:
        int getLineCount();
        textFile(string file_to_open);
        void print_line(int line_number);//prints the line when the line number is given as a parameter
        void file_slice(string file_name,int line_number);//slice the file after a specific line number and name it(returns the opened file)
        textMatcher searchLines(string pattern);//seach for matched lines
        void split(int line_number); //splits a line of strings into words and print them in a line
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
        system("cls");
        if(1>command || 5<command)
            cout<<"ERROR: "<<command<<" is not a valid input"<<endl;
        switch (command) {
        case 1:{
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            cout<<"Enter the string you want to find: ";
            getline(cin,searchPattern);
            cout<<"Select a filter option from below"<<endl;
            cout<<"1. no filter"<<endl;
            cout<<"2. Module code only"<<endl;
            cout<<"3. Module name only"<<endl;
            cout<<"Your choice: ";
            cin>>subCommand;
            system("cls");
            textMatcher res1 = t1.searchLines(searchPattern);
            res1.printMatchedLines(subCommand);
            cout<<res1.getNumOfmatches()<<" matches found"<<endl;
            char c = 'x';
            while(c!='y' && c!='n' && c!='Y' && c!= 'N'){
                cout<<"Do you want to save results on a file? (y/n): ";
                cin>>c;
                if(c=='y' || c=='Y'){
                    cout<<"Enter a name for the file: ";
                    string name;
                    cin>>name;
                    res1.saveMatchedLines(name,subCommand);
                    cout<<"File saved. ";
                    system("pause");
                }
            }
            
            break;
        }      


        case 2:{
            cout<<"Enter the line number you want to print: ";
            cin>>subCommand;
            if(1>subCommand | subCommand> t1.getLineCount()){
                cout<<"ERROR: Line number out of range. ";
                system("pause");
                break;
            }
            cout<<"Line "<<subCommand<<": ";
            t1.print_line(subCommand);
            system("pause");
            system("cls");
            break;
        }

        case 3:{
            cout<<"Enter a line number to divide the file: ";
            cin>>subCommand;
            if(1>subCommand | subCommand> t1.getLineCount()){
                cout<<"ERROR: Line number out of range. ";
                system("pause");
                break;
            }
            string fileName;
            cout<<"Enter a name for the new file: ";
            cin>> fileName;
            t1.file_slice(fileName,subCommand);
            cout<<"File created. ";
            system("pause");
            break;
        }

        case 4: {
            cout<<"Enter the line number you want to split into words and print: ";
            cin>>subCommand;
            if(1>subCommand | subCommand> t1.getLineCount()){
                cout<<"ERROR: Line number out of range. ";
                system("pause");
                break;
            }
            t1.split(subCommand);
            system("pause");
            break;
        }
        }
        system("cls");   
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
                    //only the pattern character is upper case
                    if(isupper(pattern[j]) && !isupper(text[shift_value + j])){
                        if (int(pattern[j]) +32 != int(text[shift_value + j]))
                            break;        
                    }
                    //only the text character is uppercase
                    else if(!isupper(pattern[j]) && isupper(text[shift_value + j])){
                        if (int(pattern[j]) != int(text[shift_value + j])+32)
                            break;
                    }
                    else{
                        if (pattern[j] != text[shift_value + j])
                        break;
                    }
                }
                //if j<0, a match found
                if (j < 0){
                    number_of_matches++;
                    return true;
                    //if the total shift value is smaller than text length(to prevent text overflow)
                    if (shift_value + p_length < t_length){
                        //considering character is uppercase
                        if(isupper(text[j + p_length + 1]))
                            shift_value += bad_c_table[int(text[j + p_length + 1])+32];
                        //considering character is lowercase
                        else
                            shift_value += bad_c_table[int(text[shift_value+p_length])];
                
                    }
                    else
                        shift_value++;
                }
                //match not found
                else{
                    //if the last character of the text window is uppercase
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
            return words;
}
//print
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
            else {
                vector< vector<string> > results;
                vector<string>:: iterator i=textLines.begin();
                vector<int>:: iterator j;
                for(j= matched_lines.begin();j<matched_lines.end();j++){
                    results.push_back(split(*(i+*j-1)));
                }
               
                vector< vector<string> > :: iterator k;
                for(k=results.begin();k<results.end();k++)
                    if (filter == 2)
                        printStringVector(*k,0,0);
                    else if(filter==3)
                        printStringVector(*k,1);
                
            }

        
}


//save

void textMatcher:: saveStringVector(vector<string> vec, int startingPos, int endPos){
            vector<string>::iterator i;
            for (i=vec.begin()+startingPos;i<vec.begin()+ endPos+1;i++)
                results_file<<*i<<" ";
            results_file<<endl;
}

void textMatcher:: saveStringVector(vector<string> vec, int startingPos){
            vector<string>::iterator i;
            for (i=vec.begin()+startingPos;i<vec.end();i++)
                    results_file<<*i<<" ";
            results_file<<endl;
}


void textMatcher:: saveMatchedLines(string fileName,int filter){
            results_file.open(fileName, ios::out);
            if (filter==1){
                vector<string>:: iterator i=textLines.begin();
                vector<int>:: iterator j;
                for(j= matched_lines.begin();j<matched_lines.end();j++){
                    results_file<<*(i+*j-1)<<endl;
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
                    saveStringVector(*k,0,0);
        
                
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
                    saveStringVector(*k,1);
        
                
            }
            results_file.close();
}



void  textMatcher:: setMatchedLines(int ln){
            matched_lines.push_back(ln);
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

void textFile:: file_slice(string file_name,int line_number){
            if(line_number<1 | line_number>line_count){
                cout<<"ERROR: Line number out of range. Duplicating the original file."<<endl;
                vector<string>:: iterator line_index;
                fstream newFile;
                newFile.open(file_name, ios::out);
                if(!newFile)
                    return;

                for(line_index=lines.begin();line_index<lines.end();line_index++)
                    newFile<<*line_index<<endl;
                return;
            }
            vector<string>:: iterator line_index;
            fstream newFile;
            newFile.open(file_name, ios::out);
            if(!newFile)
                return;

            for(line_index=lines.begin()+(line_number-1);line_index<lines.end();line_index++)
                newFile<<*line_index<<endl;
            newFile.close();
            return;
}

textMatcher textFile:: searchLines(string pattern){
            int line_number;
            textMatcher matcher1(pattern, lines);
            for(line_number=0;line_number<line_count;line_number++){
                if(matcher1.searchMatchingLine(lines[line_number])){
                    matcher1.setMatchedLines(line_number+1);
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

int textFile::getLineCount(){
    return line_count;
}
