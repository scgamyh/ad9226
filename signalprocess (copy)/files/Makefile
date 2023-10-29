APP = signalprocess

# Add any other object files to this list below
APP_OBJS = signalprocess.o cqueue.o threadwork.o libaxidma.o util.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS) -lm -lpthread
clean:
	rm -f $(APP) *.o
