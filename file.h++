#ifndef FILE_H
#define FILE_H
#include <filesystem>

char* pathtobuf(std::filesystem::path path);
char* pathtobufsize(std::filesystem::path path, int*);
#endif
