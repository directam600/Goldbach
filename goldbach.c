/* THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
     A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - JAMES DU */

#include <stdlib.h>
#include <stdio.h>

typedef struct _seg{
	int bits[256];
	struct _seg *next, *prev;
} seg;

#define BITSPERSEG (8*256*sizeof(int))

void sieveOfE(int maxNum);
seg* whichSeg(int j);
int whichInt(int j);
int whichBit(int j);
void setBit(seg *s, int i, int b);
void numOfPrimes(int N);
int goldBach(int k);

int l_prime, h_prime, l_seg, h_seg;
seg *ptr, *ptr2, *head, *temp;

int main(int argc, char *argv[])
{
	int i, N, k;
	int seg_num;  //number of segments needed
	
	if(argc == 2)
		sscanf(argv[1],"%d",&N);
	else
		scanf("%d",&N);
	seg_num = ((N - 1)/BITSPERSEG) + 1;

	ptr2 = calloc(1, sizeof *head);	//ptr2 allocates the initial segment and sets all bits to 0
	head = ptr2;	//head will remain at the initial allocated segment for the lifespan of the program
	ptr = ptr2;	//ptr and ptr2 will work together to allocate the required number of segments along with creating the doubly linked list

	for(i = 1; i < seg_num; i++){	//creation of doubly linked list
		ptr->next = calloc(1, sizeof *head);
		ptr = ptr->next;
		ptr->prev = ptr2;
		ptr2 = ptr;
	}

	sieveOfE(N);	//begins the sieve process by taking in initial user input N
	numOfPrimes(N);	//prints out the total number of primes less than N

	printf("Enter Even Numbers >5 for Goldbach Tests\n");
	
	ptr = head;	//ptr needs to be returned back to initial point in order to run the goldbach function
	while(!feof(stdin)){	//detects EOF
		scanf("%d", &k);	//user input k will be decomposed
		if(k % 2 == 0 && k > 5 && k <= N){	//l_prime is the prime # closest to but <k/2 and h_prime is the prime # closest to but >k/2
			printf(" Largest %d = %d + %d out of %d solutions\n", k, l_prime, h_prime, goldBach(k));
		}
		else continue;
	}

	free(ptr);
	return 0;
}

void sieveOfE(int maxNum){
	seg *s; //new seg pointer s
	int n, i, b;
	int p = 2;
	int test, test2;

	//neither 1 nor 2 are odd prime numbers, so mark them as non-prime
	ptr = head;
	ptr->bits[0] |= (1 << 0);
	ptr->bits[0] |= (1 << 1);

	//set each non-prime bit as 1, leaving the bits that are still 0 as prime
	while(p <= maxNum){
		for(n = 2 * p; n <= maxNum; n += p){
			s = whichSeg(n);
			i = whichInt(n);
			b = whichBit(n);
			setBit(s, i, b);
		}
		p++;
	}
}

seg* whichSeg(int j){
	seg *p;
	int count;
	int i = ((j - 1)/8192) + 1;

	p = head;
	for(count = 8192; count < j; count += 8192){
		p = p->next;
	}
	return p;
}

int whichInt(int j){
	int i;

	if(j < 8192)
		return (j - 1)/32;
	else
		i = ((j - 1) % 8192)/32;
	return i;
}

int whichBit(int j){
	return (j - 1) % 32;
}

void setBit(seg *s, int i, int b){
	ptr = s;
	ptr->bits[i] = ptr->bits[i] | (1 << b);
}

void numOfPrimes(int N){
	int b, c;
	int i = 0;
	int bitCounter = 0;
	int counter = 0;
	int pos = 0;
	ptr = head;
	
	//
	for(b = 0; b < ((N - 1)/32) + 1; b++){
		for(c = 0; c < 32; c++){	//once c hits 32, this indicates int needs to increment by 1
			if(bitCounter < N){
				unsigned int flag = 1;
				flag = flag << pos;
				pos++;
				bitCounter++;
				if(ptr->bits[i] & flag)
					continue;
				else
					counter++;
			}
		}
		pos = 0;
		i++;
		if(bitCounter % 8192 == 0){ //if bitCounter % 8192 == 0, move on to next node
			ptr = ptr->next;
			i = 0;
		}
	}
	printf("The number of odd primes less than or equal to %d is: %d\n", N, counter);
}

int goldBach(int k){
	int s = 0;
	int counter = 0;
	int p = 3;
	int p2 = k - p;
	l_seg = 1;
	h_seg = ((k - 1)/BITSPERSEG) + 1;
	int lowInt = 0;
	int lowBit = 2;
	int highInt = whichInt(p2);
	int highBit = whichBit(p2);
	ptr = head;
	ptr2 = whichSeg(p2);	//ptr2 will find the correct seg to start at

	while(p <= k/2){
		if(ptr->bits[lowInt] & (1 << lowBit))
			s = 1;	//if the lower # is not prime, s = 1 will prevent testing whether the higher # is prime and counter will not increment
		if(s == 0){	//lower # is prime.  Test if higher # is prime.  If it is, counter++.  If not, do nothing
			temp = ptr;	//ptr needs to be set at ptr2 location in order to find the number in the correct seg
			ptr = ptr2;
			if(ptr->bits[highInt] & (1 << highBit))
				ptr = temp;
			else{
				counter++;
				l_prime = (lowInt * 32) + (lowBit + 1);
				h_prime = (highInt * 32) + (highBit + 1);
				ptr = temp;
			}
		}

		s = 0;
		p += 2;
		lowBit += 2;
		highBit -= 2;		
		if(lowBit == 32){	//if lowBit hits 32, that means that the # of bits has increased past the # of bits per int and needs to be incremented
			if(lowInt == 255){
				ptr = ptr->next;
				lowInt = 0;
				lowBit = 0;
				l_seg++;
			}
			else{
				lowInt++;
				lowBit = 0;
			}
		}
		if(highBit == -2){	//if highBit hits -2 (it'll be 0 but highBit decreases by 2 again before the logic executes), int needs to be decremented
			if(highInt == 0){
				temp = ptr;
				ptr = ptr2;
				ptr = ptr->prev;
				ptr2 = ptr;
				ptr = temp;
				highInt = 255;
				highBit = 30;
				h_seg--;
			}
			else{
				highInt--;
				highBit = 30;
			}
		}
	}
	l_prime += (l_seg - 1) * 8192;
	h_prime += (h_seg - 1) * 8192;
	return counter;
}
