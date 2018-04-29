CFLAGS = -Wall -Wextra
LFLAGS = -lgpio
CC = clang

.PHONY: clean

list_of_targets = runlamp turn_off_run_lamp \
	waitlamp turn_off_wait_lamp \
	turn_on_fault_lamp turn_off_fault_lamp \
	turn_on_traffic_lamp turn_off_traffic_lamp \
	flickertraffic flickerblocked

all: $(list_of_targets)

runlamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_off_run_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

waitlamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_off_wait_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_on_fault_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_off_fault_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_on_traffic_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

turn_off_traffic_lamp: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

flickertraffic: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

flickerblocked: $@.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

install: $(list_of_targets)
	cp -v $(list_of_targets) /usr/local/bin/

clean:
	rm -fv a.out core
	rm -fv $(list_of_targets)

