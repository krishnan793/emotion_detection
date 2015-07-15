/*
 * ui.c
 *
 *  Created on: 11-Jul-2015
 *      Author: angel
 */


void cprintf(char *str,int color)
{
	printf("\e[%dm%s\e[39m",color,str);
}
