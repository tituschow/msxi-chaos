/*
  relay.c - Titus Chow

  This provides relay-related functions.

*/
#include "relay.h"

// open_relay(relay) opens the specified relay,
//   verifying it opened or returning true if unknown.
bool open_relay(const struct Relay *relay) {
	set_io_low(&relay->relay);
	return (get_relay_status(relay) == OPEN) || (get_relay_status(relay) == UNKNOWN);
}

// close_relay(relay) closes the specified relay,
//   verifying it closed or returning true if unknown.
bool close_relay(const struct Relay *relay) {
	set_io_high(&relay->relay);
	return (get_relay_status(relay) == CLOSED) || (get_relay_status(relay) == UNKNOWN);
}

// get_relay_status(relay) returns the relay's actual status
//   based on the status pin returning UNKNOWN if no status pin exists.
RelayState get_relay_status(const struct Relay *relay) {
	if (relay->status.port == NO_STATUS) {
		return UNKNOWN;
	} else {
		return (RelayState)get_io_state(&relay->status);
	}
}