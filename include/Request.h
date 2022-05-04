#ifndef REQUEST
#define REQUEST

class Request{
    private:
        std::future<std::string> fut;
        std::promise<std::string> prom;
        std::string word;
    public:
        Request(int& balance, int client_id, int type, std::string word): word(word){};
        void set_promise_value(int value);
        std::string get_future_value();
        int getClientID();
        int getType();
        int decrease_balance();
        std::string getWord();
        void operator()(int *int);
};

#endif