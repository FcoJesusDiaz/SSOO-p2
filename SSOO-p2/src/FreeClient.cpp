#include <string>

class FreeClient : public Client
{}
    FreeClient::FreeClient(int id, std::string word): Client(id, word);