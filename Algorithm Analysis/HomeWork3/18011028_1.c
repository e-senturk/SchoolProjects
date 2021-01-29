#include <stdio.h>
#include <stdlib.h>

/* Hesaplanan matrisi yazdiran fonksiyon.
	parametreler
		- A : Dinamik Programlamada kullanilan matris
		- M,N : Matrisin boyutlari
	return 
		- Iki kelime arasindaki Levenshtein Edit Distance mesafesi
*/
void print(float** A,int M,int N){
	int i,j;						// iterator
	for(i=0;i<M;i++){
		for(j=0;j<N;j++){
			printf("%0.2f ",A[i][j]);
		}
		printf("\n");
	}
}

/* Hesaplanan matrisi icin memory allokasyonu yapan fonksiyon.
	parametreler
		- M,N : Olusturlacak matrisin boyutlari
	return 
		- Dinamik Programlamada kullanilacak matris
*/
float ** generate_void(int M,int N){
	float** A = (float**)malloc(sizeof(float*)*M);			// Hesaplanan orani tutacak bos matris
	int i;													// iterator
	for(i=0;i<M;i++){
		A[i]=(float*)malloc(sizeof(float)*N);
	}
	return A;												
}

/* Main Fonksiyon
	- Satir kismi kazanma sutun kismi ise kaybetme ihtimalini temsil ediyor.
	- 0. indislerde kazanma satiri icin 1 kaybetme sutunu icin ise 0 olacak sekilde ayarlandi.
	- Geriye kalan indislerdeki elemanlar kazanma ihtimali*ustundeki hücre + kaybetme ihtimal* solundaki hucre formulu ile hesaplandi
	- Tüm matris dolduruldugunda son hucre hesaplanan oran degerimizi vermis oldu. 
	return 
		- Program tamamlandi
*/
int main() {
	int i,j;			// Tterator
	int n;				// Gerekli kazanma sayisi
	float ratio;		// Hesaplanan orani tutan matris
	float** A;			// Oranlar matrisi
	printf("\tWin Ratio Calculator\n");
	printf("Please enter required win count: ");
	scanf("%d",&n);
	printf("Please enter win ratio: ");
	scanf("%f",&ratio);	
	A = generate_void(n+1,n+1);
	for(i=0;i<n+1;i++){
		for(j=0;j<n+1;j++){
			if(i==0 && j>0)
				A[i][j] = 1.0;
			else if(i>0 && j==0)
				A[i][j] = 0.0;
			else if(i>0 && j>0)
				A[i][j] = A[i-1][j]*ratio+A[i][j-1]*(1-ratio);
		}
	}
	printf("Winning ratio is %0.2f.\n\n",A[n][n]);
	printf("Dynamic Programming Table:\n");
	print(A,n+1,n+1);
	return 0;		  
}
