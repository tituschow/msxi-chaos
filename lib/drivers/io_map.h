/*
  io_map.h - Titus Chow

  This provides a structure to represent port/pin mappings and related functions.

*/
#ifndef IO_MAP_H_
#define IO_MAP_H_

struct IOMap {
	int port;
	int pins;
};

typedef enum {
	LOW,
	HIGH
} IOState;

typedef enum {
	IN,
	OUT
} IODirection;

// set_io_dir(map, direction) sets the specified pin(s)' IO direction.
void set_io_dir(const struct IOMap *map, IODirection direction);

// set_io_high(map) sets the specified pin(s) to output high.
// requires: map's IO direction has been set to OUT.
void set_io_high(const struct IOMap *map);

// set_io_low(map) sets the specified pin(s) to output low.
// requires: map's IO direction has been set to OUT.
void set_io_low(const struct IOMap *map);

// toggle_io(map) toggles the output state of the specified pin(s).
// requires: map's IO direction has been set to OUT.
void toggle_io(const struct IOMap *map);

// get_io_state(map) returns the state of the specified pin.
IOState get_io_state(const struct IOMap *map);

#endif