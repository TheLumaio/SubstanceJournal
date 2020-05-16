## Yoinked from https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

TARGET_EXEC ?= bin/SUBSTANCE.EXE

BUILD_DIR ?= ./obj
SRC_DIRS ?= ./src

SRCS := $(shell gnufind $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell gnufind $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LDFLAGS := -std=c99 -static -lraylib -lenet -lopengl32 -lgdi32 -limagehlp -lwinmm -lws2_32 -lssl -lcrypto

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(TARGET_EXEC): $(OBJS)
	$(CC) -g $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) -g $(ASFLAGS) -c $< -o $@

# c sourceB
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -g $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -g $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= gnumkdir -p
