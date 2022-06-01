#include <string>
#include<sys/socket.h>
#include "check.hpp"
enum actions {
	START,
	END,
    KEYBOARD,
    MOUSE
};  

struct message {
	actions action;
	std::string data;
    std::string user_name;
};

constexpr int PORT = 59987;
inline bool try_send(int fd, const message& msg) {
	errno = 0;
	int size = check_except(send(fd, &msg, sizeof(message), 0), EPIPE);
	return size > 0;
}

inline bool try_recv(int fd, message& msg) {
	errno = 0;
	int size = check_except(recv(fd, &msg, sizeof(message), MSG_WAITALL), EPIPE);
	return size > 0;
}