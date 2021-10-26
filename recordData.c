/*
 * This is a template file to show all of the necessary components to add
 * to new data collection files that are created. 
*/

#include <sys/stat.h>// For the structures used to read the time
#include <time.h> // For recording info about time
#include "piBoard.h"
#include "RS485Devices.h"
#define BUFSIZE 1024
#define NUMCHAN 3

int main (int argc, char* argv[]){
	initializeBoard();
	int i,k,j,n,m;
	float volts;
	float psFrontDisplay, dmmReading, magnetometer;
	unsigned int periods;
	unsigned int valueADC[NUMCHAN];
	//char fileName[]="/home/pi/data.tsv";
	char fileName[]="/media/pi/C8D7-8FAC/data.tsv";
	char buffer[BUFSIZE];
	FILE* fp;

	if(argc > 1){
		k=atoi(argv[1]);
		if(k==-1){
			k=2000000000;
		}
	}else{
		k=1;
	}

	// Variables for recording the time. 
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	struct stat st = {0};

	fp=fopen(fileName,"a");
	if (!fp) {
		printf("Unable to open file: %s\n",fileName);
		exit(1);
	}
	fclose(fp);

	n=10;
	fp=fopen(fileName,"a");
	fprintf(fp,"***\n*\n*\n* NEW BLOCK OF DATA\n*\n*\n***\n");
	fclose(fp);
	for(i=0;i<k;i++){
		for(j=0;j<n;j++){
			fp=fopen(fileName,"a");
			getADC(4,&valueADC[0]);
			getADC(5,&valueADC[1]);
			getADC(6,&valueADC[2]);

			time(&rawtime);
			timeinfo=localtime(&rawtime);
			strftime(buffer,BUFSIZE,"%Y\t%m\t%d\t%H\t%M\t%S\t",timeinfo);
			fprintf(fp,"%s",buffer);
			for(m=0;m<NUMCHAN;m++)
			{
				volts=valueADC[m]*.0041+.0416;
				psFrontDisplay = ((float) valueADC[m] - 3.5) / 263.5;
				dmmReading = psFrontDisplay * 1.0454 + .0109;
				magnetometer = dmmReading * 1.0015 + .003168;
				printf("%f", magnetometer);
				fprintf(fp,"%d\t%f",valueADC[m],magnetometer);
				printf("%d\t%f",valueADC[m],magnetometer);
				if(m<NUMCHAN-1){
					fprintf(fp,"\t");
					printf("\t");
				}else{
					fprintf(fp,"\n");
					printf("\n");
				}
			}
			delay(500); // delay in microseconds
			fclose(fp);
		}
	}
	return 0 ;
}
