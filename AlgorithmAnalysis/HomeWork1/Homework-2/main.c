#include <stdio.h>
#include <stdlib.h>

//Fonksiyon Tan�mlar�
void neighborhood(int**,int*,int);
void print_2d_matrix(int**,int);
void print_array(int*,int);

int main(int argc, char *argv[]) {
	/*
	De�i�kenler
		- n: Von Neumann de�eri
		- size: matris boyutu i�in
		- i ve j: iterasyon i�in
		- A matris i�in
		- row her sat�rdaki siyah karelerin say�s�n� saklamak i�in
	kullan�ld�.
	*/
	int n,size,i,j,**A,*row;
	// Kullan�c�dan n de�eri al�nd�.
	printf("Lutfen bir n sayisi giriniz: ");
	scanf("%d",&n);
	// Al�nan n de�erinin 2 kat�n�n 3 fazlas� kadar sat�r ve s�t�n gerekece�inden bu de�er hesapland�.
	size= 2*n+3;
	// T�m ba�lang�� de�erleri 0 olmas� gerekti�i i�in calloc ile dinamik memory allocation yap�ld�.
	A = (int **)calloc(size, sizeof(int *));
	for(i=0;i<size;i++)
		A[i] = (int *)calloc(size, sizeof(int));
	// Sat�rdaki siyah kare say�s�n� saklamak i�in bir dizi olu�turuldu.
	row = (int*)malloc(size*sizeof(int));
	// Neighborhood fonksiyonu ile matris istenilen say�da 1 le dolduruldu ve her sat�rdaki 1 say�s� bir diziye kaydedildi.
	neighborhood(A,row,n);
	// Kullan�c� i�in gerekli outputlar yard�mc� fonksiyonlar arac�l��� ile yazd�r�ld�.
	printf("Sonuclar:\n\n");
	printf("Istenilen Matris:\n");
	print_2d_matrix(A,size);
	printf("\n\n");
	print_array(row,size);
	printf("Toplam siyah kare sayisi: %d\n",2*n*(n+1)+1);
	return 0;
}

//Matrisi istenen �ekilde 1'ler ile dolduran ve her sat�rdaki 1 say�s�n� bir diziye kaydeden fonksiyon
void neighborhood(int** A,int* row, int n){
	/*
	De�i�kenler
		- A matris i�in
		- row her sat�rdaki siyah karelerin say�s�n� saklamak i�in
		- n: Von Neumann de�eri
		- i ve j: iterasyon i�in
		- st,en: Eklenecek 1 de�erlerinin ba�lang�� ve biti� indexlerini saklamak i�in
	kullan�ld�.
	*/
	int i,j,st,en;
	// Ilk ve son sat�rlar bo� olaca��ndan 0 de�erleri atand�.
	row[0] = 0;
	row[2*n+2] = 0;
	// Kalan t�m sat�rlar� teker teker dolduruldu.
	for(i=0;i<=2*n;i++){
		// Matrisin �st yar�s� ve alt yar�s� i�in farkl� i�lemler yap�lmas� gerekti�inden koyulacak 1'lerin ba�lang�� ve biti� noktalar� belirlendi.
		if(i<n){
			st = n+1-i;
			en = n+1+i;
		}
		else{
			st = i-n+1;
			en = 3*n-i+1;
		}
		// Belirlenen ba�lang�� ve biti� noktalar�na g�re 1'ler yerle�tirildi.
		for(j=st;j<=en;j++){
			A[i+1][j]++;
		}
		// Yerle�tirilen 1'lerin say�s� kullan�c�ya verilmek �zere bir dizide kaydedildi.
		row[i+1]=en-st+1;	
	}
}

//2D matris yazd�ran yard�mc� fonksiyon
void print_2d_matrix(int **A, int size){
	/*
	De�i�kenler
		- A matris i�in
		- size matrisin boyutu i�in
		- i ve j: iterasyon i�in
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

// Verilen diziyi yazd�ran yard�mc� fonksiyon
void print_array(int *A, int size){
	/*
	De�i�kenler
		- A dizi i�in
		- size dizinin boyutu i�in
		- i: iterasyon i�in
	kullan�ld�.
	*/
	int i;
	printf("Satirdaki Siyah Kareler: [");
	for (i = 0; i < size-1; i++) {
		printf("%d, ", A[i]); 	
	}
	printf("%d]\n",A[size-1]);
}

