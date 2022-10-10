#include <stdio.h> // print, fgets function
#include <stdlib.h> // exit, atoi function
#include <string.h> //strlen function
#include <math.h> // sqrt function
#include <algorithm>    // std::min
#include <limits.h> // INT_MAX
#include <time.h>


int calculateTourDistance(int *tour);//calculate the distance of a tour
void imprimir_matriz();
void imprimir_tour(int *tour);
int best_city(int indice, int *tour);

int **distanceMatrix, size=-1, pos=0;

int main(const int argc, const char **inputFile){
	double *x, *y;
	char type[10]="";

	if(argc < 2){ //verify if an argument was passed (the first argument in C is the name of the executable)
		fprintf(stderr,"use: tspMatrix <tsp file>\n\n");
		exit(1);
	}
	char s[500];
	FILE *stream = fopen(inputFile[1], "r"); //read file (read-only permission)
	if( stream == NULL ){ // if the file was not read, error
		fprintf(stderr,"\nFail to Open tsp File!!\n");
		exit(1);
	}
	while(fgets(s,500,stream)){ //read every line (not larger than 500 chars) of the input
		if(strlen(s)-1>0) //remove the last break line of the line
			s[strlen(s)-1]='\0';
		if(('\r'==s[strlen(s)-1]))//in some files there is a carriage return at the end, don't know why. This command removes it
			s[strlen(s)-1]=0;

		char* value1 = strtok(s," "); //creating sub-strings separated by space
		char* value2 = strtok(NULL," ");
		char* value3 = strtok(NULL," ");

		if(!strcmp(value1, "EDGE_WEIGHT_TYPE")){ //verify if the instance is of type EUC_2D, ATT or CEIL_2D, only the calculation for these types were implemented
			if(strcmp(value3, "EUC_2D") && strcmp(value3, "ATT") && strcmp(value3, "CEIL_2D")){
				fprintf(stderr,"\nERROR! tsp file is not of type EUC_2D, ATT or CEIL_2D aborting!!\n");
				exit(1);
			}else
				strcpy(type,value3);
		}

		if(!strcmp(value1, "TYPE") && (strcmp(value3, "TSP")) ){ //verify if the instance is of type TSP, the other types will not be considered
			fprintf(stderr,"\nERROR! tsp file is not of type TSP, aborting!!\n");
			exit(1);
		}

		if(!strcmp(value1, "DIMENSION")){//read the dimension from the header and allocate memory for the cities
			size = atoi(value3);
			distanceMatrix=(int**)malloc(size * sizeof(int*));
			x=(double*)malloc(size * sizeof(double*));
			y=(double*)malloc(size * sizeof(double*));

			for (int i=0;i<size;i++)
				distanceMatrix[i] = (int*)malloc(size * sizeof(int));
		}

		if(atoi(value1)){//if the first substring is a number, the list of cities started
			if(size==-1){//if the size was not set, it was not in the header, error
				fprintf(stderr,"\nERROR! Dimension not set in file header!!\n");
				exit(1);
			}
			x[pos]=atof(value2);//storing the values from the file in the arrays
			y[pos]=atof(value3);
			pos++;
		}
	}

	if(!strcmp(type, "EUC_2D")){
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double dist=sqrt(xd*xd+yd*yd);
				distanceMatrix[i][j]=(int)(dist+0.5);//calculating the euclidean distance, rounding to int and storing in the distance matrix
			}
		}
	}else if(!strcmp(type, "CEIL_2D")){
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double dist=sqrt(xd*xd+yd*yd);
				distanceMatrix[i][j]=(int)(dist+0.000000001);
			}
		}
	}else if(!strcmp(type, "ATT")){
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				double xd = x[i]-x[j];
				double yd = y[i]-y[j];
				double rij=sqrt((xd*xd+yd*yd)/10.0);
				double tij=(int)(rij+0.5);
				if(tij<rij)
					distanceMatrix[i][j]=tij+1;
				else
					distanceMatrix[i][j]=tij;
			}
		}
	}

	int tour[size], i, j, menor, num_city = 0;
	for(int i=1; i<size; i++)
		tour[i]= -1;
	// tour[0] = 0;

	// imprimir_matriz();
	// num_city = 0;
	for(j=0;j<size;j++){
		num_city = best_city(num_city, tour);
		tour[j] = num_city;
	}

	imprimir_tour(tour);
	printf("\n distancia da gulosa: %d \n", calculateTourDistance(tour));

}

int calculateTourDistance(int *tour){
	int dist=0;
	for(int i=0; i<size-1; i++){
		dist+=distanceMatrix[tour[i]][tour[i+1]];
	}
	dist+=distanceMatrix[tour[size-1]][tour[0]];
	return dist;
}

void imprimir_matriz(){
	int i, j;
	for(i=0;i<size;i++){
        //printf("%d |", i);
		for(j=0;j<size;j++){
			printf("%d ", distanceMatrix[i][j]);
		}
	printf("\n\n");
	}
}

int best_city(int indice, int *tour){
	int i, j, num_city, menor = INT_MAX, flag;

	for(i=0;i<size;i++){
		flag = 0;
		for(j=0;j<size;j++){
			if(tour[j] == i){
				flag = 1;
				// printf("\ntour = %d cidade = %d\n", tour[j], i);
			}
		}
		if(flag == 0){
			if(distanceMatrix[indice][i] < menor){
				menor = distanceMatrix[indice][i];
				num_city = i;
				// printf("\nmenor = %d cidade = %d\n", menor, num_city);
			}
		}
	}
	return num_city;
}

void imprimir_tour(int *tour){
	int i;
	printf("\n");
	for(i=0;i<size;i++)
		printf("%d ", tour[i]);
}
