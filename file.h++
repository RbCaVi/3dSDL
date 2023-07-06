#ifndef FILE_H
#define FILE_H
#include <filesystem>
char* pathtobuf(std::filesystem::path path);
char* pathtobuf(const char* path);
#endif