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
        
    public:
        Request(int balance, int client_id, client_type type, std::string word,
        std::promise<std::string>& prom): 
            balance(balance), client_id(client_id),
            type(type), word(word), prom(prom){};

        void set_promise_value(std::string value);
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();
        std::string to_string();

};

#endif