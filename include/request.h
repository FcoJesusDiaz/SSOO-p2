#ifndef REQUEST
#define REQUEST

#include <future>

#include "client.h"

class Request{
    private:
        int balance;
        int client_id;
        client_type type;
        std::string word;
        std::promise<std::string>& prom;
        std::future<std::string>& fut;
        
    public:
        Request(int balance, int client_id, client_type type, std::string word,
        std::promise<std::string>& prom, std::future<std::string>& fut): 
            balance(balance), client_id(client_id),
            type(type), word(word), prom(prom), fut(fut){};

        void set_promise_value(std::string value);
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();
        std::string to_string();

};

#endif