#include <stdio.h>
#include <stdlib.h>
void merge(int*,int*,int*,int);
void merge_sort(int*,int);

int main(int argc, char *argv[]) {
	/*
	variables
		- size: keeps array size
		- i ve j: iterators
		- min elemanlar aras�ndaki en k���k fark� saklamak i�in
		- index aranan fark� en k���k elemanlardan ilkinin konumunu saklamak i�in
	kullan�ld�.
	*/
	int size,i,j,min,index;
	// Dizinin boyutu kullan�c�dan al�nd�.
	printf("Dizinin boyutunu giriniz: ");
	scanf("%d",&size);
	//2 elemandan daha k���k dizide en yak�n 2 eleman olamayaca��ndan programdan ��k�ld�.
	if(size<2){
		printf("Dizi boyutu en az 2 olabilir.\n");
		return 0;
	}
	// Al�nan boyuttaki dizi olu�turuldu.
	int A[size];
	
	// Dizinin elemanlar� kullan�c�dan input olarak al�nd�.
	printf("Dizinin elemanlarini giriniz:\n");
	for(i=0;i<size;i++){
		scanf("%d",&A[i]);
	}
	// Dizinin elemanlar� merge sort ile s�raland�. O(nlogn) karma��kl�k
	merge_sort(A,size);
	// Arda��k iki eleman�n fark� en k���k olan eleman al�nd�. O(n) karma��kl�k
	min = A[1]-A[0];
	index = 0;
	for(i=1;i<size-1;i++){
		if(A[i+1]-A[i]<min){
			min = A[i+1]-A[i];
			index = i;
		}
	}
	printf("En yakin 2 sayi: %d ve %d",A[index],A[index+1]);
	return 0;
}

void merge(int* A, int* B, int* C,int size){
	/*
	De�i�kenler
		- i B dizisinin, j C dizisinin, k A dizisinin indisini saklamak i�in 
		- B ve C birle�tirilecek dizileri A ise sonucun kaydedilece�i diziyi ifade etmek i�in
		- size dizi boyutu i�in kullan�ld�.
	kullan�ld�.
	*/
	int i=0, j=0, k=0;
	//Dizinin her iki par�as� s�ras� ile A dizisinde birle�tirildi.
	while(i<size/2 && j<size-size/2){
		if(B[i]<=C[j]){
			A[k]=B[i++];
		}
		else{
			A[k]=C[j++];
		}
		k++;
	}
	//Eleman� kalan dizideki elemanlar da diziye eklendi.
	while(i<size/2){
		A[k]=B[i++];
		k++;
	}
	while(j<size-size/2){
		A[k]=C[j++];
		k++;
	}
}


// Merge sort algoritmas�
void merge_sort(int* A,int size){
	//Boyutu 1 oluncaya kadar dizi alt par�alara b�l�nd�.
	if(size>1){
		/*
		De�i�kenler
			- size: dizi boyutu i�in
			- i ve j: iterasyon i�in
			- B dizinin ilk yar�s�n� C ise son yar�s�n� saklamak i�in
		kullan�ld�.
		*/
		int B[size/2],C[size-size/2],i,j;
		for(i=0;i<size/2;i++){
			B[i]=A[i];
		}
		for(i=0,j=size/2;j<size;i++,j++){
			C[i]=A[j];
		}
		// Dizinin k���k alt par�as� ve b�y�k alt par�as� rek�rsif olarak par�aland�.
		merge_sort(B,size/2);
		merge_sort(C,size-size/2);
		// S�ralanan iki par�a birle�tirildi.
		merge(A,B,C,size);
	}
}
