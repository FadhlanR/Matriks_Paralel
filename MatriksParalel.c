/* Author : Fadhlan Ridhwanallah
   Date : 11 Maret 2017
   */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define UKURAN 400

int A[UKURAN][UKURAN];
int B[UKURAN][UKURAN];
int C[UKURAN][UKURAN]; //matriks hasil kali
int TEMP[UKURAN][UKURAN];
int col_s,col_f,row_s,row_f,akar;
pthread_mutex_t cross_mutex;

void PrintMatriks(int M[UKURAN][UKURAN]);
void CopyMatriks(int M1[UKURAN][UKURAN],int M2[UKURAN][UKURAN]);
void InsertMatriks(int M);
void GeserBaris1();
void GeserBaris2();
void GeserKolom1();
void GeserKolom2();
void *PerkalianMatriks(void *threadid);
void PenjumlahanMatriks(int M1[UKURAN][UKURAN],int M2[UKURAN][UKURAN]);

int main(int argc, char *argv[])
{
	/*int a[UKURAN][UKURAN] = {
	{1,2,3,4,5,6,7,8,9},
	{9,8,7,6,5,4,3,2,1},
	{5,6,7,8,9,1,2,3,4},
	{4,5,6,7,8,9,1,2,3},
	{1,2,3,7,8,9,4,5,6},
	{4,5,6,1,2,3,7,8,9},
	{1,2,4,3,5,6,8,7,9},
	{9,7,1,2,3,4,5,6,8},
	{3,2,1,6,5,4,9,8,7}
	};
	int b[UKURAN][UKURAN] = {
	{9,8,7,6,5,4,3,2,1},
	{5,6,7,8,9,1,2,3,4},
	{1,2,3,4,5,6,7,8,9},
	{4,5,6,1,2,3,7,8,9},
	{1,2,3,7,8,9,4,5,6},
	{4,5,6,7,8,9,1,2,3},
	{3,2,1,6,5,4,9,8,7},
	{1,2,4,3,5,6,8,7,9},
	{9,7,1,2,3,4,5,6,8}
	};
	CopyMatriks(A,a);
	CopyMatriks(B,b);*/
	
	//Deklarasi dan Assignment
	clock_t start, end;
	double cpu_time;
	pthread_t threads[UKURAN];
	int i,j,rc;
	akar = (int) sqrt((double) UKURAN);
	
	for(i=0;i<UKURAN;i++){
		for(j=0;j<UKURAN;j++){
			C[i][j] = 0;
		}
	}
	
	for(i=0;i<UKURAN;i++){
		for(j=0;j<UKURAN;j++){
			A[i][j] = rand()%10;
			B[i][j] = rand()%10;
		}
	}
	/*printf("Matriks A: \n");
	PrintMatriks(A);
	printf("\nMatriks B: \n");
	PrintMatriks(B);*/
	//printf("\n");
	start = clock();
	//Iterasi 1
	GeserKolom1();
	GeserBaris1();
	/*printf("Iterasi 1 Matriks A: \n");
	PrintMatriks(A);
	printf("\nIterasi 1 Matriks B: \n");
	PrintMatriks(B);*/
	//printf("\n");
	row_s = 0;
	row_f = akar - 1;
	col_s = 0;
	col_f = akar - 1;
	
	for(i=0;i<UKURAN;i++){
		rc = pthread_create(&threads[i], NULL, PerkalianMatriks, (void *) i);
		if(rc){
			printf("Eror in: %d\n",i);
			exit(-1);
		}
	}
	
	for(j=0;j<UKURAN;j++){
		pthread_join(threads[j],NULL);	
	}
	//Iterasi Next
	for(i=0;i<akar-1;i++){
		PerkalianNextIterasi();
		/*printf("Iterasi %d Matriks A: \n",i+2);
		PrintMatriks(A);
		printf("\nIterasi %d Matriks B: \n",i+2);
		PrintMatriks(B);*/
		//printf("\n");
	}
	
	printf("Waktu Hasil:\n");
	end = clock();
	cpu_time = (end - start) / CLOCKS_PER_SEC;
	printf("%f", cpu_time);
	//PrintMatriks(C);
	printf("\n");
	pthread_exit(NULL);
}

void PerkalianNextIterasi(){
	int i,j,rc;
	pthread_t threads[UKURAN];
	GeserKolom2();
	GeserBaris2();
	
	row_s = 0;
	row_f = akar - 1;
	col_s = 0;
	col_f = akar - 1;
	
	for(i=0;i<UKURAN;i++){
		rc = pthread_create(&threads[i], NULL, PerkalianMatriks, (void *) i);
		if(rc){
			printf("Eror in: %d\n",i);
			exit(-1);
		}
	}
	
	
	for(j=0;j<UKURAN;j++){
		pthread_join(threads[j],NULL);	
	}
}

void PrintMatriks(int M[UKURAN][UKURAN]){
	int i,j;
	i = 0;
	j = 0;
	for(i=0;i<UKURAN;i++){
		for(j=0;j<UKURAN;j++){
			printf("%d ",M[i][j]);
		}
		printf("\n");
	}
}

void CopyMatriks(int M1[UKURAN][UKURAN],int M2[UKURAN][UKURAN]){
	int i,j;
	i = 0;
	j = 0;
	for(i=0;i<UKURAN;i++){
		for(j=0;j<UKURAN;j++){
			M1[i][j] = M2[i][j];
		}
	}
}

void GeserKolom1()
{	
	int r, c, i, j;
	r = 0;
	int temp[akar][akar];
	while(r<akar){
		c = 0;
		while(c<r*akar){
			if((c%akar)==0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						temp[i][j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if (c < akar - 1 && r != 0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][(((c%akar)-1)*akar)+j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if (r != 0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][(((c%akar)-1)*akar)+j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][((c%akar)*akar)+j] = temp[i][j];
					}
				}
			}
			c++;
		}
		r++;
	}			
}

void GeserKolom2()
{	
	int r, c, i, j;
	r = 0;
	int temp[akar][akar];
	while(r<akar){
		c = 0;
		while(c<akar){
			if((c%akar)==0){		
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						temp[i][j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if(c < akar - 1){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][(((c%akar)-1)*akar)+j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else{
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][(((c%akar)-1)*akar)+j] = A[(r*akar)+i][((c%akar)*akar)+j];
					}
				}
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						A[(r*akar)+i][((c%akar)*akar)+j] = temp[i][j];
					}
				}
			}
			c++;
		}
		r++;
	}			
}


void GeserBaris1()
{	
	int r, c, i, j;
	c = 0;
	int temp[akar][akar];
	while(c<akar){
		r = 0;
		while(r<c*akar){
			if((r%akar)==0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						temp[i][j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if (c !=0 && r < akar -1){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[(((r%akar)-1)*akar)+i][((c%akar)*akar)+j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if (c != 0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[(((r%akar)-1)*akar)+i][((c%akar)*akar)+j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[((r%akar)*akar)+i][((c%akar)*akar)+j] = temp[i][j];
					}
				}
			}
			r++;
		}
		c++;
	}			
}

void GeserBaris2()
{	
	int r, c, i, j;
	c = 0;
	int temp[akar][akar];
	while(c<akar){
		r = 0;
		while(r<akar){
			if((r%akar)==0){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						temp[i][j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else if(r < akar - 1){
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[(((r%akar)-1)*akar)+i][((c%akar)*akar)+j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
			}
			else{
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[(((r%akar)-1)*akar)+i][((c%akar)*akar)+j] = B[((r%akar)*akar)+i][((c%akar)*akar)+j];
					}
				}
				for(i=0;i<akar;i++){
					for(j=0;j<akar;j++){
						B[((r%akar)*akar)+i][((c%akar)*akar)+j] = temp[i][j];
					}
				}
			}
			r++;
		}
		c++;
	}			
}

void *PerkalianMatriks(void *threadid){
	pthread_mutex_lock(&cross_mutex);
	int i,j,k;
	for(i=row_s;i<=row_f;i++){
		for(j=col_s;j<=col_f;j++){
			for(k=0;k<akar;k++){
				C[i][j] = C[i][j]+(A[i][k+col_s] * B[k+row_s][j]);
			}
		}
	}
	
	if(col_f < (UKURAN - 1)){
		col_s = col_f + 1;
		col_f = col_f + akar;
	}
	else if(row_f <= (UKURAN - 1)){
		row_s = row_f + 1;
		row_f = row_f + akar;
		col_s = 0;
		col_f = akar - 1;
	}
	pthread_mutex_unlock(&cross_mutex);
	pthread_exit(NULL);
}






