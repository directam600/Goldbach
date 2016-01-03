/* James Du
   jdu7@emory.edu
   Collaboration Statement: I worked on this assignment alone, using only this semester's course materials.*/ 

 #include <stdio.h>
 #include <stdlib.h>
 #include "header.h"


void clearAll()
{

	int i;
	int j;
   	seg* p;

   	p = head;


	for (i = 0; i <= SIZE_OF_SEG; i++)
	{
		p->bits[i] = 0;	//sets all elements to 0
	}
}

void setBit( int n ) 
{
   int i = n/32;       // Index of prime[]
   int pos = n%32;     // Bit position in prime[i]
   int segments = n/256;
   int j;
   seg* p;

   p = head;
   for(j = 0; j < segments; j++)
	p = p->next;

   (*p).bits[i] = (*p).bits[i] | (1 << pos);

}

void sieveOfE(int n)
{
	clearAll();    //sets all elements of prime[MAX] to 0
	int j;
	int k = 2;	//starts at 2 because 0 and 1 are not prime
	while(k <= n)
	{
		for(j = 2*k; j <= n; j = j+k)
		{
			setBit(j);	 //All numbers set by setBit are not prime
		}
		k = k+1;
	}
}

int testBitIs0(int n)
{

	int j;
	int segments = n/256;
	int i = n/32;	
	int pos = n%32;	
	seg* p;

   	p = head;
   	for(j = 0; j < segments; j++)
	    p = p->next;

	unsigned int flag = 1;	
	flag = flag << pos;
	if((*p).bits[i] & flag)
		return 0;	//testBitIs0 returns 0 if the number is not prime
	else
		return 1;	//testBitIs0 returns 1 if the number is prime
}

int countPrimes(int n)
{
	int i;
	int count = 0;
	for(i = 2; i <= n; i++)
	{		
		if(testBitIs0(i))
			count = count + 1;	//if testBitIs0(i) returns true, then prime[i] is a prime number and the counter increases by 1
		else
			continue;
	}
	return count;	//returns number of prime numbers
}

void printPrimes(int n)
{
	int i;
	for(i = 2; i <= n; i++)
	{
		if(testBitIs0(i))
			printf("%d ", i);	//if testBitIs0(i) returns true, this function prints out the number stored at prime[i]
	}
	printf("\n");
}

void factor (int n)
{
	int k;
	int j;
	int z;
	int count;
	int original = n;
	if(n == 0 || n == 1)
		printf("non-factorable\n");
	else 
	{
		k = 2;
		j = k;
		while(n%k == 0)		//tests to see if 2 is a factor and prints 2 every time it factors
		{
			printf("%d\n", k);
			n = n/k;
		}
		z = 3;
		while(n%z == 0)		//tests to see if 3 is a factor and prints 3 every time it factors
		{
			printf("%d\n", z);
			n = n/z;
		}
		for(count = 5; count < original; count++)	//tests to see all other numbers and prints the number every time it factors
		{
			while(n%count == 0)
			{
				printf("%d\n", count);
				n = n/count;
			}
		}
	}
}
