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

typedef struct RSVP_REF RSVP_REF;
typedef struct RSVP_LINK RSVP_LINK;
struct RSVP_REF {
	uint ref_num;
	time_t date;
	uint staff_id;
	char *customerName;
	RSVP_LINK *list;
};
struct RSVP_LINK {
	RESERVATION *item;
	RSVP_LINK *prev;
	RSVP_LINK *next;
};

extern COURT courts[COURTS_COUNT];

_Bool courts_load();
_Bool courts_save();

uint courts_getFirstBlockSlot(uint block, char courtId);

void courts_init(char *);
char *courts_typeIDStr(uint type);
int courts_timeBlock(time_t time);
time_t courts_blockTime(int block);

RESERVATION *courts_getReservation(uint id);
uint courts_countCourtReservations(char courtId);
void courts_getCourtReservations(char courtId, RESERVATION **dataArray, uint maxCount);
RESERVATION *courts_getBlockReservation(char courtId, uint block);
bool courts_isBlockRangeEmpty(char courtId, uint lowerBlock, uint upperBlock);
RESERVATION *courts_addReservation(uint ref_num, char courtId, uint startTime, uint blockCount);
bool courts_delReservation(RESERVATION *reservation);

//NOTE: ref num will never be 0.
//THIS FUNCTION WILL RETURN A ZERO-ED RSVP_REF ON FAILURE.
RSVP_REF *courts_newRef(char *custName, time_t date, uint staff_id);
void courts_delRef(uint ref);
RSVP_REF *courts_getRefItem(uint ref);
uint courts_countRefReservations(uint ref);

//Printing functions
bool courts_printRefReceipt(FILE*, uint);
bool courts_printRefDetails(FILE*, uint);
#endif // !COURTS_HEADER