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
    //std::cout << "Client id "<< id << ": Referencia a prom" << &prom <<std::endl;
    
    //notifications.lock();
    if(type == free_acc) {
        std::cout << BOLDYELLOW << "Client id "<< id << ": pushing to normal queue" << RESET << std::endl;
        sem_normal.lock();
        normal_requests.push(Request {balance, id, type, choose_word(), std::ref(prom)});
        sem_normal.unlock();
        std::cout << BOLDYELLOW << "Client id "<< id << ": pushed to normal queue" << RESET << std::endl;
    }
    else {
        std::cout << BOLDYELLOW << "Client id "<< id << ": pushing to premium queue" << RESET << std::endl;
        sem_premium.lock();
        premium_requests.push(Request {balance, id, type, choose_word(), std::ref(prom)});
        sem_premium.unlock();
        std::cout << BOLDYELLOW << "Client id "<< id << ": pushed to premium queue" << RESET << std::endl;
    }
    notifications.lock();

    while(occupied_threads == 0){
        //std::cout << BOLDGREEN << "Client id "<< id << ": All searchers are currently occupied" << RESET << std::endl;
    }
    
    std::cout << BOLDBLUE << "Client id "<< id << ": Petition notified" << RESET << std::endl;
    //std::this_thread::sleep_for (std::chrono::milliseconds(100));
    condition.notify_one();
    notifications.unlock();

    //if(fut.valid()) std::cout << BOLDBLUE << "Client id "<< id << ": Future valido" << RESET << std::endl;
    //else std::cout << BOLDRED << "Client id "<< id << ": FUTURE NO VALIDO" << RESET << std::endl;
    
    std::string result = fut.get();

    return result;
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

