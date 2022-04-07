#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#define READ_BUF_SIZE 512

using namespace std;

class process {
private:
    string _pid;
    string _name;
    string _status;

public:
    process(string pid, string name, string status): _pid(pid), _status(status), _name(name){};

    void printProcess(){
        std::cout <<" Pid:" << _pid << std::endl;
        std::cout <<" Name:" << _name << std::endl;
        std::cout <<" Status:" << _status << std::endl << std::endl;
    }
};

vector<process> GetProcessesList() {
    DIR *dir;
    struct dirent *next;
    std::vector<process> procList;

    dir = opendir("/proc");
    if (!dir) {
        perror("Cannot open /proc");
        exit(-1);
    }

    while ((next = readdir(dir)) != NULL) {
        FILE *status;
        char filename[READ_BUF_SIZE];
        char buffer[READ_BUF_SIZE];
        char name[READ_BUF_SIZE];
        char stat[READ_BUF_SIZE];

        if (strcmp(next->d_name, "..") == 0)
            continue;

        if (!isdigit(*next->d_name))
            continue;

        sprintf(filename, "/proc/%s/status", next->d_name);
        if (!(status = fopen(filename, "r"))) { continue; }

        fgets(buffer, READ_BUF_SIZE - 1, status);
        sscanf(buffer, "%*s %s ", name);

        fgets(buffer, READ_BUF_SIZE - 1, status);
        fgets(buffer, READ_BUF_SIZE - 1, status);
        sscanf(buffer, "%*s %*s %s", stat);

        procList.emplace_back(process(next->d_name, name, stat));
        fclose(status);
    }
    return procList;
}