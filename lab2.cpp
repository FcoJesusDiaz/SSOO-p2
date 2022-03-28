#include <iostream>
#include <thread>
#include <stdlib.h>
#include <queue>
#include <string>

class Searcher{
    private:
        int id;
        int begin;
        int end;
        std::queue<result> results;
    
    public:
        Searcher(int id, int begin, int end){
            this->id = id;
            this->begin = begin;
            this->end = end;
        }
        
        void operator()(){

        }
};

struct result{
    std::string previous;
    std::string next;
};

int main(int argc, char **argv){
    return 0;
}