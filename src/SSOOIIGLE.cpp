#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <mutex>
#include <bits/stdc++.h>
#include <dirent.h>

#include "Searcher.h"
#include "colors.h"

/*DECLARATIONS OF FUNCTIONS*/
int CountLines(std::string filename);
void checkArguments(int argc, char **argv);
bool is_integer(char *str);
void printResults(std::string word, std::vector<Searcher>);
void get_filenames();


/*GLOBAL VARIABLES*/
std::vector<int> numLines; //array to hold the start byte of each line
std::string colours[] = {BOLDBLUE, BOLDGREEN, BOLDYELLOW, BOLDMAGENTA};
std::vector<std::string> files;

/*MAIN*/
int main(int argc, char **argv){

    //checkArguments(argc,argv);

    get_filenames();

    for (int i = 0; i < files.size(); i++)
    {
        std::cout << files[i]<< std::endl;
    }
    

    int num_threads=files.size(); //al ser un hilo por libro; se crean tantos hilos como libros

    //int num_lines=CountLines(argv[1]);

    
    /* if the number of threads requested is greater than the number of lines of the indicated file; 
    program ends */
    /*
    if(num_lines<num_threads) {
        std::cerr << RED<< "The number of threads is higher than the number of lines"<< RESET<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "The file has "<< num_lines <<" lines"<< std::endl;
    */

    //vectors in which we will store the created threads and the searcher instances
    std::vector<std::thread> v_hilos;
    std::vector<Searcher> v_objetos;

    //lines of each thread
    //int task_size = num_lines/num_threads;

    for (int i = 0; i < files.size(); i++)
    {
        /*
        int l_begin= i*task_size;
        int l_end;
        if(i!=num_threads-1) l_end= (l_begin+task_size)-1;
        else l_end=num_lines-1; */

        Searcher s{i+1,files[i],argv[1], colours[i % 4]};
        v_objetos.push_back(s);
    }


    for (int i = 0; i < num_threads; i++){
        v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
    }

    //wait until all threads are finished
    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
    
    printResults(argv[1], v_objetos);
    
    return EXIT_SUCCESS;
}

/* method to count the number of lines of the requested file and to obtain the byte in which each line 
starts so that we can tell each thread in which byte to start reading. we store that byte in the vector 
"numLines" */
int CountLines(std::string filename){
    int lines;
    std::string line;
    std::ifstream mFile(filename);
    if(!mFile.is_open()) 
	{
        std::cerr<< RED <<"Could not open file: "<< filename << RESET << std::endl;
        exit(EXIT_FAILURE);
	}

	while(mFile.peek()!=EOF)
	{
        numLines.push_back(mFile.tellg());
		getline(mFile, line);
        lines++;
            
	}

	mFile.close();
	return lines;

}
/* method to check that the arguments used are correct. Otherwise, the execution of the program ends. */
void checkArguments(int argc, char **argv){
    if (argc!=4)
    {
        std::cerr << RED << "Usage: <SSOIIGLE> <file> <word> <number of threads>" <<  RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!is_integer(argv[3])){
        std::cerr << RED << "The number of threads input must be a positive integer" << RESET << std::endl;
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

void printResults(std::string word, std::vector<Searcher> v_objetos){
    std::cout <<" Results for: " << BOLDYELLOW << word << RESET << std::endl;

    for(long unsigned int i = 0; i < v_objetos.size(); i++){
        std::cout << v_objetos[i].to_string();
    }
}

void get_filenames(){
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("Libros")) != NULL) {
    
        while ((ent = readdir (dir)) != NULL) {
            std::string file = ent->d_name;
            file = "Libros/" + file;
            if(file!="Libros/."&& file!="Libros/..") files.push_back(file);
        }
        closedir (dir);
    } else {
        std::cerr << RED << "Could not open the directory" <<  RESET << std::endl;
    }

    
}