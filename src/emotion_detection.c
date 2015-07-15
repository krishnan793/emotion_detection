/*
 * emotion_detection.c
 *
 *  Created on: 11-Jul-2015
 *      Author: angel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>

#include "sound.h"
#include "ui.h"

#define path "data/training_data/"
#define feature_path "data/features/"
#define config "config/emotion_detection.conf"

#define no_of_features 40
#define temp_name_wav "temp"
#define temp_feature_name "temp"

char buttons=0;
void record();
void cprintf(char *str,int color);
void format_4_em4gmm(char *file_name);
void exec_command(char *command, char *msg);

int classify();
int train();
struct file_name rec_file_name;

int main (int argc, char **argv)
{


	int options=999;
	char msg[100];

	while(options != 0)
	{
		system("clear");
		printf("=====================================\n");
		cprintf("\tEmotion Detection\n",GREEN);
		printf("=====================================");
		printf("\nEnter \n\t1 to train.");
		printf("\n\t2 to test.");
		printf("\n\t3 to clean.");
		printf("\n\t0 to quit.\n:]");
		scanf("%d",&options);
		switch (options)
		{
		case 1:
			train();
			break;
		case 2:
			classify();
			break;
		case 3:
			sprintf(msg,"rm %s*.gmm",feature_path);
			system(msg);
			sprintf(msg,"rm %s*.csv",feature_path);
			system(msg);
			sprintf(msg,"rm %s*.txt",feature_path);
			system(msg);
			break;
		case 0:
			printf("\n\rBye bye.\n");
			break;
		default:
			printf("\n\rwrong options");
			break;
		}
		getc(stdin);
	}


	return 0;

}

void record()
{
	buttons=0;
	sprintf(rec_file_name.name,"%s%s.wav",path,temp_name_wav);

	pthread_t thread1;
	cprintf("\n\rStarted Recoding:",RED);
	printf("\n\rPress q to stop");
	pthread_create( &thread1, NULL,(void *)start_record, (void *)&rec_file_name);
	system("stty raw");
	while(buttons!='q')
		scanf("%c",&buttons);
	system("stty cooked");
	pthread_join( thread1, NULL);
	cprintf("\n\r\033[FEnded Recording:\n",GREEN);

}

void format_4_em4gmm(char *file_name)
{

	FILE *fp1=fopen(file_name,"r+");
	char msg1[200];

	//printf("%s",file_name);

	//system("sleep 2");

	sprintf(msg1,"%s%s.txt",feature_path,temp_feature_name);

	FILE *fp2=fopen(msg1,"w");
	char tmp[200];
	int count = 0;

	/* Read no of features present in .csv file */
	char *line = NULL;
	size_t len=0;
	while ((getline(&line, &len, fp1)) != -1) {
	           count++;
	}

	fprintf(fp2,"%d %d\n",no_of_features,count);
	rewind(fp1);
	count = 0;

	/* End */

	/* convert 1.2*e^2 into 120 */

	while (fscanf(fp1, "%s", tmp) != EOF)
	{
		fprintf(fp2,"%0.4f ",atof(tmp));
		//printf("%0.4f",atof(tmp));
		if(++count==no_of_features)						// Line break at every feature
			{
				fprintf(fp2,"\n");
				count=0;
			}
	}

	/* End */

	fclose(fp1);
	fclose(fp2);

	sprintf(msg1,"octave --silent %splots.m %s &",feature_path,file_name);
	system(msg1);
}

int train()
{
	char msg[2000];
	char name[100];

	printf("\n\rEmotion: ");
	scanf("%s",name);
	scanf("%c",&buttons);
	record();

	int rply=0;
	sprintf(msg,"%s%s.wav",path,temp_name_wav);
	play(msg);
	printf("Proceed with the recorded sound?");
	scanf("%d",&rply);
	if(rply==0)
		return 1;

	sprintf(msg,"SMILExtract -C %s -I %s -O %s%s.csv -l 1",config,rec_file_name.name,feature_path,name);
	system(msg);
	sprintf(msg,"%s%s.csv",feature_path,name);
	format_4_em4gmm(msg);
	sprintf(msg,"gmmtrain -d %s%s.txt -m %s%s.gmm",feature_path,temp_feature_name,feature_path,name);
	system(msg);
	printf("\nEnter to continue...");
	while( getchar() != '\n' );		// Just to wait and show the output
	return 0;
}

int classify()
{
	char msg[2000];
	//printf("Recording");
	record();

	int rply=0;
	sprintf(msg,"%s%s.wav",path,temp_name_wav);
	play(msg);
	printf("Proceed with the recorded sound?");
	scanf("%d",&rply);
	if(rply==0)
		return 1;

	sprintf(msg,"%s%s.csv",feature_path,temp_feature_name);
	remove(msg);
	sprintf(msg,"SMILExtract -C %s -I %s -O %s%s.csv -l 1",config,rec_file_name.name,feature_path,temp_feature_name);
	system(msg);
	sprintf(msg,"%s%s.csv",feature_path,temp_feature_name);
	format_4_em4gmm(msg);

	DIR *dp;
	struct dirent *ep;
	dp = opendir (feature_path);
	char reply[100];

	float score=-999,score_tmp=0;
	char match[100];

	if (dp != NULL)
	{
		while ((ep = readdir (dp)))
		{
			if(strcmp((ep->d_name)+strlen(ep->d_name)-3,"gmm")==0)
			{
				//printf("\n%s\n",ep->d_name);
				sprintf(msg,"gmmclass -d %s%s.txt -m %s%s",feature_path,temp_feature_name,feature_path,ep->d_name);
				memset(reply,0,sizeof(reply));
				exec_command(msg, reply);
				score_tmp=atof(reply);
				if(score_tmp>=score)
				{
					score=score_tmp;
					strcpy(match,ep->d_name);
				}
				//system(msg);
			}
		}
		closedir (dp);

		printf("\n\t[");
		cprintf(match,GREEN);
		printf("]");
		printf("\n\nEnter to continue...");
		while( getchar() != '\n' );		// Just to wait and show the output
	}
	else
		perror ("Couldn't open the directory");

	return 0;
}

void exec_command(char *command, char *reply)
{
	FILE *fpipe;


	if (0 == (fpipe = (FILE*)popen(command, "r")))
	{
		perror("popen() failed.");
		exit(1);
	}

	while (fscanf(fpipe,"%s",reply)!=EOF)//fread(reply+strlen(reply), 1, 1, fpipe))
	{

	}

	//printf("%s", reply);
	pclose(fpipe);

}
