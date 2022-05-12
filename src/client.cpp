#include <iostream>
#include <queue>

#include "client.h"
#include "request.h"

extern std::vector<std::string> dictionary;
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;

std::string Client::make_search(){
    fut = prom.get_future();
    Request req{balance, id, type, choose_word(), prom, fut};
    if(type == free_acc) premium_requests.push(req);
    else normal_requests.push(req);
    condition.notify_one();
    std::cout << "Client id: "<< id << " Palabra: " << req.getWord() << " Balance: " << balance <<std::endl;
    std::string word = fut.get();
    return word;
}

void Client::set_balance(int new_balance){
    balance = new_balance;
}

std::string Client::choose_word(){
    srand(time(NULL));
    int random_num = rand() % dictionary.size();
    std::cout << "Dictionary size: " << dictionary.size() << " Random number: " 
    << random_num << std::endl;
    std::cout << "Client id " << id << " Random number " << random_num << 
    "Palabra :" << dictionary[random_num] << std::endl;
    return dictionary[random_num];
}

void Client::operator()(){
   make_search();
};

