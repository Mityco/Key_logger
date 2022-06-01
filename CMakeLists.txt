cmake_minimum_required(VERSION 3.0)
project(application)
set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries( Threads::Threads rt atomic)

add_executable(application main.cpp GetProcessesList.cpp key_logger.cpp async_write.cpp)
target_link_libraries(application  X11 Xi)