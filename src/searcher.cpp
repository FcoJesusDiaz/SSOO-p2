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
#include <memory>

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
    
    Request *req;
    while(1){
        std::unique_lock<std::mutex>queue_size(sem);
        std::vector<std::thread> v_hilos;
        std::vector<thread_searcher> v_objetos;

        condition.wait(queue_size, [&]{return !premium_requests.empty() || !normal_requests.empty();});
        occupied_threads = occupied_threads - 1;
        random_num = (rand() % 10) + 1;

        if(premium_requests.empty() && !normal_requests.empty()){
            sem_normal.lock();
            req = new Request(std::ref(normal_requests.front()));
            normal_requests.pop();
            sem_normal.unlock();
        }

        else if(!premium_requests.empty() && normal_requests.empty()){
            sem_premium.lock();
            req = new Request(std::ref(premium_requests.front()));
            premium_requests.pop();
            sem_premium.unlock();
        }

        else if(!premium_requests.empty() && !normal_requests.empty() && random_num >= 1 && random_num <= 8){
            sem_premium.lock();
            req = new Request(std::ref(premium_requests.front()));
            premium_requests.pop();
            sem_premium.unlock();
        }
        
        else{
            sem_normal.lock();
            std::cout << "[SEARCHER " << id << "]: Retrieving request for normal requests queue" << std::endl;
            req = new Request(std::ref(normal_requests.front()));
            normal_requests.pop();
            sem_normal.unlock();
        }

        std::cout << BOLDBLUE << "[SEARCHER " << id << "]: Element retreived from queue: " << req->to_string() << RESET << std::endl;

        queue_size.unlock();
        std::mutex balance_sync;

        unsigned t0 = clock();

        for (long unsigned i = 0; i < files.size(); i++)
        {
            thread_searcher s{(int)(i+1),files[i],req->getWord(), colours[i % 4], std::ref(req->get_balance()),
             req->getType(), req->getClientID(), std::ref(balance_sync)};
            v_objetos.push_back(s);
        }

        for (int i = 0; i < num_threads; i++){
            v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
        }

        //wait until all threads are finished
        std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));

        unsigned t1 = clock();

        double time = (double(t1-t0) / CLOCKS_PER_SEC);
        int client_id = req->getClientID();
        client_type type = req->getType();
        std::string word = req->getWord();
    
        std::string results = "[Client id " + std::to_string(client_id) + "] with type " + std::to_string(type) + ". 0(Free account), 1(Limited premium), 2(Unlimited_premium)\n";
        results = results + "Time of execution in seconds: " + std::to_string(time) + " \nResults for: " + BOLDYELLOW + word + RESET + "\n";
        for(long unsigned int i = 0; i < v_objetos.size(); i++){
            results += v_objetos[i].to_string();
        }

        req->set_promise_value(results);
        
        occupied_threads = occupied_threads + 1;
        std::cout << BOLDBLUE << "[SEARCHER " << id << "]: " << "Finished request of client " << 
        std::to_string(client_id) << ". Free searchers: " << occupied_threads << RESET << std::endl;
        req = NULL;
    }
}

