#include <iostream>
#include <queue>
#include <fstream>
#include <bits/stdc++.h>
#include "colors.h"

#ifndef SEARCHER_H
#define SEARCHER_H

extern std::vector<int> numLines;

struct Result{
    std::string previous;
    std::string next;
    std::string word;
    int line;
};

class Searcher{
    private:
        int id;
        int begin;
        int end;
        std::string filename; //archivo en el que se buscará la palabra
        std::string word; //palabra que se debe buscar
        std::queue<Result> results;
    public:
        Searcher(int id, int begin, int end, std::string filename, std::string word): id(id), 
        begin(begin), end(end),filename(filename),word(word){};

        void operator()(){
            searching();
            printResults();
        }
        void searching();
        void findWord(std::string line, int numLine);
        void printResults();
};

void Searcher::searching(){
    std::string line;
    int lines=begin;
    std::ifstream mFile(filename);
    if(mFile.is_open()) {
        mFile.seekg(numLines[begin]);
		while(mFile.peek()!=EOF && lines<=end)
		{
            getline(mFile, line);
            lines++;
            findWord(line,lines);
		}
		mFile.close();
    }
    else
        std::cerr << RED << "Thread " << id << "could not open the file " << filename << RESET <<std::endl;
}

void Searcher::findWord(std::string line, int numLine){
    std::vector<std::string> tokens;//array en el que guardaremos la linea
    std::stringstream check1(line);
    std::string intermediate;
    while (getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    for (unsigned i = 0; i < tokens.size(); i++)
    {
        std::transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), ::tolower);
        if (word==tokens[i] && i!=0 && i!=tokens.size()-1) //la palabra coincide y no es la primera ni la ultima de la linea
        {
            std::cout<< tokens[i-1] << " - " << tokens[i] << " - " << tokens[i+1] << std::endl;
            Result coincidencia;
            coincidencia.line=numLine;
            coincidencia.next=tokens[i+1];
            coincidencia.previous=tokens[i-1];
            coincidencia.word=tokens[i];
            results.push(coincidencia);
        }
        
    }
}

void Searcher::printResults(){
    
    while (!results.empty())
    {
        Result resultado = results.back();
        results.pop();
        std::cout<< "[Hilo "<< id << " inicio: " << begin << " - final: "<< end << "] línea " << resultado.line<<
        " :: ... "<< resultado.previous << " "<<resultado.word << " "<< resultado.next << std::endl; 
    }
    
    
}

#endif