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

typedef struct {
	unsigned int id;
	unsigned int type;
	char label;
	int start;
	int end;
	float rate;
} COURT;

#endif // !COURTS_HEADER