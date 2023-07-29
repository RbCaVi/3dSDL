#include "shared.h++"

#include <stddef.h>
#include <stdlib.h>

#include "chunk.h++"

chunk_exception::chunk_exception(const char* message,const int i): msg(message),_idx(i) {}

chunk_exception::~chunk_exception() noexcept {}

const char* chunk_exception::what() const noexcept {
   return msg;
}