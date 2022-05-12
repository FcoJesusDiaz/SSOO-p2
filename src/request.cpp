#include <future>
#include <string>
#include <future>
#include <iostream>

#include "request.h"

extern enum client_type type;


void Request::set_promise_value(std::string value){
    prom.set_value(value);
}

int Request::getClientID(){
    return client_id;
}

int Request::getType(){
    return type;
}

std::string Request::to_string(){
    return "Client id: " + std::to_string(client_id) + ". Balance " + std::to_string(balance) + ". Word " + word;
}

/*int Request::decrease_balance(){
    return --balance;
}*/

std::string Request::getWord(){
    return word;
}

