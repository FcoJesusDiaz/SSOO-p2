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
#include <atomic>

#include "request.h"
#include "colors.h"
#include "searcher.h"

/*DECLARATIONS OF FUNCTIONS*/
int CountLines(std::string filename);
void checkArguments(int argc, char **argv);
bool is_integer(char *str);
void printResults(std::string word, std::vector<thread_searcher>);


/*GLOBAL VARIABLES*/
std::string colours[] = {BOLDBLUE, BOLDGREEN, BOLDYELLOW, BOLDMAGENTA};
extern std::vector<std::string> files;

/*EXTERN VARIABLES*/
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem;
extern std::mutex sem_premium;
extern std::mutex sem_normal;
extern std::atomic<int> occupied_threads;

void Searcher::operator()(){
    int num_threads = files.size(); //al ser un hilo por libro; se crean tantos hilos como libros
    int random_num;

    while(1){
        std::unique_lock<std::mutex>queue_size(sem);
        std::vector<std::thread> v_hilos;
        std::vector<thread_searcher> v_objetos;
        Request *req;
        

        condition.wait(queue_size, [&]{return !premium_requests.empty() || !normal_requests.empty();});
        occupied_threads = occupied_threads - 1;
        std::cout << "[Searcher " << id << "]: Exiting condition..." << std::endl;
        std::cout << BOLDGREEN << "[SEARCHER " << id << "]: "<<"Free searchers: " << occupied_threads << RESET << std::endl;
        random_num = (rand() % 10) + 1;
        
        if(premium_requests.empty() && !normal_requests.empty()){
            sem_normal.lock();
            std::cout << "[Searcher " << id << "]: Retrieving request for normal requests queue" << std::endl;
            req = &normal_requests.front();
            normal_requests.pop();
            sem_normal.unlock();
        }

        else if(!premium_requests.empty() && normal_requests.empty()){
            sem_premium.lock();
            std::cout << "[Searcher " << id << "]: Retrieving request for premium requests queue" << std::endl;
            req = &premium_requests.front();
            premium_requests.pop();
            sem_premium.unlock();
        }

        else if(!premium_requests.empty() && !normal_requests.empty() && random_num >= 1 && random_num <= 8){
            sem_premium.lock();
            std::cout << "[Searcher " << id << "]: Retrieving request for premium requests queue" << std::endl;
            req = &premium_requests.front();
            premium_requests.pop();
            sem_premium.unlock();
        }
        
        else{
            sem_normal.lock();
            std::cout << "[Searcher " << id << "]: Retrieving request for normal requests queue" << std::endl;
            req = &normal_requests.front();
            normal_requests.pop();
            sem_normal.unlock();
        }

        std::cout << BOLDBLUE << "[Searcher " << id << "]: Element retreived: " << req->to_string() << RESET << std::endl;

        queue_size.unlock();
        
        for (long unsigned i = 0; i < files.size(); i++)
        {
            thread_searcher s{(int)(i+1),files[i],"hola", colours[i % 4]};
            v_objetos.push_back(s);
        }

        for (int i = 0; i < num_threads; i++){
            v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
        }

        //wait until all threads are finished
        std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
        
        std::string results = results + " Results for: " + BOLDYELLOW + "hola" + RESET + "\n";
        for(long unsigned int i = 0; i < v_objetos.size(); i++){
            results += v_objetos[i].to_string();
        }
        req->set_promise_value(results);

        std::this_thread::sleep_for (std::chrono::milliseconds(50));
        occupied_threads = occupied_threads + 1;
        std::cout << BOLDGREEN << "[SEARCHER " << id << "]: "<<"Finished request, free searchers: " << occupied_threads << RESET << std::endl;
    }
}

