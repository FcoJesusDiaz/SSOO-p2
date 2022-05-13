#include <vector>
#include <mutex>

#include "payment_service.h"
#include "client.h"


extern int *array_balances;
extern int id_send;
extern std::mutex payment_sem;
extern std::mutex client_sem;

void payment_service::update_balance(int client_id){

    //vector_clients[client_id];

}

void payment_service::operator()(){

    while(1){
        
        client_sem.lock();
        update_balance(id_send);
        payment_sem.unlock();
        
    }

}