#include <iostream>
#include <queue>
#include <fstream>
#include <bits/stdc++.h>

extern std::vector<int> numLines;

struct Result{
    std::string previous;
    std::string next;
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

        }
    void searching();
    void Searcher::findWord(std::string line, int num_line);
    void Searcher::printResults();
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
        std::cout<<"No se pudo abrir el archivo\n";
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
            coincidencia.line=numLine;
            coincidencia.next=tokens[i+1];
            coincidencia.previous=tokens[i-1];
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
        " :: ... "<< resultado.previous << " "<<word << " "<< resultado.next << std::endl; 
    }
    
    
}