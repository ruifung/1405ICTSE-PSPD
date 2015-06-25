#ifndef COURTS_HEADER
#define COURTS_HEADER

#define COURTS_RESERVATION_FILE "reservations.dat"

#define COURT_TYPE_BATMINTON 0
#define COURT_TYPE_SQUASH 1
#define COURT_TYPE_TENNIS 2
#define COURT_TYPE_FUTSAL 3

#define COURT_NAME_BATMINTON "Batminton"
#define COURT_NAME_SQUASH "Squash"
#define COURT_NAME_TENNIS "Tennis"
#define COURT_NAME_FUTSAL "Futsal"

#define MAX_BOOKING_SIZE 5000
#define COURTS_COUNT 12

// Seconds per block (30 mins)
#define BLOCK_DURATION (30 * 60)

typedef unsigned int uint;
typedef struct BST_NODE_S BST_NODE;

typedef struct {
	uint id;
	uint type;
	char label;
	uint startBlock;
	uint endBlock;
	float rate; // per block
	BST_NODE *reservations;
} COURT;

typedef struct {
	uint id;
	uint ref_num;
	char court_id;
	uint startTime;
	uint blockCount;
} RESERVATION;

typedef struct {
	uint ref_num;
	 // of bill
} RECEIPT;
typedef struct RSVP_REF RSVP_REF;
typedef struct RSVP_LINK RSVP_LINK;
struct RSVP_REF {
	uint ref_num;
	char *customerName;
	time_t date;
	RSVP_LINK *list;
};
struct RSVP_LINK {
	RESERVATION *item;
	RSVP_LINK *prev;
	RSVP_LINK *next;
};

extern COURT courts[COURTS_COUNT];

void courts_init(char *);
char *courts_typeIDStr(int type);
int courts_timeBlock(time_t time);
time_t courts_blockTime(int block);

RESERVATION *courts_getReservation(uint id);
uint courts_countCourtReservations(char courtId);
void courts_getCourtReservations(char courtId, RESERVATION **dataArray, uint maxCount);
RESERVATION *courts_getBlockReservation(char courtId, uint block);
bool courts_checkBlockRange(uint lowerBlock, uint upperBlock);
RESERVATION *courts_addReservation(uint ref_num, char courtId, uint startTime, uint blockCount);
bool courts_delReservation(RESERVATION *reservation);

RSVP_REF courts_newRefNumt(char *custName, time_t date);
RSVP_REF courts_getRefItem(uint ref);
uint courts_countRefReservations(uint ref);
void courts_getRefReservations(uint ref, RESERVATION **dataArray, uint arraySize);
#endif // !COURTS_HEADER