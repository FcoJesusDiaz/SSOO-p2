#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <mutex>
#include <bits/stdc++.h>
#include <dirent.h>
#include <stdlib.h>
#include <condition_variable>

#include "request.h"
#include "colors.h"
#include "searcher.h"

/*DECLARATIONS OF FUNCTIONS*/
int CountLines(std::string filename);
void checkArguments(int argc, char **argv);
bool is_integer(char *str);
void printResults(std::string word, std::vector<thread_searcher>);
void get_filenames();


/*GLOBAL VARIABLES*/
std::vector<int> numLines; //array to hold the start byte of each line
std::string colours[] = {BOLDBLUE, BOLDGREEN, BOLDYELLOW, BOLDMAGENTA};
std::vector<std::string> files;

/*EXTERN VARIABLES*/
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem;
extern std::unique_lock<std::mutex>queue_size;


void Searcher::operator()(){
    std::cout << "[Searcher "<< this->id << "] My id is: " << this->id << std::endl;

    get_filenames();

    int num_threads=files.size(); //al ser un hilo por libro; se crean tantos hilos como libros

    //vectors in which we will store the created threads and the searcher instances
    std::vector<std::thread> v_hilos;
    std::vector<thread_searcher> v_objetos;
    int random_num;

    Request *req;

    while(1){
        condition.wait(queue_size, [&]{return !premium_requests.empty() || !normal_requests.empty();});
        
        srand(time(NULL));  
        random_num = (rand() % 10) + 1;
        
        if(!premium_requests.empty() && random_num >= 1 && random_num <= 8){
            req = &premium_requests.front();
            premium_requests.pop();
        }
        
        else if(!normal_requests.empty() && (random_num == 9 || random_num == 10)){
            req = &normal_requests.front();
            normal_requests.pop();
        }
        
        for (long unsigned i = 0; i < files.size(); i++)
        {
            thread_searcher s{i+1,files[i],req->getWord(), colours[i % 4]};
            v_objetos.push_back(s);
        }

        for (int i = 0; i < num_threads; i++){
            v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
        }
    }

    //wait until all threads are finished
    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
    
    //printResults(argv[1], v_objetos);
}

/* method to count the number of lines of the requested file and to obtain the byte in which each line 
starts so that we can tell each thread in which byte to start reading. we store that byte in the vector 
"numLines" */
int Searcher::CountLines(std::string filename){
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

void Searcher::printResults(std::string word, std::vector<thread_searcher> v_objetos){
    std::cout <<" Results for: " << BOLDYELLOW << word << RESET << std::endl;

    for(long unsigned int i = 0; i < v_objetos.size(); i++){
        std::cout << v_objetos[i].to_string();
    }
}

void Searcher::get_filenames(){
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("Libros")) != NULL) {
    
        while ((ent = readdir (dir)) != NULL) {
            std::string file = ent->d_name;
            file = "Libros/" + file;
            if(file!="Libros/."&& file!="Libros/..") files.push_back(file);
        }
        closedir (dir);
    } else {
        std::cerr << RED << "Could not open the directory" <<  RESET << std::endl;
    }
}