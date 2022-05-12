#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <future>

enum client_type {free_acc, limited_prem, unlimited_prem};

class Client{
    private:
        int id;
        client_type type;
        int balance;
        std::promise<std::string> prom;
        std::future<std::string> fut;
    public:
        Client(int id, client_type type, int balance): id(id), type(type), balance(balance){};
        std::string make_search();
        void set_balance(int new_balance);
        void operator()();
        std::string choose_word();
};

#endif
