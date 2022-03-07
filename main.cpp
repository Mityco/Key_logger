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

int check(int result){
    if (result >= 0){
        std::cout << "DONE!" << std::endl;
        return result;
    }
        
    perror(strerror(errno));
    exit(-1);
}

int main()
{
    
    int keys_fd;
    char ret[2];
    struct input_event t;
    
    keys_fd = check(open("/dev/input/event2", O_RDONLY));
    printf("%d\n", keys_fd);
    if (keys_fd <= 0)
    {
        printf("open/dev/input/event2 device error!\n");
        return 0;
    }
    while (true)
    {
        if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            if (t.type == EV_KEY)
                if (t.value == 0 || t.value == 1)
                {
                    printf("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released");
                    if (t.code == KEY_ESC)
                        break;
                }
        }
    }

    close(keys_fd);
    return 0;
}