TARGET:=tower_defense

D_SRC:=src
D_INC:=include
BUILD:=bin
D_OBJ:=$(BUILD)/obj
D_DEP:=$(BUILD)/dep
FOLDERS:=game utils graphics
DIRS:=$(BUILD) $(D_DEP) $(D_OBJ) $(addprefix $(D_OBJ)/, $(FOLDERS)) $(addprefix $(D_DEP)/, $(FOLDERS))
SRC:=$(wildcard $(D_SRC)/*.c $(D_SRC)/*/*.c)
OBJ:=$(patsubst $(D_SRC)/%.c, $(D_OBJ)/%.o, $(SRC))
DEP:=$(patsubst $(D_SRC)/%.c, $(D_DEP)/%.d, $(SRC))

CC:=gcc
CFLAGS:=-Wall -Wextra -std=c17 -I$(D_INC) `pkg-config --cflags MLV` -g3
LDFLAGS:=`pkg-config --libs-only-other --libs-only-L MLV`
LDLIBS:=`pkg-config --libs-only-l MLV`
DFLAGS=-MP -MMD -MF $(D_DEP)/$*.d -MT $@

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

-include $(DEP)

$(D_OBJ)/%.o : $(D_SRC)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -o $@ -c $<

$(DIRS) :
	mkdir -p $@

run: $(TARGET)
	./$<

clean :
	rm -rf $(BUILD)/*
