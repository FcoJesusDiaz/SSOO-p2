#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <mutex>
#include <bits/stdc++.h>
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


/*GLOBAL VARIABLES*/
std::vector<int> numLines; //array to hold the start byte of each line
std::string colours[] = {BOLDBLUE, BOLDGREEN, BOLDYELLOW, BOLDMAGENTA};
extern std::vector<std::string> files;

/*EXTERN VARIABLES*/
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem;
extern std::unique_lock<std::mutex>queue_size;


void Searcher::operator()(){
    std::cout << "[Searcher "<< this->id << "] My id is: " << this->id << std::endl;

    int num_threads=files.size(); //al ser un hilo por libro; se crean tantos hilos como libros

    //vectors in which we will store the created threads and the searcher instances
    std::vector<std::thread> v_hilos;
    std::vector<thread_searcher> v_objetos;
    int random_num;

    Request *req;

    while(1){
        condition.wait(queue_size, [&]{return !premium_requests.empty() || !normal_requests.empty();});

        random_num = (rand() % 10) + 1;
        std::cout << "random number: " << random_num << std::endl;
        
        if(premium_requests.empty() && !normal_requests.empty()){
            std::cout << "Retrieving request for normal requests queue" << std::endl;
            req = &normal_requests.front();
            normal_requests.pop();
        }

        else if(!premium_requests.empty() && normal_requests.empty()){
            std::cout << "Retrieving request for premium requests queue" << std::endl;
            req = &premium_requests.front();
            premium_requests.pop();
        }

        else if(!premium_requests.empty() && !normal_requests.empty() && random_num >= 1 && random_num <= 8){
            std::cout << "Retrieving request for premium requests queue" << std::endl;
            req = &premium_requests.front();
            premium_requests.pop();
        }
        
        else{
            std::cout << "Retrieving request for normal requests queue" << std::endl;
            req = &normal_requests.front();
            normal_requests.pop();
        }
        
        for (long unsigned i = 0; i < files.size(); i++)
        {
            thread_searcher s{i+1,files[i],req->getWord(), colours[i % 4]};
            v_objetos.push_back(s);
        }

        std::cout << "Files size: " << files.size() << std::endl;
        for (int i = 0; i < num_threads; i++){
            v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
        }

        //wait until all threads are finished
        std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
        std::string results = results + " Results for: " + BOLDYELLOW + req->getWord() + RESET + "\n";
        for(long unsigned int i = 0; i < v_objetos.size(); i++){
            results += v_objetos[i].to_string();
        }
        req->set_promise_value(results);

        //send_results(req->getWord(), v_objetos);
    }
}

void Searcher::send_results(std::string word, std::vector<thread_searcher> v_objetos){
    
}

