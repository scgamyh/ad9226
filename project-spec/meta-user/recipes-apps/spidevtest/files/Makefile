APP = spidevtest

# Add any other object files to this list below
APP_OBJS = spidev_test.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)
clean:
	rm -f $(APP) *.o
