#include <iostream>
#include <thread>
#include <stdlib.h>
#include <queue>
#include <string>

class Searcher{
    private:
        int id;
        int begin;
        int subdivision;
        std::queue<result> results;
    
    public:
        Searcher(int id, int subdivision, std::fstream file){
            this->id = id;
            this->subdivision = subdivision;
            this->file = file;
        }
        
        void operator()(){

        }
};

struct result{
    std::string previous;
    std::string next;
};

int main(int argc, char **argv){
    std::ifstream file(FILE_NAME);
    if(!file.is_open()){
        std::cout << "Error opening file" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int n_lines;
    int subdivision;
    std::string aux;


    while (std::getline(file, aux))
        n_lines++;

    subdivision = n_lines/THREADS;

    std::vector<int> thread_read_beginning;
    for(int i = 0; i < THREADS; i++)
        thread_read_beginning.push_back(i*subdivision);
    return 0;
}
