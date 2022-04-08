/*
    Code belonging to practice 2 of SSOOII. The code made is intended to simulate the search for a substring 
    in a file passed by arguments by using threads. The implementation carried out does not distinguish 
    between uppercase or lowercase, so that if the word searched for is DaVid, all the results corresponding 
    to david, DAVID or DaVid will appear. similarly, the implementation will also take into account the 
    substrings included in a larger string so that if you search for example "al"; all words containing the 
    substring "al" will be taken into account.

    Code made by:
        - MIGUEL DE LAS HERAS FUENTES
        - FRANCISCO JESÚS DÍAZ PELLEJERO
        - JAVIER VILLAR ASENSIO

*/

#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

#include "Searcher.h"
#include "colors.h"

int CountLines(std::string filename);
void checkArguments(int argc, char **argv);
bool is_integer(char *str);
void assignTurns(int num_threads);
void printResults(std::string word);

//array to hold the start byte of each line
std::vector<int> numLines;
//array used to store all final results
std::vector<Result> Totalsearches;
//semaphore used to restrict access to the flag variable
std::mutex m;
//vector used to restrict access to the vector
std::mutex vectorLock;
//variable to assign each thread a turn
int flag;

int main(int argc, char **argv){

    checkArguments(argc,argv);

    int num_threads=atoi(argv[3]);

    int num_lines=CountLines(argv[1]);

    /* if the number of threads requested is greater than the number of lines of the indicated file; 
    program ends */
    if(num_lines<num_threads) {
        std::cerr << RED<< "The number of threads is higher than the number of lines"<< RESET<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "The file has "<< num_lines <<" lines"<< std::endl;

    //vectors in which we will store the created threads and the searcher instances
    std::vector<std::thread> v_hilos;
    std::vector<Searcher> v_objetos;

    //lines of each thread
    int task_size = num_lines/num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        /* variables indicating the start and end line of each thread.  if it is the last thread; it is 
        assigned until the last line */
        int l_begin= i*task_size;
        int l_end;
        if(i!=num_threads-1) l_end= (l_begin+task_size)-1;
        else l_end=num_lines-1;

        Searcher s{i+1,l_begin,l_end,argv[1],argv[2]};
        v_hilos.push_back(std::thread(s));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //round robin so that each thread stores the search results in the global array (Totalsearches)
    assignTurns(num_threads);

    //wait until all threads finish
    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));

    printResults(argv[2]);
    
    return 0;
}
/* method to count the number of lines of the requested file and to obtain the byte in which each line 
starts so that we can tell each thread in which byte to start reading. we store that byte in the vector 
"numLines" */
int CountLines(std::string filename){
    int lines;
    std::string line;
    std::ifstream mFile(filename);
    if(!mFile.is_open()) 
	{
        std::cerr<< RED <<"Could not open file: "<< filename << RESET << std::endl;
        exit(EXIT_FAILURE);
	}

	while(mFile.peek()!=EOF)
	{
        numLines.push_back(mFile.tellg());
		getline(mFile, line);
        lines++;
            
	}

	mFile.close();
	return lines;

}
/* method to check that the arguments used are correct. Otherwise, the execution of the program ends. */
void checkArguments(int argc, char **argv){
    if (argc!=4)
    {
        std::cerr << RED << "Usage: <SSOIIGLE> <file> <word> <number of threads>" <<  RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!is_integer(argv[3])){
        std::cerr << RED << "The number of threads input must be a positive integer" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    
}
//method used to check if the number of threads established is correct
bool is_integer(char *str){
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

/* method used to give each thread a turn to insert its search results into the global result vector */
void assignTurns(int num_threads){

    for (unsigned i = 0; i < num_threads; i++)
    {
        std::unique_lock<std::mutex> lk(m);
        while (!flag){
            flag=true;
            lk.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            lk.lock();
        }
        
    }
}

//method used to display the final results
void printResults(std::string word){
    std::cout << "Results for: " << word << std::endl;
    for (int  i = 0; i < Totalsearches.size(); i++)
    {
        Result result = Totalsearches[i];
        std::cout<< "[Hilo "<< BOLDYELLOW <<result.id << RESET<<" inicio: " << BOLDGREEN <<result.l_begin+1 
        << RESET <<" - final: "<< BOLDGREEN << result.l_end+1 << RESET <<"] línea " << BOLDRED
        << result.line<< RESET <<" :: ... "<< result.previous << " "<< BOLDBLUE<<result.word <<  RESET <<" "<< result.next << std::endl;
    }
}