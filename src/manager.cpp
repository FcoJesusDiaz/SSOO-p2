#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <time.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <condition_variable>

#include "colors.h"
#include "client.h"
#include "request.h"


#define NUMSEARCHERS 4


//global variables
std::vector<std::string> dictionary;
std::queue<Request> premium_requests;
std::queue<Request> normal_requests;
std::condition_variable condition;
std::mutex sem;
std::unique_lock<std::mutex>queue_size(sem);




void create_searchers(int num_search);
void check_arguments(int argc, char **argv);
bool is_integer(char *str);
void create_dictionary(char *filename);
void create_clients(int n_clients);

int main(int argc, char **argv){
    check_arguments(argc, argv);
    create_dictionary(argv[2]);
    create_searchers(NUMSEARCHERS);
    create_clients(atoi(argv[1]));
    return EXIT_SUCCESS;
}

void check_arguments(int argc, char **argv){
    if (argc != 3)
    {
        std::cerr << RED << "Usage: <SSOIIGLE> <n_clients> <filename (for dictionary generation)>" <<  RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!is_integer(argv[1])){
        std::cerr << RED << "The number of clients input must be a positive integer" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }   
}

//method used to check if the number of threads established is correct
bool is_integer(char *str){
    while (*str)
        if (!isdigit(*str++))
            return false;
    return true;
}

void create_dictionary(char *filename){
    std::ifstream file;
    std::string word;
    file.open(filename);

    if (!file.is_open()){
        std::cerr << RED << "Could not open txt file for dictionary generation" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    while (file >> word)
        if (word.length() > 2) dictionary.push_back(word);
    file.close();
}

void create_searchers(int num_search){
    pid_t searcher_pid;
    for(int i = 0; i < num_search; i++) {
        std::cout << "[MANAGER]: Creating child " << i << std::endl;
        searcher_pid = fork();
        
        if(searcher_pid == 0)
            execlp("exec/searcher", "searcher", std::to_string(i).c_str(), NULL);
    }
}

void create_clients(int n_clients){
    std::vector<std::thread>vec_threads;

    for(int i = 0; i < n_clients; i++){
        std::cout << "[MANAGER]: " << "creating client " << i+1 << std::endl;
        Client c{i+1, (client_type)(rand() % 3)};
        srand(time(NULL));
        vec_threads.push_back(std::thread(c));
        if(i % (std::thread::hardware_concurrency()/2) == 3)
        std::this_thread::sleep_for (std::chrono::seconds(2));
    }
    std::for_each(vec_threads.begin(),vec_threads.end(),std::mem_fn(&std::thread::join));
}