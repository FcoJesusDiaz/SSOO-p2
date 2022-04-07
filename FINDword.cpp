#include <iostream>
#include <thread>
#include <stdlib.h>
#include <queue>
#include <string>
#include <bits/stdc++.h>

int main(){
    std::string line = "hola me llamo GERmaN daniel german";
    std::string word= "german";

    std::vector<std::string> tokens;

    std::stringstream check1(line);
    std::string intermediate;
    while (getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    for (unsigned i = 0; i < tokens.size(); i++)
    {
        std::transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), ::tolower);
        if (word==tokens[i])
        {
            std::cout<< tokens[i-1] << " - " << tokens[i] << " - " << tokens[i+1] << std::endl;
        }
        
    }
    return 0;
}