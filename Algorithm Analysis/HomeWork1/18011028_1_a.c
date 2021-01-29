#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	/*
	Deðiþkenler
		- size: dizi boyutu için
		- i ve j: iterasyon için
		- min elemanlar arasýndaki en küçük farký saklamak için
		- diff elemanlar arasýndaki her farký geçici olarak saklamak için
		- first ve second aranan farký en küçük elemanlarý saklamak için
	kullanýldý.
	*/
	int size,i,j,min,diff,first,second;
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
	// min deðiþkeninde en az farký olan 2 eleman first ve second deðiþkenlerinde bu elemanlarýn indexleri tutuldu.
	// iç içe 2 for döngüsü ile bruteforce metodu ile ikiþerli olarak elemanlarýn farký hesaplandý.
	// farkýn önceki minimum farktan küçük olmasý durumunda yeni fark deðeri mininum olarak atandý. Bu elemanlarýn indisleri saklandý.
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
	// Bulunan indexlere göre en yakýn 2 eleman kullanýcýya output olarak verildi.
	printf("En yakin 2 sayi: %d ve %d",A[first],A[second]);
	return 0;
}
