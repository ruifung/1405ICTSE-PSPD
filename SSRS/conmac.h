#ifndef COMMAC
#define COMMAC

#define FOREGROUND_WHITE (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define FOREGROUND_CYAN (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA (FOREGROUND_BLUE | FOREGROUND_RED)
#define FOREGROUND_YELLOW (FOREGROUND_GREEN | FOREGROUND_RED)

#define BACKGROUND_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define BACKGROUND_CYAN (BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BACKGROUND_MAGENTA (BACKGROUND_BLUE | BACKGROUND_RED)
#define BACKGROUND_YELLOW (BACKGROUND_GREEN | BACKGROUND_RED)

#define BORDER_LEFT (COMMON_LVB_GRID_LVERTICAL | FOREGROUND_WHITE)
#define BORDER_TOP (COMMON_LVB_GRID_HORIZONTAL | FOREGROUND_WHITE)
#define BORDER_RIGHT (COMMON_LVB_GRID_RVERTICAL | FOREGROUND_WHITE)
#define BORDER_BOTTOM (COMMON_LVB_UNDERSCORE | FOREGROUND_WHITE)
#define BORDER_TOPLEFT (COMMON_LVB_GRID_LVERTICAL | COMMON_LVB_GRID_HORIZONTAL | FOREGROUND_WHITE)
#define BORDER_TOPRIGHT (COMMON_LVB_GRID_RVERTICAL | COMMON_LVB_GRID_HORIZONTAL | FOREGROUND_WHITE)
#define BORDER_BOTTOMLEFT (COMMON_LVB_GRID_LVERTICAL | COMMON_LVB_UNDERSCORE | FOREGROUND_WHITE)
#define BORDER_BOTTOMRIGHT (COMMON_LVB_GRID_RVERTICAL | COMMON_LVB_UNDERSCORE | FOREGROUND_WHITE)

#define StdoutHandle GetStdHandle(STD_OUTPUT_HANDLE)
#define StdinHandle GetStdHandle(STD_INPUT_HANDLE)
#define SetConTextAttr(attr) SetConsoleTextAttribute(StdoutHandle, attr)
#define SetConCursorPos(x, y) SetConsoleCursorPosition(StdoutHandle, (COORD){x, y})
#define FlushConInput FlushConsoleInputBuffer(StdinHandle)
#define SetConCursorInfo(visible, size) {CONSOLE_CURSOR_INFO ci = {size, visible};SetConsoleCursorInfo(StdoutHandle, &ci);}
#define clrscr system("cls");

#endif