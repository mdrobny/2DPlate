#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define N 10 			//plate size
#define MonteN 10000	//MonteCarlo trys amount

/**
*	1-north, 2-east, 3-south, 4-west
*/
int randDirection(){
	return rand()%5+1;
}

/**
*	start conditions - temperature at edges of plate
*/
void setEdgeTemp(double p[N][N]){
	int i,j;
	
	//init array
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			p[i][j] = 0.0;
		}
	}
	
	// for(j=20;j<80;j++)
	for(j=2;j<8;j++)
	// for(j=5;j<15;j++)
		p[N-1][j] = 100.0;
	
	//for(i=3;i<6;i++)
	//	p[i][0] = 100.0;
	
}

/**
*	recursive walk
*	ends after reaching one of edges
*/
double walk(double p[N][N],int x, int y){
	switch(randDirection()){
		case 1:	if(x-1 != 0) x--;
			else return p[0][y]; break;
		case 2:  if(y+1 != N-1) y++;
			else return p[x][N-1]; break;
		case 3:  if(x+1 != N-1) x++;
			else return p[N-1][y]; break;
		case 4:  if(y-1 != 0) y--;
			else return p[x][0]; break;
	}
	return walk(p,x,y);
}


/**
*	find temperature at given point using MonteCarlo method
*	x,y - cooridinates of point in array
*/
void temperature(double p[N][N],int x, int y){
	int n;
	double accu = 0.0;
	
	while(n++ < MonteN){
		accu += walk(p,x,y);
	}
	p[x][y] = accu/n;
	
	accu=0.0; n=0; //don't remove
	
	//printf("[%d %d] ",x,y);
}

/**
*	if out==1 shows plate at std output
*	save plate in file in format to generate pm3d map
*/
void showAndSavePlate(double p[N][N], int out){
	FILE* file = fopen("proj1.txt","w");
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++) if(out) printf("%3.2f ", p[i][j]);
		if(out) printf("\n");
	}
	
	//file output, used to create grid image
	for(i=0;i<N;i++){
		for(j=0;j<N;j++)
			fprintf(file,"%d %d %3.6f\n",i,j,p[i][j]);
		fprintf(file,"\n");
	}
	fclose(file);
}


/**
*	program implements problem of:
*		Finding temperature at a point inside a 2D plate using MonteCarlo method
*/

int main(int argc, char** argv)
{
	double p[N][N];
	
	setEdgeTemp(p);
	
	/*    main loop in plate array	*/
	int i,j;
	for(i=1;i<N-1;i++){
		for(j=1;j<N-1;j++){
			//printf("[%d %d] ",i,j); <--- BLAD, WYNIKI SIE ZERUJA
			temperature(p,i,j);
		}
	}
			
	showAndSavePlate(p,0);

	return 0;
}


