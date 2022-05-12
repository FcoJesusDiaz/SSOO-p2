#include <string>

class PremiumClient : public Client
{}
    PremiumClient::PremiumClient(int id, std::string word): Client(id, word);