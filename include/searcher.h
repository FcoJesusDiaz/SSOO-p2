#ifndef SEARCHER_H
#define SEARCHER_H

#include "thread_searcher.h"
class Searcher{
    private:
        int id;
    public:
        Searcher(int id): id(id){};
        void operator()();
        int CountLines(std::string filename);
        void printResults(std::string word, std::vector<thread_searcher> v_objetos);
        void get_filenames();
};

#endif
