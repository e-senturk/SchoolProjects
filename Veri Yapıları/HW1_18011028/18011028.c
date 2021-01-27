#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Maximum String Boyutu 5000, Maximum Eleman Boyutu 100, Maximum Dosya Adý Uzunluðu 30, Maximum Integer rakam sayýsý 10 olarak atandý.
#define MAX_STRING 5000
#define MAX_SIZE 100
#define FILE_NAME_SIZE 30
#define MAX_INT 10

//struct yapýsýnda; adres, o adrese sahip elemen sayýsýný tutan counter, ileri ve geri nodlarýný tutacak struct pointerlarý tanýmlandý.
struct cache{
	char address [MAX_SIZE];
	int counter;
	struct cache* next;
	struct cache* previous;
};

//typedef ile tip tanýmlamasý yapýldý. Bu sayede nesne oluþturmada karþýlaþýlan tanýmlama problemi ortadan kaldýrýldý.
typedef struct cache cache_buffer;

//Headerlar tanýmlandý.
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

//Bu fonksiyon boþ bir linkli liste elemaný oluþturmak için kullanýldý.
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

//Bu fonksiyon verilen deðerlere göre yeni bir linkli liste elemaný oluþturmak için kullanýldý.
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

//Bu fonksiyon verilen linkli listenin sonuna verilen yeni elemaný eklemek için kulanýldý.
cache_buffer* push_back(cache_buffer* list, cache_buffer* adding){
	cache_buffer* temp;
	for(temp = list; temp -> next != NULL; temp = temp -> next);
	temp -> next = adding;
	adding -> previous = temp;
	return list;
}

//Bu fonksiyon verilen linkli listenin baþýna verilen yeni elemaný eklemek için kulanýldý.
cache_buffer* push_front(cache_buffer* list, cache_buffer* adding){
	list -> previous = adding;
	adding -> next = list;
	list = adding;
	return list;
}

//Bu fonksyion verilen listenin son elemanýný silip deðer olarak o elemaný döndürmek için kullanýldý.
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

//Bu fonksiyon verilen linkli listeyi yazdýrmak için kullanýldý.
void print_list(cache_buffer* list){
	cache_buffer* temp;
	
	for(temp = list; temp -> next != NULL; temp = temp -> next){
		printf("%s,%d <-> ", temp -> address, temp -> counter);
	}
	printf("%s,%d\n", temp -> address, temp -> counter);	
}

//Bu fonksiyon verilen linkli listenin boyutunu ölçmek için kullanýldý.
int size_calculator(cache_buffer* list){
	int size = 0;
	cache_buffer* temp;
	for(temp = list; temp != NULL; temp = temp -> next){
		size++;
	}
	return size;
}

//Bu fonksiyon verilen dosyada ilk satýr içerisindeki 2 elemaný integer olarak almak için kullanýldý.
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

//Bu fonksiyon açýlýþ yazýsýný yazdýrmak için kullanýldý.
void startup_text(){
	printf("\t\t--Veri Yapilari 1. Odevi--\n");
	printf("1-) Klavye ile giris yapin.\n");
	printf("2-) Dosyadan okuyarak giris yapin.\n");
	printf("Q-) Cikmak icin Q ya basin.\n");
	printf("Veri giris metodunu seciniz: ");
}

//Bu fonksiyon verilen linkli listeyi pointerý üzerinden alýp kendisini ve ona baðlý elemanlarý silmek için kullanýldý.
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

//Kullanýcýdan diziyi temizlemek isteyip istemediði inputunu alýp ona göre diziyiyi ve ekrandaki yazýlarý temizlemek için kullanýldý.
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

/*Bu fonkisyon tanýmlanan iþlemlerin büyük kýsmýný yapmak için kullanýlan temel fonksiyondur.
  list verilen listeyi;
  text input olarak alýnan yeni elemaný
  limit ise bize verilen T deðerini ifade etmekte.
  Algoritma aþamalarýnda;
  	1-) Ýlk while döngüsü ile verilen eleman adresinin listede olup olmadýðý kontrol edildi.
  	 	Eðer adres listede yoksa temp deðeri listenin en sonundaki NULL pointerda,
  	 	Eðer adres listede varsa temp deðeri listenin o elemanýn olduðu konumunda kalmasý þartý tanýmlandý.
  	
	2-) Eðer adres listede yoksa listenin sonuna o adres deðeri için yeni eleman eklendi.
	
	3-) Eðer adres listede varsa counter 1 arttýrýldý.
	
	4-)	Eðer counter limit deðerinin üstünde ise ve listenin ilk elemaný deðilse, eleman bulunduðu konumdan
		silinip listenin baþýna eklendi. Listenin son elemaný olma durumunda bir sonraki eleman olmadýðýndan
		hata oluþumunu engellemek için o eleman üzerinden iþlem yapýlmadý.
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
	1-) Liste boþ ise linkli listenin doðrudan head nodunu oluþtumak için,
	2-) Liste Bboþ deðilse alýnan listenin adresini ve her bir adresi uygun biçimde list modifiere yönlendirmek için,
	3-) Liste boyutunu kontrol edip L deðerinden büyükse son elemaný silmek için,
	4-) listenin elemanlarýný yazdýrmak için kullanýldý.
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
   # Ana fonksiyon üzerinde input alma iþlemi yapýldý.
   # metot deðiþkeni alýnacak inputun türünü seçmek için oluþturuldu.
   # text_temp input alýnan adreslerin herbirini geçici olarak saklamak için oluþturudu.
   # size L deðerini saklamak için oluþturudu.
   # limit T deðerini saklamak için oluþturudu.
   # list head nodunu saklamak için oluþturuldu.
   
   Main fonksiyonu Aþamalarý;
   1-) Input yöntemi metot karakterinde saklandý. Hatalý inputlar önemsenmedi.
   2-) Q veya q için program kapatýldý.
   3-) 1 için klavyeden input alýndý.
   		3a-) Klavyeden T, L deðerleri alýndý.
   		3b-) Tüm adresler klavyeden gets fonksiyonu ile alýnýp text_long adlý bir stringe kaydedildi.
   		3c-) str_tok fonksyonu ile her bir adres text_short adlý stinge aktarýldý.
   		3d-) Alýnan adreslere göre ve oluþturulan listeyi düzenlenmek üzere deðiþkenler prime fonksiyonuna gönderildi.
   		3e-) Kullanýcýdan input alýnarak listenin silinip tüm iþlemin yeniden yapýlmasý için clear_question fonksiyonu kullanýldý.
   			 Bu iþlem için yine metot deðiþkeni kullanýldý ve metot için oluþturulan while döngüsü ile baþa dönüldü.
   	4-) 2 için dosyadan input alýndý.
   		4a-) Dosya adý kullanýcýdan alýnarak sonuna txt uzantýsý eklendi.
   		4b-) Dosya file pointer üzerinde açýldý eðer dosya yoksa hata mesajý yazdýrýldý.
   		4c-) Dosyanýn ilk satýrýndaki T ve L deðerleri get_numbers fonksiyonu ile limit ve size deðiþkenlerine kaydedildi.
   		4d-) Kalan dosya üzerindeki bilgiler fscanf fonksiyonu ile kelime kelime alýnýp text_temp deðiþkenine kaydedildi.
   		4e-) Alýnan adreslere göre ve oluþturulan listeyi düzenlenmek ve listeyi yazdýrmak üzere deðiþkenler prime fonksiyonuna gönderildi.
   		4f-) Kullanýcýdan input alýnarak listenin silinip tüm iþlemin yeniden yapýlmasý için clear_question fonksiyonu kullanýldý.
		     Bu iþlem için yine metot deðiþkeni kullanýldý ve metot için oluþturulan while döngüsü ile baþa dönüldü. 		
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
