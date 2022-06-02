#include <string>
#include<sys/socket.h>
#include<vector>
enum actions {
	START,
	END,
    KEYBOARD,
    MOUSE
};  

struct message_header{
	actions action;
	size_t size;
};
struct message{
	std::string keys;
	std::string mice;
    std::string user_name;
};

constexpr int PORT = 59987;

inline bool try_send_header(int fd, const message_header& msg){
	errno = 0;
	int size_fd = send(fd, &msg, sizeof(message_header), 0);
	return size_fd > 0;
}
inline bool try_send(int fd, const message& msg, size_t size) {
	errno = 0;
	int size_fd = send(fd, &msg, size, 0);
	return size_fd > 0;
}

inline bool try_recv_header(int fd, message_header& msg) {
	errno = 0;
	int size_fd = recv(fd, &msg, sizeof(message_header), MSG_WAITALL);
	return size_fd > 0;
}
inline bool try_recv(int fd, message& msg,size_t size) {
	errno = 0;
	int size_fd = recv(fd, &msg, size, MSG_WAITALL);
	return size_fd > 0;
}