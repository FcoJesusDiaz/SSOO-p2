#include <future>
#include <string>
#include <future>

#include "Request.h"

void Request::operator()(int* n){
    fut = prom.get_future();
}

void Request::set_promise_value(int value){
    prom.set_value(value);
}

std::string Request::get_future_value(){
    return fut.get();
}

int Request::getClientId(){
    return client_id;
}

int Request::getType(){
    return type;
}

int Request::decrease_balance(){
    return --balance;
}

std::string getWord(){
    return word;
}

