/*
	Simple Console Interface Library
	
	https://github.com/0nepeop1e/SimCon
	
	Actualy this project is just for fun help to improve
	user experience of console application, it is a Visual
	Studio 2013 project, only for windows XD
*/
#ifndef SIMCON

#pragma comment(lib, "SimCon.lib")

#define SIMCON __declspec(dllimport) 

#define SCI_ALLOW_TAB 1
#define SCI_ALLOW_ESCAPE 2

#define SCI_STATE_SUCCESS 0
#define SCI_STATE_TAB_PRESSED 1
#define SCI_STATE_CANCELLED 2

#define MENU_MAX_ITEM 20

typedef struct{
	unsigned char f;
	unsigned char s;
} CHS;

#define MENU_MAX_ITEM 20

/*
	Pointer to a callback function which will be called when
	item was selected. This function have a parameter which
	is the index of the selected item and return a boolean
	which mean the should stay on the same menu or not.
*/
typedef _Bool (*SCI_ITEM_CALLBACK)(unsigned int);

/*
	This function will be called after the menu title draw
	and before the items draw.
*/
typedef void(*SCI_MENU_PRE)();

typedef struct{
	char name[31];
	SCI_ITEM_CALLBACK callback;
} SCI_ITEM;

typedef struct{
	char title[31];
	SCI_ITEM items[MENU_MAX_ITEM];
	SCI_MENU_PRE pre;
	unsigned int size;
} SCI_MENU;

/*
	scanf with very save method, same as scanf_s, but this clear input buffer before and after.
*/
SIMCON int scanf_vs(const char format[], ...);

/*
	Pause with print format, parameters pls refer to printf.
*/
SIMCON void _pause(const char format[], ...);

/*
	Pause the program, can press any key to continue.
*/
SIMCON void pause();

/*
	Get String
	* string      : the charater array where the input should be stored, a default value is allowed
	* size        : the size of character array
	* mask        : mask character, set 0 to disable
	* allowCancel : boolean, if true then user is allowed to cancel the input with escape
	* return      : boolean, return false when user cancel the input, otherwise true.
*/
SIMCON _Bool getstr(char string[], unsigned int size, char mask, _Bool allowCancel);

/*
	Get String with a input box
	* pos       : the position of the input box
	* string    : the charater array where the input should be stored, a default value is allowed
	* mask      : mask character, set 0 to disable
	* size      : the size of character array
	* width     : the size of the input box
	* attribute : attribute of the input box(SCI_ALLOW_ESCAPE & SCI_ALLOW_TAB)
	* return    : return an integer which represent the satus of input
*/
SIMCON int getstr_ex(COORD pos, char string[], char mask, UINT size, UINT width, char attribute);

/*
	Get Integer
	* digit       : how many digit can be input(take care from overflow)
	* allowCancel : boolean, if true then user is allowed to cancel the input with escape
	* return      : return a positive integer after input, return negative when cancel
*/
SIMCON int getint(unsigned int digit, _Bool allowCancel);

/*
	Get character(getch) with a safer method
	* return : CHS structure(f = first returned character, s = second returned character)
*/
SIMCON CHS getch_s();

/*
	Prompt a yes no confirmation to user with format, parameters pls refer to printf.
	* return : boolean, true if yes
*/
SIMCON _Bool confirm(const char format[], ...);

/*
	Get Date
	* result     : pointer to a tm structure where the result stored, a default value is allowed.
	* allowCancel: boolean, if true then user is allowed to cancel the input with escape
	* return     : boolean, return false when user cancel the input, otherwise true.
*/
SIMCON _Bool getdate(struct tm *result, _Bool allowCancel);

/*
	Start the menu interface. Wont have any effect after menu started.
	* menu: pointer to the menu.
*/
SIMCON void menu_start(SCI_MENU *menu);

/*
	Switch from one menu to another.
	* menu : pointer to the menu, the menu will end when NULL.
*/
SIMCON void menu_switch(SCI_MENU *menu);

/*
	Create an empty menu.
	* title : title of the menu, maximum 30 character.
*/
SIMCON SCI_MENU menu_create(const char title[]);

/*
	Add an item to a menu.
	* menu : pointer to the menu.
	* item : the item which to be added.
*/
SIMCON _Bool menu_add_item(SCI_MENU *menu, SCI_ITEM item);

#endif
