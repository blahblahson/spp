CC=gcc
CFLAGS=-O2 -Wall -ggdb
LIBS=-lpthread

SOURCE=src
BUILD=build
TARGET=spp
CFILES=$(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
OBJS=$(foreach obj,$(CFILES:.c=.o),$(BUILD)/$(obj))

$(BUILD)/%.o: $(SOURCE)/%.c $(SOURCE)/spp.h
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS) $(CFLAGS)

.PHONY: clean
clean:
	rm $(TARGET)
	rm $(BUILD)/*
