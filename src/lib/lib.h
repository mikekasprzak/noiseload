#pragma once
#include <stdio.h>

#define Log(str, ...) printf(str "\n", ## __VA_ARGS__)
#define ELog(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)
