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

typedef struct BST_NODE_S BST_NODE;

typedef struct {
	unsigned int id;
	unsigned int type;
	char label;
	int startBlock;
	int endBlock;
	float rate; // per block
	BST_NODE *reservations;
} COURT;

typedef struct {
	unsigned int id;
	unsigned int customer_id;
	char court_id;
	unsigned int startTime;
	unsigned int blockCount;
} RESERVATION;

extern COURT courts[COURTS_COUNT];

void courts_init(char *);
char *courts_typeIDStr(int type);
int courts_timeBlock(time_t time);
time_t courts_blockTime(int block);

RESERVATION *courts_getReservation(unsigned int id);
unsigned int courts_countCourtReservations(char courtId);
void courts_getCourtReservations(char courtId, RESERVATION **dataArray, unsigned int maxCount);
RESERVATION *courts_getBlockReservation(char courtId, unsigned int block);
bool courts_checkBlockRange(unsigned int lowerBlock, unsigned int upperBlock);
RESERVATION *courts_addReservation(unsigned int customerId, char courtId, unsigned int startTime, unsigned int blockCount);
bool *courts_delReservation(RESERVATION *reservation);
#endif // !COURTS_HEADER