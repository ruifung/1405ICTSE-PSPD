#ifndef COURTS_HEADER
#define COURTS_HEADER

#define COURT_TYPE_BATMINTON 0
#define COURT_TYPE_SQUASH 1
#define COURT_TYPE_TENNIS 2
#define COURT_TYPE_FUTSAL 3

#define COURT_NAME_BATMINTON "Batminton"
#define COURT_NAME_SQUASH "Squash"
#define COURT_NAME_TENNIS "Tennis"
#define COURT_NAME_FUTSAL "Futsal"

#define MAX_BOOKING_SIZE 5000

// Seconds per block (30 mins)
#define BLOCK_DURATION (30 * 60)

struct {
	FILE reservations;
	FILE customers;
} courts_config;

typedef struct {
	unsigned int id;
	unsigned int type;
	char label;
	int start;
	int end;
	float rate; // per block
} COURT;

typedef struct{
	unsigned int id;
	unsigned int customer_id;
	char court_id;
	time_t startTime;
	unsigned int duration;
} RESERVATION;

COURT courts[12];

void courts_init();
char* courts_typeIDStr(int type);
#endif // !COURTS_HEADER