#include <iostream>
#include <queue>
#include <fstream>
#include <atomic>

#include "client.h"
#include "request.h"
#include "colors.h"

extern std::vector<std::string> dictionary;
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem_premium;
extern std::mutex sem_normal;
extern std::atomic<int> occupied_threads;
extern std::mutex notifications;

std::string Client::make_search(){
    std::promise<std::string> prom;
    std::future<std::string> fut;
    fut = prom.get_future();
    Request req{balance, id, type, choose_word(), std::ref(prom), std::ref(fut)};
    
    notifications.lock();
    if(type == free_acc) {
        std::cout << BOLDRED << "Client id "<< id << ": pushed to normal queue" << RESET << std::endl;
        sem_normal.lock();
        normal_requests.push(req);
        sem_normal.unlock();
    }
    else {
        std::cout << BOLDRED << "Client id "<< id << ": pushed to premium queue" << RESET << std::endl;
        sem_premium.lock();
        premium_requests.push(req);
        sem_premium.unlock();
    }

    while(occupied_threads == 0){
        //std::cout << BOLDGREEN << "Client id "<< id << ": All searchers are currently occupied" << RESET << std::endl;
    }
        std::cout << BOLDGREEN << "Client id "<< id << ": Petition notified" << RESET << std::endl;
    
    condition.notify_one();
    notifications.unlock();
    

    return fut.get();
}

void Client::set_balance(int new_balance){
    balance = new_balance;
}

std::string Client::choose_word(){
    int random_num = rand() % dictionary.size();
    return dictionary[random_num];
}

void Client::log_result(std::string results){
    std::string filename = LOG_DIR + std::to_string(id) + ".txt";
    std::ofstream out(filename);
    out << results;
}

void Client::operator()(){
   log_result(make_search());
};

