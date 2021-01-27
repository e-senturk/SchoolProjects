#include <stdio.h>
#include <stdlib.h>

int main() {
	/*
	Variables
		- size: array size
		- i ve j: iterators
		- min elemanlar aras�ndaki en k���k fark� saklamak i�in
		- diff elemanlar aras�ndaki her fark� ge�ici olarak saklamak i�in
		- first ve second aranan fark� en k���k elemanlar� saklamak i�in
	kullan�ld�.
	*/
	int size,i,j,min,diff,first,second;
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
	// min de�i�keninde en az fark� olan 2 eleman first ve second de�i�kenlerinde bu elemanlar�n indexleri tutuldu.
	// i� i�e 2 for d�ng�s� ile bruteforce metodu ile iki�erli olarak elemanlar�n fark� hesapland�.
	// fark�n �nceki minimum farktan k���k olmas� durumunda yeni fark de�eri mininum olarak atand�. Bu elemanlar�n indisleri sakland�.
	min = abs(A[0]-A[1]);
	first = 0;
	second = 1,
	diff;
	for(i=0;i<size-1;i++){
		for(j=i+1;j<size;j++){
			diff = abs(A[i]-A[j]);
			if(diff < min){
				min = diff;
				first = i;
				second = j;
			}
		}
	}
	// Bulunan indexlere g�re en yak�n 2 eleman kullan�c�ya output olarak verildi.
	printf("En yakin 2 sayi: %d ve %d",A[first],A[second]);
	return 0;
}
