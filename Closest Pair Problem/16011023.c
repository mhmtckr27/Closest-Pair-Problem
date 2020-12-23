#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define COLNUMBER 2
#define TRUE 1
#define FALSE 0

#define RED   "\x1B[31;1m"	//kirmizi yazi
#define GRN   "\x1B[32;1m"	//yesil	
#define YEL   "\x1B[33;1m"	//sari
#define BLU   "\x1B[34;1m"	//mavi
#define MAG   "\x1B[35;1m"	//magenta
#define CYN   "\x1B[36;1m"	//cyan
#define WHT   "\x1B[37;1m"	//beyaz
#define GRY "\x1B[30;1m"	//gri

typedef struct
{
	int x;
	int y;
}Point;

typedef struct
{
	Point p1;
	Point p2;
	double d;
}ClosestPair;

Point* GetPoints(int);
void QuickSort(Point*, int, int, char);
void PrintArray(Point*, int);
double bruteForceClosestPair(Point*, ClosestPair*, int);
double minOfTwoDouble(double, double);
double distanceBetweenTwoPoints(Point, Point, ClosestPair*);
void findMinInSubarrays(Point *, ClosestPair*, ClosestPair*, ClosestPair*, int);

int main()
{
	FILE* fp = NULL;	//dosya pointer�
	Point* points;	//noktalar dizisi
	char line[32];	//her sat�r� alaca��m dizi
	char fileName[32];	//dosya ad�
	char *token;	//strtok fonksiyonunda kullan�lacak
	ClosestPair * closestPair = (ClosestPair *) calloc (1, sizeof(ClosestPair));		//en yak�n nokta i�in struct de�i�keni
	ClosestPair * closestPairLeft = (ClosestPair *) calloc (1, sizeof(ClosestPair));	//sol
	ClosestPair * closestPairRight = (ClosestPair *) calloc (1, sizeof(ClosestPair));// ve sa� en yak�n noktalar i�in de�i�kenler
	int n = 0;	//sat�r say�s�n� tutacak
	int i;	//d�ng� de�i�keni

	system("COLOR F5");
	(*closestPair).d = INT_MAX;
	(*closestPair).p1.x = 0;
	(*closestPair).p1.y = 0;
	(*closestPair).p2.x = 0;
	(*closestPair).p2.y = 0;
	(*closestPairLeft).d = INT_MAX;
	(*closestPairRight).d = INT_MAX;
	printf("File name: ");
	scanf("%s", fileName);

	if((fp = fopen(fileName, "r")) == 0)
	{
		exit(-2);
	}
	while(fgets(line, 32, fp))
	{
		++n;
	}
	points = (Point*) calloc(n,sizeof(Point));
	fseek(fp,0,SEEK_SET);
	for (i = 0; i < n; ++i)
	{
		fgets(line, 32, fp);
		token = strtok(line," \n");
		points[i].x = atoi(token);
		token = strtok(NULL," \n");
		points[i].y = atoi(token);
	}
	printf(GRY"\n>Printing given array..\nx \t y\n----------------------------------------\n"RED);
	PrintArray(points, n);

	//points = GetPoints(n);
	//createPointsArray(points,n);
	QuickSort(points, 0, n-1, 'x');
	findMinInSubarrays(points, closestPair, closestPairLeft, closestPairRight, n);

	printf(GRY"\n>Printing sorted array..\nx \t y\n----------------------------------------\n"MAG);
	PrintArray(points, n);

	printf(GRY"\n>Printing closest points..\nPoint1 \t Point2\t Distance\n----------------------------------------\n"BLU);
	printf("%d,%d \t %d,%d \t %f\n\n"GRY, (*closestPair).p1.x, (*closestPair).p1.y, (*closestPair).p2.x, (*closestPair).p2.y, (*closestPair).d);
	system("PAUSE");
	return 0;
}

//orta �izgiye yak�n noktalar�n minimum mesafesini bulur.
double findMinInMidLine(Point* pointsAroundMidLine, int j, double minOfLeftRightD, ClosestPair *closestPair) 
{ 
    double min = minOfLeftRightD;
	int i;
	int k;
	QuickSort(pointsAroundMidLine, 0, j-1, 'y');
    for (i = 0; i < j; ++i) 
	{
		for (k = i+1; k < j && (pointsAroundMidLine[k].y - pointsAroundMidLine[i].y) < min; ++k) 
		{
			if (distanceBetweenTwoPoints(pointsAroundMidLine[i],pointsAroundMidLine[k], closestPair) < min) 
			{
				min = distanceBetweenTwoPoints(pointsAroundMidLine[i], pointsAroundMidLine[k], closestPair);
			}
		}
	}
    return min; 
} 


//sol ve sa� dizilerde minimum mesafeyi bulur.
void findMinInSubarrays(Point * points, ClosestPair* closestPair,ClosestPair* closestPairLeft,ClosestPair* closestPairRight, int n)
{
	Point* pointsAroundMidLine;
	double minOfLeftRightD;
	int i;
	int j;
	if((n >= 2) && (n <= 3))
	{
		bruteForceClosestPair(points, closestPair, n);
		return;
	}
	findMinInSubarrays(points, closestPairLeft, closestPair, closestPairRight, n/2);
	findMinInSubarrays(points + n/2, closestPairRight, closestPair, closestPairLeft, n-n/2);
	minOfLeftRightD = minOfTwoDouble((*closestPair).d, (*closestPairRight).d);

	pointsAroundMidLine = (Point*) calloc (n, sizeof(Point));
	j = 0;
	for (i = 0; i < n; ++i)
	{
		if (abs(points[i].x - points[n/2].x) < minOfLeftRightD)
		{
            pointsAroundMidLine[j] = points[i];
			j++; 
		}
	}

	findMinInMidLine(pointsAroundMidLine, j, minOfLeftRightD, closestPair);
}


//iki doubledan k���k olan�n� d�ner.
double minOfTwoDouble(double a, double b)
{
	return (a > b) ? b : a;
}

//iki nokta aras� mesafeyi bulup, closestpair.d sinden k���kse g�ncelliyor.
double distanceBetweenTwoPoints(Point p1, Point p2, ClosestPair* closestPair)
{
	double dist = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	if(dist < (*closestPair).d)
	{
		(*closestPair).p1 = p1;
		(*closestPair).d = dist;
		(*closestPair).p2 = p2;
	}
	return dist;
}


//brute force kar��la�t�rma fonksiyonu
double bruteForceClosestPair(Point* points, ClosestPair* closestPair, int n)
{
	int i;//d�ng� de�i�keni
	int j;//d�ng� de�i�keni
	for (i = 0; i < n; ++i)
	{
		for (j = i + 1; j < n; ++j)
		{
			if(distanceBetweenTwoPoints(points[i], points[j], closestPair) < (*closestPair).d)
			{
				(*closestPair).p1 = points[i];
				(*closestPair).p2 = points[j];
				(*closestPair).d = distanceBetweenTwoPoints(points[i], points[j], closestPair);
			}
		}
	}
	return (*closestPair).d;
}

//diziyi yazd�rmak i�in
void PrintArray(Point* points, int n)
{
	int i;
	for(i = 0; i < n; ++i)
	{
		printf("%d \t %d\n", points[i].x,points[i].y);
	}
}

//yer de�i�me i�lemini yapacak
void Swap(int* a, int* b) 
{ 
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
}


//quicksort i�in b�lme(divide) i�lemi
int Partition (Point* arr, int lo, int hi, char whichAxis) 
{ 
	int j;
    int pivot = whichAxis=='x' ? arr[hi].x : arr[hi].y;
	int valToComparePivot;
    int i = (lo - 1);  
  
    for (j = lo; j <= hi- 1; j++) 
    { 
		valToComparePivot = whichAxis=='x' ? arr[j].x : arr[j].y;
        if (valToComparePivot < pivot) 
        { 
            i++;
            Swap(&arr[i].x, &arr[j].x); 
            Swap(&arr[i].y, &arr[j].y); 
        } 
    } 
    Swap(&arr[i + 1].x, &arr[hi].x); 
    Swap(&arr[i + 1].y, &arr[hi].y); 
    return (i + 1); 
} 

void QuickSort(Point* arr, int lo, int hi, char whichAxis) 
{ 
	int pi;//pivot
    if (lo < hi) 
    { 
        pi = Partition(arr, lo, hi, whichAxis); 
  
        QuickSort(arr, lo, pi - 1, whichAxis); 
        QuickSort(arr, pi + 1, hi, whichAxis); 
    } 
} 
