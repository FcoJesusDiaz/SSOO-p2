/* 
    code belonging to practice 2 of SSOOII. In this code, a "RESULT" structure is implemented that we will use
    to format the results obtained in the search. And the Searcher class whose methods we will use to perform 
    the search and store all the data of each word found. Each instance of this class will be assigned a thread 
    to be able to perform the search in parallel between several threads.
    Code made by:
            - MIGUEL DE LAS HERAS FUENTES
            - FRANCISCO JESÚS DÍAZ PELLEJERO
            - JAVIER VILLAR ASENSIO
*/

#ifndef SEARCHER_H
#define SEARCHER_H

//structure that we will use for the results
struct Result{
    std::string previous;
    std::string next;
    std::string word;
    int id;
    int l_begin;
    int l_end;
    int line;
};

/* search engine class that we will assign to each thread to perform the search. includes all methods for 
searching and storing results */
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

        void searching();
        void findWord(std::string line, int numLine);
        bool checkWord(std::string checked);
        void printResults();
        void operator()();
};
#endif