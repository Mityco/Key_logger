#pragma once
#include <aio.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <fcntl.h>

struct aiocb createIoRequest(int fd,
                             off_t offset,
                             volatile void * content,
                             size_t length);

void write_vector_async_file(std::vector<std::string>& names, int fd);
