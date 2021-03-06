TARGET									:=	noiseload.exe
SRC										:=	src
OUT										:=	.output

INCLUDE_DIRS							:=	src \
											src/external

# Functions (must use '=', and not ':=') #
REMOVE_UNDERSCORE	=	$(foreach v,$(1),$(if $(findstring /_,$(v)),,$(v)))
FIND_FILE			=	$(call REMOVE_UNDERSCORE,$(shell find $(1) -name '$(2)'))
# NOTE: My standard build tree rule is to ignore any file/folder prefixed with an underscore #

#PREFIX									:=	x86_64-w64-mingw32-
#CC										:=	$(PREFIX)gcc
#CXX										:=	$(PREFIX)g++

.C_FILES								:=	$(call FIND_FILE,$(SRC)/,*.c)
.CPP_FILES								:=	$(call FIND_FILE,$(SRC)/,*.cpp)

OUT_FILES								+=	$(subst $(SRC)/,$(OUT)/,$(.CPP_FILES:.cpp=.cpp.o))
OUT_FILES								+=	$(subst $(SRC)/,$(OUT)/,$(.C_FILES:.c=.c.o))

OUT_FOLDERS								:=	$(sort $(dir $(OUT_FILES)))

COMMON_FLAGS							+=	$(addprefix -I ,$(INCLUDE_DIRS)) -std=c++14
LD_FLAGS								+=	-lzip


$(TARGET): $(OUT_FOLDERS) $(OUT_FILES)
	$(CXX) $(OUT_FILES) $(LD_FLAGS) -o $@


$(OUT)/%.cpp.o: $(SRC)/%.cpp
	$(CXX) -c $< $(COMMON_FLAGS) -o $@

$(OUT)/%.c.o: $(SRC)/%.c
	$(CC) -c $< $(COMMON_FLAGS) -o $@


$(OUT_FOLDERS):
	mkdir -p $@

clean:
	rm -fr $(OUT) $(TARGET)

run: $(TARGET)
	./$(TARGET) TestSong.xrns out.bin
	hexdump -vC out.bin

info:
	echo "$(.CPP_FILES)"

.PHONY: clean run
