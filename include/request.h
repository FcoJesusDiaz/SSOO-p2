#ifndef REQUEST
#define REQUEST

#include <future>

#include "client.h"
class Request{
    private:
        std::future<std::string> &fut;
        std::promise<std::string> &prom;
        client_type type;
        std::string word;
        int client_id;
    public:
        Request(int& balance, int client_id, client_type type, std::string word,
        std::promise<std::string>& prom, std::future<std::string>& fut): 
            type(type), word(word), prom(prom), fut(fut){};
        void set_promise_value(std::string value);
        std::string get_future_value();
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();

};

#endif