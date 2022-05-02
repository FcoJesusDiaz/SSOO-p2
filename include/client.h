#ifndef CLIENT_H
#define CLIENT_H

enum client_type {free_acc, limited_prem, unlimited_prem};

class Client{
    private:
        int id;
        client_type type;
    public:
        Client(int id, client_type type): id(id), type(type){};
        void operator()();
};

#endif