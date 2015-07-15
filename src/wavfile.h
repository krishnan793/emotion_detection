/*
 * wavfile.h
 *
 *  Created on: 11-Jul-2015
 *      Author: angel
 */

#ifndef WAVFILE_H_
#define WAVFILE_H_

#include <stdio.h>
#include <inttypes.h>

FILE * wavfile_open( const char *filename );
void wavfile_write( FILE *file, short data[], int length );
void wavfile_close( FILE * file );

#endif /* WAVFILE_H_ */
