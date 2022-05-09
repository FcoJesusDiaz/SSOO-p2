#ifndef REQUEST
#define REQUEST

#include <future>

#include "client.h"
class Request{
    private:
        std::future<std::string> fut;
        std::promise<std::string> prom;
        client_type type;
        std::string word;
    public:
        Request(int& balance, int client_id, client_type type, std::string word): 
            type(type), word(word){};
        void set_promise_value(int value);
        std::string get_future_value();
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();
        void operator()(int *balance);
};

#endif