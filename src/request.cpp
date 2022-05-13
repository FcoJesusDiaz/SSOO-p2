#include <future>
#include <string>
#include <future>
#include <iostream>

#include "request.h"

extern enum client_type type;


void Request::set_promise_value(std::string value){
    //std::cout << "[REQ_Client" << client_id << "]: Referencia a prom en set_promise value " << &prom << std::endl;
    prom.set_value(value);
}

int Request::getClientID(){
    return client_id;
}

client_type Request::getType(){
    return type;
}

int & Request::get_balance(){
    return balance;
}

std::string Request::to_string(){
    return "Client id: " + std::to_string(client_id) + ". Balance " + std::to_string(balance) + ". Word " + word;
}

std::string Request::getWord(){
    return word;
}

