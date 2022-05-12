#include <string>

class BalancedPremiumClient : public Client
{
    private:
        int _balance;
    
    public:
        void setBalance(int newBalance){
            this -> balance = newBalance;
        }
        int getBalance(){
            return balance;
        }
        void decreaseBalance{
            this -> balance--;
        }
}
    BalancedPremiumClient::BalancedPremiumClient(int id, std::string word, int balance): Client(id, word), _balance(balance);