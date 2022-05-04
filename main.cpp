#define BUF (256)
#include <iostream>
#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>
#include <vector>
#include "key_logger.h"
#include "Mice.h"
#include <string>
#include <map>
#include <fstream>
#include "GetProcessesList.h"


#pragma region Функции
float GetComputerOperatingTime()
{
    char buf[BUF] = {'\0'};
    std::fstream fs("/proc/uptime", std::fstream::in);
    if(!fs.is_open())
        return 0;
    float a = -1,b;
    if ( !(fs.getline(buf, BUF).eof() )){
        if (sscanf(buf, "%f %f", &a, &b) != 2)
        {
            fs.close();
            return -1;
        }
    }
    fs.close();
    return a;
}


void print_list_processes(std::vector<process> processesList){
    for(size_t i = 0; i < processesList.size(); ++i){
        processesList[i].printProcess();
    }
}
void write_press_keys_to_file(const char* path, std::vector<std::string>& names)
{
    std::fstream f;
    f.open(path, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!f.eof())
        f << "\n";
    for (auto i = 0; i < names.size(); ++i)
    {
        f << names[i];
        if (i == 0 || i == names.size() -2)
            f << "\n";
    }
    f << "\n\n";
    f.close();
}
#pragma endregion

int main()
{ 

    #pragma region Кей логгер 
    //Key_logger name_of_the_keys;
    
    #pragma region  Ввод пути
    char Name[100];
    std::cout << "Enter the name of the user >> ";
    std::cin >> Name;
    auto path = strcat(Name, ".txt");
    #pragma endregion

    //std::vector<std::string> names = name_of_the_keys.get_keys_vector(path);
    //write_press_keys_to_file(path, names);
    #pragma endregion

    auto proceses = GetProcessesList();
    //print_list_processes(proceses);
    // active_mice_stream();
    return 0;
}