#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <ctime>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <condition_variable>
#include <dirent.h>

#include "colors.h"
#include "client.h"
#include "request.h"
#include "searcher.h"


#define NUMSEARCHERS 1
#define MAX_BALANCE 20


//global variables
std::vector<std::string> dictionary;
std::queue<Request> premium_requests;
std::queue<Request> normal_requests;
std::condition_variable condition;
std::mutex sem;
std::unique_lock<std::mutex>queue_size(sem);

std::vector<std::string> files;

void create_searchers(int num_search);
void check_arguments(int argc, char **argv);
bool is_integer(char *str);
void create_dictionary(char *filename);
void create_clients(int n_clients);
void get_filenames();


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
    get_filenames();
    for(int i = 0; i < num_search; i++) {
        std::cout << "[MANAGER]: Creating searcher " << i << std::endl;
        Searcher s(i);
        std::thread t(s);
        t.detach();
    }
}

void get_filenames(){
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("Libros")) != NULL) {
    
        while ((ent = readdir (dir)) != NULL) {
            std::string file = ent->d_name;
            file = "Libros/" + file;
            if(file!="Libros/."&& file!="Libros/.." && file != "Libros/dictionary.txt") 
                files.push_back(file);
        }
        closedir (dir);
    } else {
        std::cerr << RED << "Could not open the directory" <<  RESET << std::endl;
    }
}

void create_clients(int n_clients){
    std::vector<std::thread>vec_threads;
    client_type type;

    std::srand(time(NULL)); 
    for(int i = 0; i < n_clients; i++){
        std::cout << "[MANAGER]: " << "creating client " << i+1 << std::endl;
        type = (client_type)(std::rand() % 3);
        Client c{i+1, type , (type == unlimited_prem) ? -1 : (std::rand() % MAX_BALANCE)};
        vec_threads.push_back(std::thread(std::ref(c)));
        std::this_thread::sleep_for (std::chrono::milliseconds(50));
    }
    std::for_each(vec_threads.begin(),vec_threads.end(),std::mem_fn(&std::thread::join));
}