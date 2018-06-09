

OUT			:=	noise.exe

$(OUT): $(wildcard src/*.cpp)
	g++ $^ -o $@
