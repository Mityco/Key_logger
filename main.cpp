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
#include <fstream>
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
void write_press_keys_to_file(const char* path, const std::vector<std::string>& names)
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

std::string Date_Time()
{
    time_t rawtime;
    tm * ptm;
    std::string strTime;
    std::stringstream out;

    time (&rawtime);
    ptm = gmtime (&rawtime);
    out<<(1900+ptm->tm_year)<<"/"
       <<(1+ptm->tm_mon)<<"/"
       <<ptm->tm_mday<<", "
       <<ptm->tm_hour<<":"
       <<ptm->tm_min<<":"
       <<ptm->tm_sec;
    getline(out, strTime);
    return strTime;
}

void active_mice_stream() {
    Display *display;
    Window root_window;

    display = XOpenDisplay(0);
    root_window = XRootWindow(display, 0);

    /* check XInput */
    int xi_opcode, event, error;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error)) {
        fprintf(stderr, "Error: XInput extension is not supported!\n");
        //return 1;
    }

    /* Check XInput 2.0 */
    int major = 2;
    int minor = 0;
    int retval = XIQueryVersion(display, &major, &minor);
    if (retval != Success) {
        fprintf(stderr, "Error: XInput 2.0 is not supported (ancient X11?)\n");
        //return 1;
    }

    unsigned char mask_bytes[(XI_LASTEVENT + 7) / 8] = {0};  /* must be zeroed! */
    XISetMask(mask_bytes, XI_RawMotion);

    /* Set mask to receive events from all master devices */
    XIEventMask evmasks[1];
    /* You can use XIAllDevices for XWarpPointer() */
    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask_bytes);
    evmasks[0].mask = mask_bytes;
    XISelectEvents(display, root_window, evmasks, 1);

    //1)long e_mask
    //2)pthread
    //3)XCheckMaskEvent
    XEvent xevent;
    auto first = time(NULL);
    auto second = first;
    auto write_flag_active = true;
    auto write_flag_unactive = true;
    while (1) {
        if (time(NULL) - first > 6 && write_flag_active == true) {
            std::cout << Date_Time() <<" Mouse UNACTIVE" << std::endl;
            write_flag_active = false;
            write_flag_unactive = true;
            first = time(NULL);
        }
        while (XPending(display)) {
            XNextEvent(display, &xevent);

            if (xevent.xcookie.type != GenericEvent || xevent.xcookie.extension != xi_opcode) {
                /* not an XInput event */
                continue;
            }
            XGetEventData(display, &xevent.xcookie);
            if (xevent.xcookie.evtype != XI_RawMotion) {
                XFreeEventData(display, &xevent.xcookie);
                continue;
            }
            XFreeEventData(display, &xevent.xcookie);

            Window root_return, child_return;
            int root_x_return, root_y_return;
            int win_x_return, win_y_return;
            unsigned int mask_return;
            int retval = XQueryPointer(display, root_window, &root_return, &child_return,
                                       &root_x_return, &root_y_return,
                                       &win_x_return, &win_y_return,
                                       &mask_return);
            if (!retval) {
                continue;
            }
            assert(root_x_return == win_x_return);
            assert(root_y_return == win_y_return);


            first = time(NULL);
            //Tyt root coordinate
            //printf("root: x %d y %d\n", root_x_return, root_y_return);
            if (write_flag_unactive == true) {
                std::cout << Date_Time() << " Mouse is ACTIVE" << std::endl;
                write_flag_unactive = false;
                write_flag_active = true;
            }
                if (child_return) {
                int local_x, local_y;
                XTranslateCoordinates(display, root_window, child_return,
                                      root_x_return, root_y_return,
                                      &local_x, &local_y, &child_return);
                //printf("local: x %d y %d\n\n", local_x, local_y);
            }
        }
    }
    XCloseDisplay(display);
}


#pragma endregion

int main()
{ 

    #pragma region Кей логгер 
    Key_logger name_of_the_keys;
    
    #pragma region  Ввод пути
    std::string Name;
    std::cout << "Enter the name of the user >> ";
    std::cin >> Name; 
    auto path = Name + ".txt";
    #pragma endregion
    
    chmod("/dev/input/event3",S_IRWXU|S_IRWXG|S_IROTH|S_IWOTH);
    //std::vector<std::string> names = name_of_the_keys.get_keys_vector(path);
    //write_press_keys_to_file(path, names);
    #pragma endregion
    //active_mice_stream();
    return 0;
}