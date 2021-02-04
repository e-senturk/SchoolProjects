#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Maximum String Boyutu 5000, Maximum Eleman Boyutu 100, Maximum Dosya Ad� Uzunlu�u 30, Maximum Integer rakam say�s� 10 olarak atand�.
#define MAX_STRING 5000
#define MAX_SIZE 100
#define FILE_NAME_SIZE 30
#define MAX_INT 10

//struct yap�s�nda; adres, o adrese sahip elemen say�s�n� tutan counter, ileri ve geri nodlar�n� tutacak struct pointerlar� tan�mland�.
struct cache{
	char address [MAX_SIZE];
	int counter;
	struct cache* next;
	struct cache* previous;
};

//typedef ile tip tan�mlamas� yap�ld�. Bu sayede nesne olu�turmada kar��la��lan tan�mlama problemi ortadan kald�r�ld�.
typedef struct cache cache_buffer;

// function headers
cache_buffer* create();
cache_buffer* create_value(char*, int);
cache_buffer* push_back(cache_buffer*, cache_buffer*);
cache_buffer* push_front(cache_buffer*, cache_buffer*);
cache_buffer* pop_back(cache_buffer*);
void print_list(cache_buffer*);
int size_calculator(cache_buffer*);
bool get_numbers(FILE*, int*, int*);
void startup_text();
void clear_list(cache_buffer**);
void clear_question(cache_buffer**, char *, char);
cache_buffer* list_modifier(cache_buffer*, char*, int);
void prime(cache_buffer**, char*, int, int);

// used for generating an empty linked list
cache_buffer* create(){
	cache_buffer* element;
	element = (cache_buffer*) malloc (sizeof (cache_buffer));
	if(element == NULL){
		printf("Malloc failed.\n");
		exit(1);
	}
	element -> next = NULL;
	element -> previous = NULL;
	return element;
}

//Bu fonksiyon verilen de�erlere g�re yeni bir linkli liste eleman� olu�turmak i�in kullan�ld�.
cache_buffer* create_value(char* address, int counter){
	cache_buffer* element;
	element = (cache_buffer*) malloc (sizeof (cache_buffer));
	if(element == NULL){
		printf("Malloc failed.\n");
		exit(1);
	}
	strcpy(element -> address, address);
	element -> counter = counter;
	element -> next = NULL;
	element -> previous = NULL;
	return element;
}

//Bu fonksiyon verilen linkli listenin sonuna verilen yeni eleman� eklemek i�in kulan�ld�.
cache_buffer* push_back(cache_buffer* list, cache_buffer* adding){
	cache_buffer* temp;
	for(temp = list; temp -> next != NULL; temp = temp -> next);
	temp -> next = adding;
	adding -> previous = temp;
	return list;
}

//Bu fonksiyon verilen linkli listenin ba��na verilen yeni eleman� eklemek i�in kulan�ld�.
cache_buffer* push_front(cache_buffer* list, cache_buffer* adding){
	list -> previous = adding;
	adding -> next = list;
	list = adding;
	return list;
}

//Bu fonksyion verilen listenin son eleman�n� silip de�er olarak o eleman� d�nd�rmek i�in kullan�ld�.
cache_buffer* pop_back(cache_buffer* list){
	if(list == NULL){
		printf("Pop failed");
		return NULL;
	}
	cache_buffer* temp;
	for(temp = list; temp -> next != NULL; temp = temp -> next);
	temp ->previous ->next = NULL; 
	return temp;
}

//Bu fonksiyon verilen linkli listeyi yazd�rmak i�in kullan�ld�.
void print_list(cache_buffer* list){
	cache_buffer* temp;
	
	for(temp = list; temp -> next != NULL; temp = temp -> next){
		printf("%s,%d <-> ", temp -> address, temp -> counter);
	}
	printf("%s,%d\n", temp -> address, temp -> counter);	
}

//Bu fonksiyon verilen linkli listenin boyutunu �l�mek i�in kullan�ld�.
int size_calculator(cache_buffer* list){
	int size = 0;
	cache_buffer* temp;
	for(temp = list; temp != NULL; temp = temp -> next){
		size++;
	}
	return size;
}

//Bu fonksiyon verilen dosyada ilk sat�r i�erisindeki 2 eleman� integer olarak almak i�in kullan�ld�.
bool get_numbers(FILE* file, int* T, int* L){
	char temp[MAX_SIZE], number1[MAX_INT], number2[MAX_INT];
	fgets(temp, MAX_SIZE, file);
	int i = 0, k = 0;
	while(temp[i] != '='){
		i++;
	}
	i++;
	while((i < MAX_SIZE) && (k < MAX_INT) && (temp[i] >= '0') && (temp[i] <= '9')){
		number1[k] = temp [i];
		i++;
		k++;
	}
	while(temp[i] != '='){
		i++;
	}
	k = 0;
	i++;
	while((i<MAX_SIZE) && (k < MAX_INT) && (temp[i] >= '0') && (temp[i] <= '9')){
		number2[k] = temp [i];
		i++;
		k++;
	}
	*L = atoi(number1);
	*T = atoi(number2);	
}

//Bu fonksiyon a��l�� yaz�s�n� yazd�rmak i�in kullan�ld�.
void startup_text(){
	printf("\t\t--Veri Yapilari 1. Odevi--\n");
	printf("1-) Klavye ile giris yapin.\n");
	printf("2-) Dosyadan okuyarak giris yapin.\n");
	printf("Q-) Cikmak icin Q ya basin.\n");
	printf("Veri giris metodunu seciniz: ");
}

//Bu fonksiyon verilen linkli listeyi pointer� �zerinden al�p kendisini ve ona ba�l� elemanlar� silmek i�in kullan�ld�.
void clear_list(cache_buffer** list){  
	cache_buffer* current = *list;  
	cache_buffer* next;  
	while (current != NULL){  
		    next = current -> next;  
		    free(current);  
		    current = next;  
	}  
	*list = NULL;  
}

//Kullan�c�dan diziyi temizlemek isteyip istemedi�i inputunu al�p ona g�re diziyiyi ve ekrandaki yaz�lar� temizlemek i�in kullan�ld�.
void clear_question(cache_buffer** list, char *metot, char value){
    printf("Cache Listesini Silip Yeniden Baslamak Istermisiniz?(Y/N)\n");
	*metot = getch();
	if(*metot == 'Y' || *metot == 'y'){
		*metot = value;
		clear_list(list); 
	}
	else{
		*metot == 'Q';
	}
	system("cls || clear");
} 

/*Bu fonkisyon tan�mlanan i�lemlerin b�y�k k�sm�n� yapmak i�in kullan�lan temel fonksiyondur.
  list verilen listeyi;
  text input olarak al�nan yeni eleman�
  limit ise bize verilen T de�erini ifade etmekte.
  Algoritma a�amalar�nda;
  	1-) �lk while d�ng�s� ile verilen eleman adresinin listede olup olmad��� kontrol edildi.
  	 	E�er adres listede yoksa temp de�eri listenin en sonundaki NULL pointerda,
  	 	E�er adres listede varsa temp de�eri listenin o eleman�n oldu�u konumunda kalmas� �art� tan�mland�.
  	
	2-) E�er adres listede yoksa listenin sonuna o adres de�eri i�in yeni eleman eklendi.
	
	3-) E�er adres listede varsa counter 1 artt�r�ld�.
	
	4-)	E�er counter limit de�erinin �st�nde ise ve listenin ilk eleman� de�ilse, eleman bulundu�u konumdan
		silinip listenin ba��na eklendi. Listenin son eleman� olma durumunda bir sonraki eleman olmad���ndan
		hata olu�umunu engellemek i�in o eleman �zerinden i�lem yap�lmad�.
*/
cache_buffer* list_modifier(cache_buffer* list, char* text, int limit){
	cache_buffer* temp = list;
	while(temp != NULL && strcmp(temp -> address, text)){
		temp = temp->next;
	}
	if(temp == NULL){
		cache_buffer* new_element = create_value(text, 1);
		list = push_front(list, new_element);
		return list;
	}
	else{
		temp -> counter++;
		if(temp -> counter > limit && temp != list){
			temp -> previous -> next = temp -> next;
			if(temp -> next != NULL){
				temp -> next -> previous = temp -> previous;
			}	
			list = push_front(list, temp);
		}
		return list;
	}
}

/*Bu fonkisyon list_modifer fonksiyonunu kullanarak
	1-) Liste bo� ise linkli listenin do�rudan head nodunu olu�tumak i�in,
	2-) Liste Bbo� de�ilse al�nan listenin adresini ve her bir adresi uygun bi�imde list modifiere y�nlendirmek i�in,
	3-) Liste boyutunu kontrol edip L de�erinden b�y�kse son eleman� silmek i�in,
	4-) listenin elemanlar�n� yazd�rmak i�in kullan�ld�.
*/
void prime(cache_buffer** list, char* adr, int size, int limit){
	if(*list == NULL){
		*list =	create_value(adr, 1);
	}
	else{
		*list = list_modifier(*list, adr, limit);
	}
	if(size_calculator(*list) > size){
		pop_back(*list);
	}
	print_list(*list);  
}

/* 
   # Ana fonksiyon �zerinde input alma i�lemi yap�ld�.
   # metot de�i�keni al�nacak inputun t�r�n� se�mek i�in olu�turuldu.
   # text_temp input al�nan adreslerin herbirini ge�ici olarak saklamak i�in olu�turudu.
   # size L de�erini saklamak i�in olu�turudu.
   # limit T de�erini saklamak i�in olu�turudu.
   # list head nodunu saklamak i�in olu�turuldu.
   
   Main fonksiyonu A�amalar�;
   1-) Input y�ntemi metot karakterinde sakland�. Hatal� inputlar �nemsenmedi.
   2-) Q veya q i�in program kapat�ld�.
   3-) 1 i�in klavyeden input al�nd�.
   		3a-) Klavyeden T, L de�erleri al�nd�.
   		3b-) T�m adresler klavyeden gets fonksiyonu ile al�n�p text_long adl� bir stringe kaydedildi.
   		3c-) str_tok fonksyonu ile her bir adres text_short adl� stinge aktar�ld�.
   		3d-) Al�nan adreslere g�re ve olu�turulan listeyi d�zenlenmek �zere de�i�kenler prime fonksiyonuna g�nderildi.
   		3e-) Kullan�c�dan input al�narak listenin silinip t�m i�lemin yeniden yap�lmas� i�in clear_question fonksiyonu kullan�ld�.
   			 Bu i�lem i�in yine metot de�i�keni kullan�ld� ve metot i�in olu�turulan while d�ng�s� ile ba�a d�n�ld�.
   	4-) 2 i�in dosyadan input al�nd�.
   		4a-) Dosya ad� kullan�c�dan al�narak sonuna txt uzant�s� eklendi.
   		4b-) Dosya file pointer �zerinde a��ld� e�er dosya yoksa hata mesaj� yazd�r�ld�.
   		4c-) Dosyan�n ilk sat�r�ndaki T ve L de�erleri get_numbers fonksiyonu ile limit ve size de�i�kenlerine kaydedildi.
   		4d-) Kalan dosya �zerindeki bilgiler fscanf fonksiyonu ile kelime kelime al�n�p text_temp de�i�kenine kaydedildi.
   		4e-) Al�nan adreslere g�re ve olu�turulan listeyi d�zenlenmek ve listeyi yazd�rmak �zere de�i�kenler prime fonksiyonuna g�nderildi.
   		4f-) Kullan�c�dan input al�narak listenin silinip t�m i�lemin yeniden yap�lmas� i�in clear_question fonksiyonu kullan�ld�.
		     Bu i�lem i�in yine metot de�i�keni kullan�ld� ve metot i�in olu�turulan while d�ng�s� ile ba�a d�n�ld�. 		
*/
int main() {
	char metot = '0';
	int size, limit;
	cache_buffer* list = NULL;
	startup_text();
	
	while((metot != '1') && (metot != '2') && (metot != 'Q') && (metot != 'q' )){
		metot = getch();
	}
	if((metot == 'q') || (metot == 'Q')){
		return 0;
	}
	while(metot == '1'){
		int i, k;
		char* text_short;
		char text_long[MAX_STRING];
		printf("Klavyeden okuma metodu secildi.\n");
		printf("Adresi Listenin Basina Koymak Icin Gereken Siniri Giriniz: ");
		scanf(" %d", &limit);		
		printf("Maximum Liste Boyutunu Giriniz: ");
		scanf(" %d", &size);
		printf("Elemanlari Arada Bosluk Birakarak Giriniz ve Enter a basiniz.\n");
		getchar();	
		gets(text_long);
		text_short = strtok(text_long, " ");
		while(text_short != NULL) {
			prime(&list, text_short, size, limit);
		    text_short = strtok(NULL, " ");
      	}
		clear_question(&list, &metot, '1');
	}		
	while(metot == '2'){
		char file_name[FILE_NAME_SIZE];
		char text_temp[MAX_SIZE];
		printf("Dosyadan okuma metodu secildi.\n");
		printf("Dosya adini uzantisiz olarak giriniz.\n");
		scanf("%s", file_name);
		strcat(file_name,".txt");
		FILE* file = fopen(file_name, "rb");
		if(file == NULL){
			printf("Dosya yok veya acilamadi\n");
			return 0;
		}
		get_numbers(file, &size, &limit);
		while (fscanf(file, " %s", text_temp) == 1) {
			 prime(&list, text_temp, size, limit);      
	    }
	    fclose(file);
		clear_question(&list, &metot, '2');
	}
	return 0;
}
