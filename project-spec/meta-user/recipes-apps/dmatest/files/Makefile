APP = dmatest
# Add any other object files to this list below
APP_OBJS = axidma_transfer.o util.o libaxidma.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)
clean:
	rm -f $(APP) *.o
