#include "client.h"

#include <string>

std::string Client::make_search(std::string word){
    Request req{balance, id, type, word};
    cola_de_requests.push(std::ref(req));
    variable_de_aceso_a_cola_de_request.notify_one();
    return req.get_future_value();
}

void Clien::setBalance(int new_balance){
    balance = new_balance;
}
