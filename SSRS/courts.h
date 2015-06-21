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
#define CUSTOMER_NAME_SIZE 65

// Seconds per section
#define SPS (30 * 60)

typedef struct {
	unsigned int id;
	unsigned int type;
	char label;
	int start;
	int end;
	float price; // per section
} COURT;

typedef struct{
	unsigned int id;
	unsigned int customer_id;
	unsigned int court_id;
	unsigned int section;
	unsigned int duration;
} BOOK;

typedef struct{
	unsigned int id;
	char name[CUSTOMER_NAME_SIZE];
} CUSTOMER;

COURT courts[12];
#endif // !COURTS_HEADER