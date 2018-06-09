#include <stdio.h>

#define Log(str, ...) printf(str "\n", ## __VA_ARGS__)
#define ELog(str, ...) fprintf(stderr, str "\n", ## __VA_ARGS__)

int main( int argc, char* argv[] ) {
	Log("moon");
	ELog("mom");

	return 0;
}
