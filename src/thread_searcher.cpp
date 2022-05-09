#include <iostream>
#include <queue>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <mutex>
#include <condition_variable>
#include "colors.h"

#include "thread_searcher.h"

/*Variable from SSOOIIGLE*/
extern std::vector<int> numLines;

/*DEFINITION OF METHODS INSIDE SEARCHER*/

/* method used to read the file from the byte indicated in the variable "begin". In addition, in each read line 
we will call the "findword" method to check if the searched word is in this line */
void thread_searcher::searching(){
    std::string line;
    int lines;
    std::ifstream mFile(filename);
    if(!mFile.is_open()) {
        std::cerr << this->colour << "Thread " << id << " could not open the file " << filename << RESET <<std::endl;
        return;
    }
    mFile.seekg(0);
    while(mFile.peek() != EOF )
    {
        getline(mFile, line);
        lines++;
        findWord(line,lines);
    }
    mFile.close();
}   

/* method to check if the searched word is in the current line. for this we will create an array whose 
positions will have the words that form that line. once the array is created; We will go through it and for 
each word we will call the checkword method that will tell us if that word has the substring we are looking 
for. In the true case, we will create a Result structure with the necessary data and include it in the 
thread's private result vector. */
void thread_searcher::findWord(std::string line, int numLine){
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    std::vector<std::string> tokens;
    std::stringstream check1(line);
    std::string intermediate;
    while (getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    for (unsigned i = 0; i < tokens.size(); i++)
    {
        std::string originalWord = tokens[i];
        std::transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), ::tolower);
        bool found = checkWord(tokens[i]);

        if (found)
        {
            Result coincidencia;
            coincidencia.id = id;
            coincidencia.line = numLine;
            coincidencia.word = originalWord;
            coincidencia.previous = (i != 0) ? tokens[i-1] : "";
            coincidencia.next = (i != tokens.size()-1) ? tokens[i+1] : "";
            results.push_back(coincidencia);
        }
    }
}

//method to check if the substring we want is contained in a higher string
bool thread_searcher::checkWord(std::string checked){

    /* we convert from string to const char to be able to use the strstr function and check if a substring 
    is contained in a larger string*/
    const char *w = word.c_str();
    const char *ck = checked.c_str();

    if (strstr(ck,w)) return true;

    return false;
}

void thread_searcher :: operator()(){
    searching();
}

std::string thread_searcher::to_string(){
    std::string result;
    std::cout << "file: " << filename << " resultados: "<< results.size()<< std::endl;
    
    for (unsigned i = 0; i < results.size(); i++)
    {
        result += this->colour + "[Hilo " + std::to_string(results[i].id)+ " Libro: "+ filename
        + "] línea " + std::to_string(results[i].line) + " :: ... " + results[i].previous 
        + " " + results[i].word + " " + results[i].next + RESET + "\n";
    }
    return result;
}
