#include <stdio.h>
#include <stdlib.h>

//Fonksiyon Tanýmlarý
void neighborhood(int**,int*,int);
void print_2d_matrix(int**,int);
void print_array(int*,int);

int main(int argc, char *argv[]) {
	/*
	Deðiþkenler
		- n: Von Neumann deðeri
		- size: matris boyutu için
		- i ve j: iterasyon için
		- A matris için
		- row her satýrdaki siyah karelerin sayýsýný saklamak için
	kullanýldý.
	*/
	int n,size,i,j,**A,*row;
	// Kullanýcýdan n deðeri alýndý.
	printf("Lutfen bir n sayisi giriniz: ");
	scanf("%d",&n);
	// Alýnan n deðerinin 2 katýnýn 3 fazlasý kadar satýr ve sütün gerekeceðinden bu deðer hesaplandý.
	size= 2*n+3;
	// Tüm baþlangýç deðerleri 0 olmasý gerektiði için calloc ile dinamik memory allocation yapýldý.
	A = (int **)calloc(size, sizeof(int *));
	for(i=0;i<size;i++)
		A[i] = (int *)calloc(size, sizeof(int));
	// Satýrdaki siyah kare sayýsýný saklamak için bir dizi oluþturuldu.
	row = (int*)malloc(size*sizeof(int));
	// Neighborhood fonksiyonu ile matris istenilen sayýda 1 le dolduruldu ve her satýrdaki 1 sayýsý bir diziye kaydedildi.
	neighborhood(A,row,n);
	// Kullanýcý için gerekli outputlar yardýmcý fonksiyonlar aracýlýðý ile yazdýrýldý.
	printf("Sonuclar:\n\n");
	printf("Istenilen Matris:\n");
	print_2d_matrix(A,size);
	printf("\n\n");
	print_array(row,size);
	printf("Toplam siyah kare sayisi: %d\n",2*n*(n+1)+1);
	return 0;
}

//Matrisi istenen þekilde 1'ler ile dolduran ve her satýrdaki 1 sayýsýný bir diziye kaydeden fonksiyon
void neighborhood(int** A,int* row, int n){
	/*
	Deðiþkenler
		- A matris için
		- row her satýrdaki siyah karelerin sayýsýný saklamak için
		- n: Von Neumann deðeri
		- i ve j: iterasyon için
		- st,en: Eklenecek 1 deðerlerinin baþlangýç ve bitiþ indexlerini saklamak için
	kullanýldý.
	*/
	int i,j,st,en;
	// Ilk ve son satýrlar boþ olacaðýndan 0 deðerleri atandý.
	row[0] = 0;
	row[2*n+2] = 0;
	// Kalan tüm satýrlarý teker teker dolduruldu.
	for(i=0;i<=2*n;i++){
		// Matrisin üst yarýsý ve alt yarýsý için farklý iþlemler yapýlmasý gerektiðinden koyulacak 1'lerin baþlangýç ve bitiþ noktalarý belirlendi.
		if(i<n){
			st = n+1-i;
			en = n+1+i;
		}
		else{
			st = i-n+1;
			en = 3*n-i+1;
		}
		// Belirlenen baþlangýç ve bitiþ noktalarýna göre 1'ler yerleþtirildi.
		for(j=st;j<=en;j++){
			A[i+1][j]++;
		}
		// Yerleþtirilen 1'lerin sayýsý kullanýcýya verilmek üzere bir dizide kaydedildi.
		row[i+1]=en-st+1;	
	}
}

//2D matris yazdýran yardýmcý fonksiyon
void print_2d_matrix(int **A, int size){
	/*
	Deðiþkenler
		- A matris için
		- size matrisin boyutu için
		- i ve j: iterasyon için
	*/
	int i,j;
	for (i = 0; i <  size; i++) {
		printf("| ");
		for (j = 0; j < size-1; j++){
			 printf("%d ", A[i][j]); 	
		}
		printf("%d |\n",A[i][size-1]) ;
	}
}

// Verilen diziyi yazdýran yardýmcý fonksiyon
void print_array(int *A, int size){
	/*
	Deðiþkenler
		- A dizi için
		- size dizinin boyutu için
		- i: iterasyon için
	kullanýldý.
	*/
	int i;
	printf("Satirdaki Siyah Kareler: [");
	for (i = 0; i < size-1; i++) {
		printf("%d, ", A[i]); 	
	}
	printf("%d]\n",A[size-1]);
}

