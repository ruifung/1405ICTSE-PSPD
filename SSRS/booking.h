#ifndef BOOKING_HEADER
#define BOOKING_HEADER

#define MAX_BOOKING_RESERATION 30

struct {
	char customer[65];
	RESERVATION reservations[MAX_BOOKING_RESERATION];
	size_t count;
}booking_cache;

#endif