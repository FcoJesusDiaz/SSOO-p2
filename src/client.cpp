#include <iostream>
#include <queue>
#include <fstream>

#include "client.h"
#include "request.h"
#include "colors.h"

extern std::vector<std::string> dictionary;
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;

std::string Client::make_search(){
    Request req{balance, id, type, choose_word(), std::ref(prom), std::ref(fut)};
    if(type == free_acc) normal_requests.push(req);
    else premium_requests.push(req);
    condition.notify_one();
    std::cout << "Client id: "<< id << " Client type " << type <<" Palabra: " << req.getWord() << " Balance: " << balance <<std::endl;
    std::string results = fut.get();
    std::cout << "Resultados:\n" << results << std::endl;
    return "";
}

void Client::set_balance(int new_balance){
    balance = new_balance;
}

std::string Client::choose_word(){
    int random_num = rand() % dictionary.size();
    return dictionary[random_num];
}

void Client::log_result(std::string results){
    std::cout << "Resultados:\n" << results << std::endl;
    std::string filename = filename + LOG_DIR + std::to_string(id) + ".txt";
    std::ofstream out(filename);
    out << results;
}

void Client::operator()(){
   make_search();
};

