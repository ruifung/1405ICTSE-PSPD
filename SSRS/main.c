#include <stdio.h>
#include <Windows.h>

#include "simcon.h"
#include "menu_guest.h"

int main(int argc, char * args[]){
	SetConsoleTitle("Sport Space Rental System");
	menu_start(menu_guest());
}