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

int check(int result){
    if (result >= 0)    
        return result;
    
        
    perror(strerror(errno));
    exit(-1);
}

int main()
{
    Key_logger name_of_the_keys;
    int keys_fd; //дескриптор
    struct input_event t;  
    keys_fd = check(open(DEV_PATH, O_RDONLY));
    
    std::vector<const char*> names;
    int flag = 0;
    if (keys_fd <= 0)
    {
        printf("open/dev/input/js0 device error!\n");
        return 0;
    }
    while (true)
    {
        if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            if (t.type == EV_KEY)
                if (/*t.value == 0 || */ t.value == 1)
                {
                    if (flag == 42)
                         t.code += 1000;
                    names.push_back(name_of_the_keys.get_name_of_the_key(t.code));   
                    
                    
                    printf("key %s %s\n", name_of_the_keys.get_name_of_the_key(t.code), (t.value) ? "Pressed" : "Released");
                    
                    flag = t.code;
                }
                if (t.code == 1)
                    break;
        }

        
    }
    close(keys_fd);
    
    auto d_save = check(open("Roman.txt", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO));
    
    for (int i = 0; i < names.size(); ++i)
    {
        write(d_save, names[i], strlen(names[i]) );
    }

    close(d_save);
    return 0;
}