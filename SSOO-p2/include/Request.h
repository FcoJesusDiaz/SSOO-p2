#ifndef REQUEST
#define REQUEST

#include <string>

class Request{
    private:
        int& balance;
        std::future<std::string> fut;
        std::promise<std::string> prom;
        int client_id;
        int type;
        std::string word;
    public:
        Request(int& balance, int client_id, int type, std::string word): 
            balance(balance), client_id(client_id), type(type), word(word){};
        void set_promise_value(std::string value);
        std::string get_future_value();
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();
};

#endif