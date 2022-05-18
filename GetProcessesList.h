#pragma once
#include <string>
#include <vector>
class process {
private:
    std::string _pid;
    std::string _name;
    std::string _status;
public:
    process(std::string pid, std::string name, std::string status);
    void printProcess();
    std::string get_pid();
    
};

std::vector<process> GetProcessesList();