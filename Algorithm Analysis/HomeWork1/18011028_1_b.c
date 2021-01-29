#include <stdio.h>
#include <stdlib.h>
void merge(int*,int*,int*,int);
void merge_sort(int*,int);

int main(int argc, char *argv[]) {
	/*
	Deðiþkenler
		- size: dizi boyutu için
		- i ve j: iterasyon için
		- min elemanlar arasýndaki en küçük farký saklamak için
		- index aranan farký en küçük elemanlardan ilkinin konumunu saklamak için
	kullanýldý.
	*/
	int size,i,j,min,index;
	// Dizinin boyutu kullanýcýdan alýndý.
	printf("Dizinin boyutunu giriniz: ");
	scanf("%d",&size);
	//2 elemandan daha küçük dizide en yakýn 2 eleman olamayacaðýndan programdan çýkýldý.
	if(size<2){
		printf("Dizi boyutu en az 2 olabilir.\n");
		return 0;
	}
	// Alýnan boyuttaki dizi oluþturuldu.
	int A[size];
	
	// Dizinin elemanlarý kullanýcýdan input olarak alýndý.
	printf("Dizinin elemanlarini giriniz:\n");
	for(i=0;i<size;i++){
		scanf("%d",&A[i]);
	}
	// Dizinin elemanlarý merge sort ile sýralandý. O(nlogn) karmaþýklýk
	merge_sort(A,size);
	// Ardaþýk iki elemanýn farký en küçük olan eleman alýndý. O(n) karmaþýklýk
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
	Deðiþkenler
		- i B dizisinin, j C dizisinin, k A dizisinin indisini saklamak için 
		- B ve C birleþtirilecek dizileri A ise sonucun kaydedileceði diziyi ifade etmek için
		- size dizi boyutu için kullanýldý.
	kullanýldý.
	*/
	int i=0, j=0, k=0;
	//Dizinin her iki parçasý sýrasý ile A dizisinde birleþtirildi.
	while(i<size/2 && j<size-size/2){
		if(B[i]<=C[j]){
			A[k]=B[i++];
		}
		else{
			A[k]=C[j++];
		}
		k++;
	}
	//Elemaný kalan dizideki elemanlar da diziye eklendi.
	while(i<size/2){
		A[k]=B[i++];
		k++;
	}
	while(j<size-size/2){
		A[k]=C[j++];
		k++;
	}
}


// Merge sort algoritmasý
void merge_sort(int* A,int size){
	//Boyutu 1 oluncaya kadar dizi alt parçalara bölündü.
	if(size>1){
		/*
		Deðiþkenler
			- size: dizi boyutu için
			- i ve j: iterasyon için
			- B dizinin ilk yarýsýný C ise son yarýsýný saklamak için
		kullanýldý.
		*/
		int B[size/2],C[size-size/2],i,j;
		for(i=0;i<size/2;i++){
			B[i]=A[i];
		}
		for(i=0,j=size/2;j<size;i++,j++){
			C[i]=A[j];
		}
		// Dizinin küçük alt parçasý ve büyük alt parçasý rekürsif olarak parçalandý.
		merge_sort(B,size/2);
		merge_sort(C,size-size/2);
		// Sýralanan iki parça birleþtirildi.
		merge(A,B,C,size);
	}
}
