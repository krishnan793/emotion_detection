/*
 * sound.h
 *
 *  Created on: 11-Jul-2015
 *      Author: angel
 */

#ifndef SOUND_H_
#define SOUND_H_



#define BUF_SIZE 16000

#define RECORD_TIME 500000																// 0.5 seconds
#define WAVFILE_SAMPLES_PER_SECOND 44100
#define SAMPLE_SIZE (int)((float)WAVFILE_SAMPLES_PER_SECOND*(float)RECORD_TIME*0.000001)



//func protype
void handle_sigint (int);	//signal handle func
int play (char *argv);
void start_record(void *file_name2);

extern char buttons;

/* File name to save the recorded file */

struct file_name {
	char name[100];
};


#endif /* SOUND_H_ */
