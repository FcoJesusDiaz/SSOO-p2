#include<iostream>
#include<fstream>
#include<string>
 
using namespace std;
 
int main()
{
	string filename;   
	cout<<"Introduce el nombre del archivo:";
	cin>>filename;
 
	string line;   
	int lines=0;    
	string word="main";

	ifstream mFile (filename);   
	
	if(mFile.is_open()) 
	{
		while(mFile.peek()!=EOF)
		{
			getline(mFile, line);
            lines++;
            if (line.find(word)!=-1)
            {
                cout<<"Se ha encontrado "<<word<< " en la linea "<< lines<<endl;
            }
            
		}
		mFile.close();
		cout<<"úumero de líneas en el archivo: "<<lines<<endl;
	}
	else
		cout<<"No se pudo abrir el archivo\n";
 
	return 0;
}