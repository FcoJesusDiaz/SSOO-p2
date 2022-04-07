#include <iostream>
#include <thread>
#include <stdlib.h>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <functional>
#include <fstream>
#include <condition_variable>
#include <bits/stdc++.h>

int CountLines(std::string filename);
bool is_integer(char *str);

std::vector<int> numLines;

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
        std::queue<Result> results;
    
    public:
        Searcher(int id, int begin, int end): id(id), begin(begin), end(end){}
        
        void operator()(const std::string& input){

            std::cout<< "Hilo: " << id << ", l_inicio: " << begin+1 << " l_final: " << end+1 << std::endl;
            Searching();
            std::cout<< "Resultados del hilo: "<< id << std::endl;
            
            while (!results.empty())
            {
                Result resultado = results.back();
                results.pop();
                std::cout << "HILO:" << id <<"Coincidencia encontrada en la linea: " << resultado.line +1<< std::endl;
            }
        }

        void Searching(){
            std::string line;
            int lines = begin;
            std::string word = "David";
            std::ifstream mFile("Libros/prueba.txt");
            if(!mFile.is_open()) {
                std::cerr << "No se pudo abrir el archivo\n";
                return;
            }
            
            mFile.seekg(numLines[begin]);
            while(mFile.peek()!=EOF && lines<=end){
                getline(mFile, line);
                
                if (line.find(word)!=-1)
                {
                    struct Result instancia;
                    instancia.line=lines;
                    results.push(instancia);
                    //std::cout << "hilo " << id << "encuentra en la linea" << lines << std::endl;
                }
                //std::cout << line << std::endl;
                
                std::vector<std::string> tokens= tokenize(line);
                lines++;
        
            }
            mFile.close();
        }

        std::vector<std::string> Searcher::tokenize(std::string line);
};

std::vector<std::string> Searcher::tokenize(std::string line){
    std::vector<std::string> tokens;
    std::stringstream check1(line);
    std::string intermediate;
    while (getline(check1, intermediate, ' ')){
        tokens.push_back(intermediate);
    }
    
    return tokens;
}

int main(int argc, char **argv){
    
    int num_lines;
    int num_threads;
    int task_size;
    
    // Beggining and end line of each thread
    int l_begin;
    int l_end;
    
    //Vectors used for storing thread and Searcher objects
    std::vector<std::thread> v_hilos;
    std::vector<Searcher> v_objetos;

    if (argc != 4){
        std::cerr << "Usage: <SSOIIGLE> <file> <word> <number of threads>" << std::endl;
    }

    //Count amount of lines in the file. Store in numLines the bytes corresponding to the beggining of each line
    if(!(num_lines = CountLines(argv[1]))){
        std::cout << "El fichero tiene "<< num_lines <<" lineas"<< std::endl;
        return EXIT_FAILURE;
    }

    if(!is_integer(argv[3])){
        std::cerr << "The number of threads input must be a positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    num_threads = atoi(argv[3]);

    //Lines per thread
    task_size = num_lines/num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        l_begin = i * task_size;
        l_end = (l_begin + task_size) - 1;

        Searcher s{i + 1, l_begin, l_end};
        v_hilos.push_back(std::thread(s, argv[1]));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
    
    return EXIT_SUCCESS;
}

bool is_integer(char *str){
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

int CountLines(std::string filename){
    int lines;
    std::string line;
    std::ifstream mFile(filename);

    if(!mFile.is_open()){ 
        std::cerr << "Could not open file:" << filename << std::endl;
        return 0;
    }
    //mFile.seekg(0);
    while(mFile.peek()!=EOF){
        //Store bytes corresponding to each line in numLines
        numLines.push_back(mFile.tellg()); 
        //std::cout << mFile.tellg() << std::endl;
        getline(mFile, line);
        lines++;
    }

    mFile.close();
    return lines;
}