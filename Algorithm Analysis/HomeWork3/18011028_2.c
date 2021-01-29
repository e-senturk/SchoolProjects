#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// Tablo boyutu 1000'e en yakin asal sayi olarak secildi
# define TABLE_SIZE 997
// Horner degeri hesaplanirken ussu alinacak sayi
#define R 31
// Kelime buffer boyutu
# define WORD_SIZE 50
// Input buffer boyutu
# define INPUT_SIZE 1000
// Input icin alinan kelime sayisi
# define WORD_COUNT 300
// Oneri listesi icin buffer boyutu
# define RECOMMEND_SIZE 300
// Dictionary dosya adi daha sonra degistirilebileceginden global degisken olarak atandi.
static const char dictionary_file_name[] = "smallDictionary.txt";

// Duzeltme icin kullanilan dizide saklanan struct yapisi
struct correction{
	char original[WORD_SIZE];
	char corrected[WORD_SIZE];
};
typedef struct correction correction;

//Yardimci Fonksiyonlar 
int order(char character);
char lowercase(char character);
bool compare_char(char c1,char c2);
bool compare_string(char* string1, char* string2);
size_t horner(char* string);
int find_min(int a,int b,int c, int d);
char** generate_words(char* input);
void print_result(char** words);

// Hash Fonksiyonlari
int double_hashing(char* string,int i);
correction* create_empty_correction();
int search_correction_table(char* string, correction* correction_table);
void insert_correction_table(char* original,char* corrected, correction* correction_table);
int find_correction_index(char* string, correction* correction_table);
char** create_empty_dictionary();
int search_dictionary_table(char* string, char** dictionary_table);
void insert_dictionary_table(char* string, char** dictionary_table);
bool read_dictionary(char** dictionary_table);
bool in_dictionary(char* word,char** dictionary_table);


// Dinamik Programlama fonksiyonlari
bool get_next(int** matrix,char* text1,char* text2,int *it1,int *it2,int value);
bool call_answer(int** matrix,char* text1,char* text2,int it1,int it2,int value);
int find_distance(char* text1,char* text2);
char* find_alternate_list(char* word,char** dictionary_table);

// Memory deallokasyon fonksiyonlari
void deallocate_string_list(char** words,int size);
void deallocate_matrix(int** matrix,int size);

/* Main Fonksiyon
	- create_empty_dictionary ve create_empty_correction fonksiyonlari ile bos hash tablolari olusturuldu.
	- Dictionary dosyasindan read_dictionary fonksiyonu ile kelimeler dictionary hash tablosuna eklendi.
	- kullanicidan input alindi ve eger bos input girilmemisse alinan input kelimelerine ayrildi.(input_words).
	- kelime once dictionary hash tablosunda arandi varsa degistirilmedi.
	- kelime daha sonra correction tablosunda arandi varsa onceki deger ile degistirildi.
	- iki listede de yoksa find_alternate_list fonksiyonu ile varsa 1 fark olanlarin yoksa 2 fark olanlarin listesi olusturuldu.
	- kullaniya liste onerildi ve dogru inputu yazmasi icin input alindi.
	- hatali kelime ve duzeltilmis hali corrected_tablosuna eklendi.
	- hatali kelime ile listeden degistirildi.
	- kullanicidan programdan cikacagi veya devam edecegi duruma gore input alindi.
	- memory allocation ile olusturulan pointerlar delloke edildi. 
	
	return 
		- Program sonlandirildi.
*/
int main(){
	char** dictionary_table = create_empty_dictionary();         // Verilen sozluk dosyasindan olusturlan hash tablosu
	correction* correction_table = create_empty_correction();	 // Girilen kelimelere gore ogrenilen kelimeleri sahlayan hash tablosu
	char input [INPUT_SIZE];									 // Kullanicidan alinan inputu saklayan buffer
	char** input_words;											 // Alinan inputu kelime kelime saklayan dizi.
	char corrected[WORD_SIZE];									 // Duzeltilen kelimenin duzgun halini tutan buffer
	int i;														 // Iterator
	int correction_index;										 // Girilen kelimenin duzeltme hash tablosundaki sirasini tutan degisken
	char* alternates;											 // Yazdirilacak lternatif kelimeleri tutan degisken 
	char quit[WORD_SIZE]=" ";
	printf("Welcome to the word deduction application\n");
	read_dictionary(dictionary_table);
	while(!compare_char(quit[0],'q')){
		printf("Please enter some text: ");
		fgets(input,INPUT_SIZE,stdin);
		//fgets son karakter olarak endl aliyor onu sildim.
		input[strlen(input) - 1] = '\0';
		if(strlen(input)>0){
			input_words = generate_words(input);
			i=0;
			while(input_words[i]!=NULL){
				if(!in_dictionary(input_words[i],dictionary_table)){
					correction_index = find_correction_index(input_words[i],correction_table);
					if(correction_index==-1){
						alternates = find_alternate_list(input_words[i],dictionary_table);
						if(alternates != NULL){
							printf("In word %s did you mean? \n - %s : ",input_words[i],alternates);
							free(alternates);
							fgets(corrected,WORD_SIZE,stdin);
							corrected[strlen(corrected) - 1] = '\0';
							insert_correction_table(input_words[i],corrected,correction_table);
							strcpy(input_words[i],corrected);
						}
					}
					else{
						strcpy(input_words[i],correction_table[correction_index].corrected);
					}
				}
				i++;
			}
			print_result(input_words);
			printf("Press q to exit press enter to continue...");
			fgets(quit,WORD_SIZE,stdin);
		}
	}
	free(correction_table);
	deallocate_string_list(dictionary_table,TABLE_SIZE);
	deallocate_string_list(input_words,WORD_COUNT);
	return 0;													
}



// Levenshtein Edit Distance'i sadece diagonal uzerinde hareket ederek hesaplayan fonksiyon. (BONUS)
/*
	- 1. ve 2. metinden 1 er buyuk uzunlukta satir ve sutun sayisinda matris tanimlandi
	- 1. adimda get_next ile diagonalde text1[i] ve text2[j] esit oldukca ilerlendi.
	- Eger matrisin son elemanina ulasilmasina ragmen hala farklilik bulunamamissa 
	  get_next true degeri dondureceginden 0 degeri donduruldu. 
	- Eger son satira kadar 1 degerine ulsilamamissa call answer cagrildi.
	- Eger call answer true sonucunu verirse mesafe 1 olmus olacagindan 1 donduruldu.
	- Call answer false donduyse matrisin son elemanina bakildi 2 olmasi durumunda mesafe olarak 2 donduruldu.
	- Farkli bir sonuc varsa mesafe 2den buyuk olacagindan -1 donduruldu.
	
	parametreler
		- text1 : Mesafe aranacak 1. kelime
		- text2 : Mesafe aranacak 2. kelime
	return 
		- Iki kelime arasindaki Levenshtein Edit Distance mesafesi
*/
int find_distance(char* text1,char* text2){
	int i,j;										// iteratorler
	size_t M = strlen(text1)+1;						// matrixin sutun satir sayisi
	size_t N = strlen(text2)+1;						// matrixin satýr sayisi
	bool answer = false;							// sonucun 1 olup olmadigini kontrol eden degisken
	int** matrix = (int**)malloc(sizeof(int*)*M);   // dinamic programlama matrisi
	for(i=0;i<M;i++){
		matrix[i]=calloc(N,sizeof(int));
	}
	i=0;j=0;
	answer = get_next(matrix,text1,text2,&i,&j,0);
	if(answer) {
		deallocate_matrix(matrix,M);
		return 0;
	}	
	answer = call_answer(matrix,text1,text2,i,j,1);
	if(answer) {
		deallocate_matrix(matrix,M);
		return 1;
	}	
	if(matrix[M-1][N-1]==2){
		deallocate_matrix(matrix,M);
		return 2;
	}
	deallocate_matrix(matrix,M);
	return -1;										
}


/* Matrisin verilen indexlerdeki elemanlari esit oldugu surece matrisi diagonal uzerinden dolduran fonksiyon.
	- matrisin verilen indexlerinden itibaren text1[i]==text2[j] oldugu surece diagonelde capraz olarak ilerlenir.
	- diagonelde ilerlerken esitsizlik veya i ve j nin sona ulasmasi durumunda looptan cikilir.
	- eger i ve j matrix text1 ve text2 nin sonuna ulasabilmisse mesafe = value olacagindan true dondurulur.
	- matrixe deger atarken atanacak degerin komsuluklarinda daha dusuk bir deger varsa bulunan en kucuk deger atanir
	- esitsizlik durumunda gelinen indexe eger sona ulasilmamis ise en kucuk degerin 1 fazlasi atanir.
	
	parametreler:
		- matrix: dinamik programlama icin kullanilan matris
		- text1 : Mesafe aranacak 1. kelime
		- text2 : Mesafe aranacak 2. kelime
		- it1,it2 : Daha onceden ulasilan iterator degerleri
		- value: Diagonali dolduracak deger
	return 
		- Son elemana ulasilma durumu.
*/

bool get_next(int** matrix,char* text1,char* text2,int *it1,int *it2,int value){
	int i = *it1;												// iterator
	int j = *it2;												// iterator
	size_t size1 = strlen(text1);								// text1 boyutu
	size_t size2 = strlen(text2);								// text2 boyutu
	while(i<size1 && j<size2 && text1[i]==text2[j]){
		matrix[i+1][j+1]=find_min(value,matrix[i+1][j+1],matrix[i][j+1],matrix[i+1][j]);
		i++;
		j++;
	}
	if(i>= size1 && j>=size2)
		return true;
	if(i<size1 && j<size2 && matrix[i+1][j+1] ==0){
		matrix[i+1][j+1]=find_min(value,matrix[i+1][j+1],matrix[i][j+1],matrix[i+1][j])+1;
	}
		
	*it1 = i;
	*it2 = j;
	return false;	
}

/*	Verilen 1 ve 2 ile dolu diagonalini dolduran fonksiyon
	- ilk farkli karakter bulunduktan sonra 3 farkli konumdan devam edilebilir.
			-- dolu satirin sagi 
			-- dolu satirin alti
			-- dolu satiri sag alt diagoneli
	- her konum icin get next cagirilarak o konumundan baslayarak diagonel boyunca 1ler ile dolduruldu.
	- eger en kosedeki elemana ulasilabildiyse mesafe 1 dir o zaman true donduruldu.
	- eger en kosedeki elemana ulasilamadi ise bu sefer her ulasilan noktadan yine 3 farki yone gidilebilir.
	- o nedele call_answer her ulasilan nokta icin yeniden cagirildi.
	- bu sefer value olarak 2 girildi ve 2 degerli matris bolumleri doldurulmus oldu.
	- islem boyunca sadece 1 ve 2 olan degerler ve ilk gorulen 3 degerleri yerlestirildi. Daha fazla ilerleme yapilmadi.
	- dagonel uzerinden ilerlendi.
	parametreler:
		- matrix: dinamik programlama icin kullanilan matris
		- text1 : Mesafe aranacak 1. kelime
		- text2 : Mesafe aranacak 2. kelime
		- it1,it2 : Daha onceden ulasilan iterator degerleri
		- value: Diagonali dolduracak deger
	
	return 
		- Mesafe 1 için true diger durumlarda false dondurur. 
*/ 
bool call_answer(int** matrix,char* text1,char* text2,int it1,int it2,int value){
	int i,j;											// iterator
	bool answer;										// Farkin 1 olmasi durumunda true olan degisken;
	i=it1+1;
	j=it2+1;
	answer = get_next(matrix,text1,text2,&i,&j,value);
	if(answer)  
		return true;
	if(value==1)
		call_answer(matrix,text1,text2,i,j,2);
	i=it1+1;
	j=it2;
	answer = get_next(matrix,text1,text2,&i,&j,value);
	if(answer) 
		return true;
	if(value==1)
		call_answer(matrix,text1,text2,i,j,2);
	i=it1;
	j=it2+1;
	answer = get_next(matrix,text1,text2,&i,&j,value);
	if(answer) 
		return true;
	if(value==1)
		call_answer(matrix,text1,text2,i,j,2);
	return false;
}


/* Tum hash tablosunu gezerek hesaplanan Levenshtein Edit Distance'a gore stringleri olusturan fonksiyon.
	- sozluk tablosunda bos (degeri '*') olmayan tum elemanlar gezildi.
	- verilen kelime ile sozlukteki kelime arasindaki Levenshtein Edit Distance hesaplandi.
	- sonuc degeri 1 ise one_liste 2 ise two_liste eklendi ve aralara or eklendi.
	- son or silindi.
	- one_list doluysa one_list, bossa two_list, ikisi de bossa null donduruldu.
	- kullanilmayanlar dealloke edildi.
	
	parametreler
		- word: aranacak kelime
		- dictionary_table : kelimelerin aranacagi dictionary hash tablosu
	return 
		- Varsa 1 mesafeli kelimeleri yoksa 2 mesafeli kelimeleri o da yoksa NULL döndürür.
*/ 
char* find_alternate_list(char* word,char** dictionary_table){	
	int i;															// iterator 
	int distance;													// Levenshtein Edit Distance'i tutan degisken  
	char* one_list = (char*)malloc(sizeof(char)*RECOMMEND_SIZE);	// mesafenin 1 oldugu sonuclari tutan string
	char* two_list = (char*)malloc(sizeof(char)*RECOMMEND_SIZE);	// mesafenin 2 oldugu sonuclari tutan string
	strcpy(one_list,"");
	strcpy(two_list,"");
	
	for(i=0;i<TABLE_SIZE;i++){
		if(dictionary_table[i][0] != '*'){
			distance = find_distance(word,dictionary_table[i]);
			//Matrisi doldurarak soruyu cozen metot
			//distance = find_distance2(word,dictionary_table[i]);
			if(distance == 1){
				strcat(one_list,dictionary_table[i]);	
				strcat(one_list," or ");
			}
				
			else if(distance == 2){
				strcat(two_list,dictionary_table[i]);
				strcat(two_list," or ");
			}
		}
	}
	if(strlen(one_list)==0&&strlen(two_list)==0){
		free(one_list);
		free(two_list);
		return NULL;
	}
	else if(strlen(one_list)==0){
		free(one_list);
		two_list[strlen(two_list) - 4] = '\0';
		return two_list;
	}	
	else{
		free(two_list);
		one_list[strlen(one_list) - 4] = '\0';
		return one_list;
	}
}

 
/* Verilen karakteri lowercase yapan fonksiyon
	parametreler:
		- c: lowercase yapilacak karakter
 	return:
	 	- Karakterin lowercase degeri
*/ 
char lowercase(char c){
	if(c>='A' && c<='Z')
		return c+'a'-'A';
	else
		return c;
}

 
/* Case sensitive kurali gozetmeden 2 karakteri karsilastiran fonksiyon
	parametreler:
		- c1: karsilastirilacak 1. karakter
		- c2: karsilastirilacak 2. karakter
 	return:
	 	- karakterler case sensitive olarak ayni mi degil mi
*/ 
bool compare_char(char c1,char c2){
	return lowercase(c1) == lowercase(c2);
}

/* Case sensitive kurali gozetmeden 2 stringi karsilastiran fonksiyon
	parametreler:
		- s1: karsilastirilacak 1. string
		- s2: karsilastirilacak 2. string
 	return:
	 	- stringler case sensitive olarak ayni mi degil mi
*/ 
bool compare_string(char* s1, char* s2){
	int i = 0;
	while(s1[i]!='\0' && s2[i]!='\0' && compare_char(s1[i],s2[i]))
		i+=1;
	if(s1[i]=='\0' && s2[i]=='\0')
		return true;
	else
		return false;
}

/* Verilen karakterin alfabe sirasono belirleyen fonksiyon
	parametreler:
		- c: alfabe sirasi bulunacak karakter
 	return:
	 	- Karakterin alfabedeki sirasi
*/
int order(char c){
	if(c>='A' && c<='Z')
		return c-'A';
	else if(c>='a' && c<='z')
		return c-'a';
	else
		return (int)c;
}

/* Verilen stringin karakterleriyle horner degerini hesaplayan fonksiyon
	parametreler:
		- string: horner degeri hesaplanacak string
 	return:
	 	- Stringin horner degeri
*/
size_t horner(char* string){
	int i;									// iterator
	size_t sum = 0;							// horner toplami
	size_t size = strlen(string);				// stringin boyutunu tutan degisken
	for(i = 0;i<size;i++){
		sum = sum * R + order(string[i]);
	}
	return sum;
}

/* double hashing metodu ile verilen string ve i degerine gore hash degeri hesaplama fonksiyonu
	parametreler:
		- string: horner degeri hesaplanacak string
		- i: hash 2 icin kullanilan iterator
 	return:
	 	- Stringing double hasing degeri
*/
int double_hashing(char* string,int i){
	size_t horn;								// stringing horner degerini saklayan degisken 
	int h1,h2;									// 1. ve 2. hash degerlerini saklayan degiskenler							
	horn = horner(string);
	h1 = horn%TABLE_SIZE;
	h2 = 1+ horn%(TABLE_SIZE-1);
	return (h1 + i*h2) % TABLE_SIZE;
}

/* bos elemanlarla duzeltme tablosu olusturan fonksiyon
 	return:
	 	- Bos duzeltme hash tablosu
*/
correction* create_empty_correction(){
	correction* hash_table = (correction*)malloc(sizeof(correction)*TABLE_SIZE); // yeni duzeltme hash tablosu
	int i;																		 // iterator
	for(i = 0;i<TABLE_SIZE;i++){
		strcpy(hash_table[i].original,"*");
	}
	return hash_table;
}

/* verilen stringin duzeltme tablosunda olup olmadini bulup varsa -1 yoksa eklenebilecegi index degerini donduren fonksiyon
	parametreler:
		- string: duzeltme tablosunda aranan string
		- correction_table: duzeltme hash tablosu
 	return:
	 	- String tabloda varsa -1 yoksa eklenebilecegi index degeri
*/
int search_correction_table(char* string, correction* correction_table){
	int index = double_hashing(string,0); 										 // double hashing degerini tutan degisken
	int i = 1;																	 // iterator	
	//Eger dizide zaten varsa ekleme yapma
	if (compare_string(correction_table[index].original,string)){
		return -1;
	}
	while(i<TABLE_SIZE && correction_table[index].original[0] != '*'){
		index = double_hashing(string,i);
		if (compare_string(correction_table[index].original,string)){
			return -1;
		}
		i++;	
	}
	return index;
}

/* search_correction_table fonksiyonundan -1 degeri donmemisse o lokasyona yeni elemani kaydeden fonksiyon
	parametreler:
		- original: duzeltme tablosunda eklenecek stringin ilk yazildigi hali
		- original: duzeltme tablosunda eklenecek stringin duzeltilmis hali
		- correction_table: duzeltme hash tablosu
*/
void insert_correction_table(char* original,char* corrected, correction* correction_table){
	int index = search_correction_table(original,correction_table);              // eleman varsa -1 yoksa eklenmesi gereken indexi veren fonksiyon
	if(index != -1){
		strcpy(correction_table[index].original,original);
		strcpy(correction_table[index].corrected,corrected);
	}
}

/* search_correction_tableye benzer sekilde hash tablosunda eleman arayan ancak ondan farkli olarak oldugu lokasyonu donduren fonksiyon
	parametreler:
		- string: duzeltme tablosunda aranan string
		- correction_table: duzeltme hash tablosu
 	return:
	 	- String tabloda varsa indexi yoksa -1 degeri
*/
int find_correction_index(char* string, correction* correction_table){
	int index = double_hashing(string,0); 										 // eleman varsa bulundugu indexi tutan degisken
	int i = 1;																	 // iterator
	//Eger dizide zaten varsa ekleme yapma
	if (compare_string(correction_table[index].original,string)){
		return index;
	}
	while(i<TABLE_SIZE && correction_table[index].original[0] != '*'){
		index = double_hashing(string,i);
		if (compare_string(correction_table[index].original,string)){
			return index;
		}
		i++;	
	}
	return -1;
}

/* bos elemanlarla sozluk tablosu olusturan fonksiyon
 	return:
	 	- Bos sozluk hash tablosu
*/
char** create_empty_dictionary(){
	char** hash_table = (char**)malloc(sizeof(char*)*TABLE_SIZE);			// yeni dictionary hash tablosu
	int i;																	// iterator
	for(i = 0;i<TABLE_SIZE;i++){
		hash_table[i] = (char*) malloc(sizeof(char)*WORD_SIZE);
		strcpy(hash_table[i],"*");
	}
	return hash_table;
}

/* verilen stringin sozluk tablosunda olup olmadini bulup varsa -1 yoksa eklenebilecegi index degerini donduren fonksiyon
	parametreler:
		- string: sozluk tablosunda aranan string
		- dictionary_table: sozluk hash tablosu
 	return:
	 	- String tabloda varsa indexi yoksa -1 degeri
*/
int search_dictionary_table(char* string, char** dictionary_table){
	int index = double_hashing(string,0); 									// double hashing degerini tutan degisken
	int i = 1;																// iterator
	//Eger dizide zaten varsa ekleme yapma
	if (compare_string(dictionary_table[index],string)){
		return -1;
	}
	while(i<TABLE_SIZE && dictionary_table[index][0] != '*'){
		index = double_hashing(string,i);
		if (compare_string(dictionary_table[index],string)){
			return -1;
		}
		i++;	
	}
	return index;
}

/* search_correction_table fonksiyonundan -1 degeri donmemisse o lokasyona yeni elemani kaydeden fonksiyon
	parametreler:
		- string: sozluk tablosuna eklenecek string
		- dictionary_table: sozluk hash tablosu

*/
void insert_dictionary_table(char* string, char** dictionary_table){
	int index = search_dictionary_table(string,dictionary_table);			// eleman varsa -1 yoksa eklenmesi gereken indexi veren fonksiyon
	if(index != -1)
		strcpy(dictionary_table[index],string);
}

/* dictionary dosyasini okuyup hash tablosuna kaydeden fonksiyon
	parametreler:
		- dictionary_table: sozluk hash tablosu
 	return:
	 	- Dosyanin basari ile okunup okunmadigini donduren deger
*/
bool read_dictionary(char** dictionary_table){
	FILE* file;																// dosyayi tutan pointer
	char string[WORD_SIZE];													// kelime icin olustural buffer degiskeni
	char wordSize[12], buff[10];											// word_size degiskenini stringe cevirmek icin kullanilan degiskenler (string hali fscanfde kullanildi)
	snprintf( buff, 10, "%d", WORD_SIZE );
	strcpy(wordSize,"%");
	strcat(wordSize,buff); 
	strcat(wordSize,"s");
	file = fopen(dictionary_file_name,"r");
	if (!file){
		printf("!! Missing Dictionary (Name: %s) !!\n",dictionary_file_name);
		return false;	
	}
	
	while (fscanf(file, wordSize, string) == 1) {
        insert_dictionary_table(string,dictionary_table);
    }
	fclose(file);
	return true;
}

/* aranan kelimenin dictionary icinde olup olmadigini search_dictionary_table ile hesaplayan fonksiyon
	parametreler:
		- word: sozluk tablosunda aranacak string
		- dictionary_table: sozluk hash tablosu
 	return:
	 	- Kelimenin dictionary tablosunda olup olmadiginin degeri
*/
bool in_dictionary(char* word,char** dictionary_table){
	if (search_dictionary_table(word,dictionary_table) == -1)
		return true;
	else
		return false;
}

/* verilen texti kelime dizisine bolen fonksiyon
	parametreler:
		- input: parcanalanacak string
 	return:
	 	- parcalanmis string dizisi
*/
char** generate_words(char* input){														
	char** words = (char**)malloc(sizeof(char*)*WORD_COUNT); // kelime dizisini saklayan degisken
	char* buff;												 // strtok icin kullanilan buffer
	int i;													 // iterator
	for(i=0;i<WORD_COUNT;i++)
		words[i]=(char*)malloc(sizeof(char)*WORD_SIZE);      
	buff = strtok(input," ");
	i = 0;
	strcpy(words[i],buff);
	while(buff != NULL ) {
	  	i++;
      	buff = strtok(NULL, " ");
      	if(buff != NULL)
      		strcpy(words[i],buff);
   	}
   	words[i] = NULL;
   	return words;
}

/* 4 sayi icinde 0dan farkli en kucugunu bulan fonksiyon
	parametreler:
		- a,b,c,d: girilen sayilar
 	return:
	 	- 0 haric en kucuk deger
*/
int find_min(int a,int b,int c, int d){
	int min = INT_MAX;  				// 4 Sayi arasinda Mininum degeri tutar
	if(a!=0 && a<min) min = a;
	if(b!=0 && b<min) min = b;
	if(c!=0 && c<min) min = c;
	if(d!=0 && d<min) min = d;
	if(min==INT_MAX) return 0;
	return min;
}


/* verilen kelime listesini yazdiran fonksiyon
	parametreler:
		- words: kelimelerden olusan dizi
*/
void print_result(char** words){
	int i=0; 						//iterator
	while(words[i]!=NULL){
		printf("%s ",words[i]);
		i++;
	}
	printf("\n\n");
}

/* verilen kelime listesini dealloke eden fonksiyon
	parametreler:
		- words: kelimelerden olusan dizi
		- size: dizinin alloke edilen boyutu
*/

void deallocate_string_list(char** words,int size){
	int i;
	for(i=0;i<size;i++){
		free(words[i]);
	}
	free(words);
}

/* verilen integer matrisi dealloke eden fonksiyon
	parametreler:
		- matrix: 2D integer matrisi
		- size: matrisin satir sayisi
*/
void deallocate_matrix(int** matrix,int size){
	int i;
	for(i=0;i<size;i++){
		free(matrix[i]);
	}
	free(matrix);
}
