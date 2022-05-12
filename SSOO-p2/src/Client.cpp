#include <string>

class Client
{
    private:
        int _id;
        std::string _word;
    
    public:
        std::string getWord(){
            return _word;
        }
        int getID(){
            return _id;
        }

        Client(int id, std::string word): _id(id), _word(word);
}