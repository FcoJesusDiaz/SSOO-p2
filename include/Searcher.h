#include <iostream>
#include <queue>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <mutex>
#include "colors.h"

#ifndef SEARCHER_H
#define SEARCHER_H

extern std::vector<int> numLines;
extern std::mutex m;
extern std::mutex vectorLock;
extern int flag;

struct Result{
    std::string previous;
    std::string next;
    std::string word;
    int id;
    int l_begin;
    int l_end;
    int line;
};

extern std::vector<Result> Totalsearches;

class Searcher{
    private:
        int id;
        int begin;
        int end;
        std::string filename; //archivo en el que se buscará la palabra
        std::string word; //palabra que se debe buscar
        std::vector<Result> results;
    public:
        Searcher(int id, int begin, int end, std::string filename, std::string word): id(id), 
        begin(begin), end(end),filename(filename),word(word){};

        void operator()(){
            searching();
            saveResults();
        }
        void searching();
        void findWord(std::string line, int numLine);
        void saveResults();
        bool checkWord(std::string checked);
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
            //std::cout<< tokens[i-1] << " - " << tokens[i] << " - " << tokens[i+1] << std::endl;
            Result coincidencia;
            coincidencia.id=id;
            coincidencia.line=numLine;
            coincidencia.next=tokens[i+1];
            coincidencia.previous=tokens[i-1];
            coincidencia.word=tokens[i];
            coincidencia.l_begin=begin;
            coincidencia.l_end=end;
            results.push_back(coincidencia);
        }
        if (word==tokens[i] && i==0)
        {
            Result coincidencia;
            coincidencia.id=id;
            coincidencia.line=numLine;
            coincidencia.next=tokens[i+1];
            coincidencia.previous="";
            coincidencia.word=tokens[i];
            coincidencia.l_begin=begin;
            coincidencia.l_end=end;
            results.push_back(coincidencia);
        }
        if (word==tokens[i] && i==tokens.size()-1)
        {
            Result coincidencia;
            coincidencia.id=id;
            coincidencia.line=numLine;
            coincidencia.next="";
            coincidencia.previous=tokens[i-1];
            coincidencia.word=tokens[i];
            coincidencia.l_begin=begin;
            coincidencia.l_end=end;
            results.push_back(coincidencia);
        }
        
    }
}

void Searcher::saveResults(){
    /*
    for (int  i = 0; i < results.size(); i++)
    {
        Result resultado = results[i];
        std::cout<< "[Hilo "<< id << " inicio: " << begin+1 << " - final: "<< end+1 << "] línea " << resultado.line<<
        " :: ... "<< resultado.previous << " "<<resultado.word << " "<< resultado.next << std::endl;
    }
    */

    std::unique_lock<std::mutex> lk(m);
    while (!flag)
    {
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
    std::unique_lock<std::mutex> vk(vectorLock);
    for (unsigned i = 0; i < results.size(); i++)
    {
        Totalsearches.push_back(results[i]);
    }
    vk.unlock();
    flag=false;
   
    
}

bool Searcher::checkWord(std::string checked){

    const char *w = word.c_str();
    const char *ck = checked.c_str();

    if (strstr(w,ck)) return true;

    return false;
}

#endif