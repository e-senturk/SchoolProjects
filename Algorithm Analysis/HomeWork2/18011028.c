#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Tablo boyutu 1000'e en yak�n asal say� olarak se�ildi
#define TABLE_SIZE 997

// Horner de�eri hesaplan�rken �ss� al�nacak say�
#define R 31

// Struct Yap�lar�
/* 
Liste yap�s� document nesnelerini saklamak i�in olu�turulan struct yap�s�

parametreler:	
	value = Listenin o eleman�n�n i�inde saklanacak string de�erini tutar
	next = Sonraki eleman�n pointer de�erini tutar
*/
struct list{
	char* value;
	struct list* next;
};
typedef struct list list;

/* 
Word yap�s� herhangi bir kelimeyi ve kelimenin ge�ti�i d�k�manlar� saklamak i�in olu�turulan struct yap�s�

parametreler:
	string = Kelimenin i�eri�ini saklamak i�in olu�turulan yap�
	documents = Kelimenin ge�ti�i d�k�manlar�n listesini saklayan linkli liste
*/
struct {
	char* string;
	list* documents;
}typedef word;

// Fonksiyon Headerlar�

// String Fonksiyonlar� 
int order(char character);
char lowercase(char character);
bool compare_char(char character1,char character2);
bool compare_string(char* string1, char* string2);

// Liste Fonksiyonlar�
list* create_list(char* value);
bool find_list(list* head, char* value);
list* add_front_list(list* head, char* value);

// Kelime Fonksiyonlar�
word create_word(char* string,char* document);
word add_document(word *element,char* document);
bool is_in_document(word element,char*document);
void print_word(word element);

// Hash Fonksiyonlar�
word* create_new_hash();
int horner(char* text);
int double_hashing(char* string,int i);
int search_hash_table(char* string, word* hash_table,int* step);
void insert_hash_table(char* string, char* document, word* hash_table,int* load_factor);
void find_word(char* string,word* hash_table);

// Input/Output Fonksiyonlar�
void print_info();
bool write_database(word* hash_table,int load_factor);
word* read_database(int* load_factor);
bool read_document(char* file_name, word* hash_table,int *load_factor);
void deallocate();

// Ana Fonksiyon
/* 
Kullan�c�dan al�nan inputlara g�re dosyalar� hash tablosuna ekleyip dosyalar �zerinde arama yapan fonksiyon

parametreler:
	hash_table = hash tablosunu tutan dizi
	load_factor = tablodaki dolu eleman say�s�n� tutan de�i�ken
	select = kullan�c�dan input al�rken kullan�lan karakter
	file_name = okunacak dosya ad�n� tutan string
	string = aranacak kelimeyi tutan string
*/
int main(int argc, char *argv[]) {
	word* hash_table;
	int load_factor;
	char select = 0;
	char file_name[100];
	char string[100];
	hash_table = read_database(&load_factor);
	printf("\tHash Algoritmasi ile Kelime Arama Programi\n");
	print_info();
	while (select != 'Q' && select != 'q'){
		scanf("%c",&select);
		if(select == '1'){
			printf("Dosya ekleme islemi secildi\n");
			printf("Lutfen dosya adini girin: ");
			scanf("%s",file_name);
			read_document(file_name,hash_table,&load_factor);
		}
		else if(select == '2'){
			printf("Kelime arama islemi secildi\n");
			printf("Lutfen aranacak kelimeyi girin: ");
			scanf("%s",string);
			find_word(string,hash_table);
		}
		else if(select == '3'){
			load_factor = 0;
			hash_table = create_new_hash();
			printf("Veritabani basariyla temizlendi.\n");
		}
		else if(select == '4'){
			system("cls");
		}
		else if(select != 'Q' && select != 'q'){
			print_info();
		}
	}
	deallocate(hash_table);
	return 0;
}

// Yard�mc� fonksiyonlar

/* 
Verilen karakterin alfabetik olarak s�ras�n� 0 dan ba�layacak �ekilde hesaplayan fonksiyon
E�er karakter alfabede yoksa ascii tablosundaki de�eri kullan�ld�

parametreler:
	c = verilen karakter
	return = karakterin alfabetik s�ras�n�n integer de�eri
*/
int order(char c){
	if(c>='A' && c<='Z')
		return c-'A';
	else if(c>='a' && c<='z')
		return c-'a';
	else
		return (int)c;
}

/* 
Verilen karakteri lowercase yapan fonksiyon

parametreler:
	c = verilen karakter
	return = karakterin lowercase hali
*/
char lowercase(char c){
	if(c>='A' && c<='Z')
		return c+'a'-'A';
	else
		return c;
}

/* 
Case sensitive kural� g�zetmeden 2 karakteri kar��la�t�ran fonksiyon

parametreler:
	c1 = 1. karakter
	c2 = 2. karakter
	return = karakterlerin e�it olup olmad���n� d�nd�ren boolean de�eri
*/
bool compare_char(char c1,char c2){
	return lowercase(c1) == lowercase(c2);
}

/* 
Case sensitive kural� g�zetmeden 2 stringi kar��la�t�ran fonksiyon

parametreler:
	s1 = 1. string
	s2 = 2. string
	i = iterasyon i�in kullan�ld�
	return = stringlerin e�it olup olmad���n� d�nd�ren boolean de�eri
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

/* 
Listenin head nodunu verilen de�ere g�re olu�turan fonksiyon

a��klama: Head noduna memory allocation ile yer a��ld�
	Value i�in memory allocation ile eklenen kelime kadar yer a��ld�
	Head de�erinin i�ine de�er olarak value de�eri kaydedildi
	Head nodunun next nodu olarak NULL atand�
	Sonu� olarak olu�turulan head de�erine geri d�n�� yap�ld�

parametreler:
	value = listenin head noduna eklenecek kelime
	return = olu�turlan yeni listenin head nodu
*/
list* create_list(char* value){
	list* head = (list*) malloc(sizeof(list));
	head->value = (char*) malloc(sizeof(char)*strlen(value));
	strcpy(head->value,value);
	head->next = NULL;
	return head;
}

/* 
Verilen liste i�inde kelime arayan fonksiyon

a��klama: Verilen liste i�erisindeki t�m nodlar son noda kadar itere edildi
	E�er son noda ula��lmadan kelime bulunursa true, son noda ula��l�rsa 
	false d�nd�r�ld�

parametreler:
	head = listenin head nodu
	value = aranacak kelime
	return = kelimenin bulup bulunmad���n�n bilgisini i�eren boolean de�eri
*/
bool find_list(list* head, char* value){
	while(head != NULL && strcmp(head->value,value)!=0){
		head = head->next;
	}
	if(head == NULL)
		return false;
	else
		return true;
}

/* 
listenin ba��na eleman ekleyen fonksiyon

a��klama: Listenin i�inde find_list fonksiyonu kullan�larak d�k�man ad� arand�
	E�er d�k�man yoksa yeni bir head nodu olu�turuldu ve listenin ba��na eklendi
	E�er ilk head node bo�sa yeni olu�turlan node head olarak atand� eski head node silindi
	E�er ilk head node doluysa yeni head nodun sonras�na eski head nodu atand�
	Yeni head nodu d�nd�r�ld�

parametreler:
	head = listenin head nodu
	new_head = listenin yeni head_nodu;
	value = aranacak kelime
	return = kelimenin bulup bulunmad���n�n bilgisini i�eren boolean de�eri
*/
list* add_front_list(list* head, char* value){
	if(find_list(head,value)){
		return head;
	}
	list* new_head = create_list(value);
	if(strlen(head->value)==0){
		free(head);
	}
	else{
		new_head->next = head;
	}
	return new_head;
}

/* 
Verilen kelime ve d�k�man i�in yeni kelime olu�turan fonksiyon

a��klama: Word tipinde bir struct olu�turuldu
	Memory allocation ile bu structa yer a��ld�
	Yeni kelime word struct yap�s�na kopyaland�
	Word yap�s�ndaki listeye d�k�man yeni liste olu�tularak eklendi
	Sonu� olarak olu�turulan struct yap�s�na d�n�ld�

parametreler:
	string = eklenecek kelime
	document = kelimenin bulundu�u d�k�man�n ad�
	new_word = kelime ve d�k�man�n i�eri�inin bulundu�u struct yap�s�
	return = yeni olu�turulan struct yap�s�
*/
word create_word(char* string,char* document){
	word new_word;
	new_word.string = (char*) malloc(sizeof(char)*strlen(string));
	strcpy(new_word.string,string);
	new_word.documents = create_list(document);
	return new_word;
}

/* 
Verilen kelime ve d�k�man i�in yeni kelime olu�turan fonksiyon

a��klama: Word tipinde bir struct olu�turuldu
	Memory allocation ile bu structa yer a��ld�
	Yeni kelime word struct yap�s�na kopyaland�
	Word yap�s�ndaki listeye d�k�man yeni liste olu�tularak eklendi
	Sonu� olarak olu�turulan struct yap�s�na d�n�ld�

parametreler:
	string = eklenecek kelime
	document = kelimenin bulundu�u d�k�man�n ad�
	new_word = kelime ve d�k�man�n i�eri�inin bulundu�u struct yap�s�
	return = yeni olu�turulan struct yap�s�
*/
word add_document(word *element,char* document){
	element->documents = add_front_list(element->documents,document);
	return *element;
}

/* 
Kelimenin i�inde herhangi bir d�k�man�n olup olmad���n� arayan yard�mc� fonksiyon

a��klama: Verilen bir word struct� i�indeki d�k�man listesinde verilen d�k�man�n
	olup olmadu��n� find_list fonksiyonunu kullanarak arand�

parametreler:
	word = arama yap�lacak kelime structu
	document = aranan d�k�man
	return = d�k�man�n o word structunda olup olmad���n� g�steren boolean de�i�keni
*/
bool is_in_document(word element,char*document){
	return find_list(element.documents,document);
}

/* 
Tablo boyutuna g�re hash tablosunu bo� elemanlarla olu�turan fonksiyon

a��klama: Define ile belirlenen tablo boyutunda word structundan olu�an bir dizi memory allocation ile olu�turuldu
	Her eleman tek tek memory allocation ile olu�turulmas� gerekti�inden create_word fonksiyonu �a��r�ld�
	�a��r�lan fonksiyona kelime "*" olarak d�k�man ad� ise bo� olarak atand�
	write_database fonksiyonu ile database dosyas� g�ncellendi
	D�n�� de�eri olarak olu�turulan tabloya d�n�ld�  

parametreler:
	hash_table = yeni olu�turulan hash tablosu
	i = iterasyon i�in kullan�ld�
	return = yeni olu�turulan hash tablosu
*/
word* create_new_hash(){
	word* hash_table = (word*)malloc(sizeof(word)*TABLE_SIZE);
	int i;
	for(i = 0;i<TABLE_SIZE;i++){
		hash_table[i] = create_word("*","");
	}
	write_database(hash_table,0);
	return hash_table;
}

/* 
Verilen bir texti horner metodu ile integera �eviren fonksiyon

a��klama: Verilen bir text bir for loopu ile ba�tan sona do�ru horner metodu
	kullan�larak elde edilen toplam her ad�mda R ile �arp�larak karakterin farkl� katsay�da toplama etki etmesi
	sa�land� bulunan toplam negatif ��karsa (toplam�n integer boyutunu ge�me ihtimali y�z�nden) sonu� -1 ile �arp�ld�.

parametreler:
	text = verilen text de�eri 
	i = iterasyon i�in kullan�ld�
	sum = horner i�in hesaplanan toplam� tutan de�i�ken
	size = verilen de�i�kenin
	return = bulunan toplam sonucunun pozitif de�eri bulunan integer de�eri 
*/
int horner(char* text){
	int i;
	int sum = 0;
	int size = strlen(text);
	for(i = 0;i<size;i++){
		sum = sum * R + order(text[i]);
	}
	if(sum<0){
		sum = -sum;
	}
	return sum;
}

/* 
Verilen bir stringi verilen bir integer de�eri ile birlikte double hashing i�leminden ge�iren fonksiyon

a��klama: String horner metoduna g�re bir integer say�ya d�n��t�r�ld�
	Tablo boyutuna g�re h1 ve h2 fonksiyonlar� double hashing i�in olu�turuldu
	h2 de�eri i de�eri ile �arp�l�p h1 de�eri ile topland� ve bulunan sonucun tablo boyutuna g�re modu al�nd�
	Kullan�c�ya bu sonu� de�eri d�nd�r�ld�
	
parametreler:
	string = verilen string de�eri 
	i = h2 de�eri katsay�s�
	horn = horner metoduyla hesaplanan integer say�
	h1 = h1 fonksiyonunun sonucunu tutan de�i�ken
	h2 = h2 fonksiyonunun sonucunu tutan de�i�ken
	hash = hesaplanan hash de�erini tutan de�i�ken 
	return = hesaplanan hash de�eri
*/
int double_hashing(char* string,int i){
	int hash,h1,h2,horn;
	horn = horner(string);
	h1 = horn%TABLE_SIZE;
	h2 = 1+ horn%(TABLE_SIZE-1);
	hash = (h1 + i*h2) % TABLE_SIZE;
	return hash;
}

/* 
Hash tablosu i�inde verilen bir stringin ge�ip ge�medi�ini bulan ge�iyorsa o indisi ge�miyorsa 
bulunan ilk bo� indisi d�nd�ren fonksiyon

a��klama: Aran�lan stringin double_hashing fonksiyonu ile hash de�eri i=0 i�in hesapland�
	i de�i�keni step de�i�keninde daha sonra ka� ad�mda i�lem yap�ld���n� bulmak i�in sakland�
	E�er buldu�umuz hash de�erindeki string arad���m�z stringle ayn� ise o index de�eri d�nd�r�ld�
	E�er e�it de�ilse bo� hash eleman� buluncaya kadar i de�eri artt�r�larak sonraki hash de�erlerine bak�ld�
	E�er eleman herhangi bir hash de�erinde bulunursa o indexe d�nd�r�ld�
	E�er bo� eleman bulunursa da yine o indexe d�nd�r�l�d� bu sayede sonraki eleman� yerle�tirebilece�imiz indexi de bulmu� olduk
		
parametreler:
	string = verilen string de�eri 
	hash_table = hash tablosu
	step = i�lem ad�m say�s�
	index = hesaplanan hash de�erini tutan de�i�ken
	i = iterasyon i�in kullan�ld�
	return = hesaplanan index de�eri
*/
int search_hash_table(char* string, word* hash_table,int* step){
	int index = double_hashing(string,0); 
	int i = 1;
	*step = i;
	if (compare_string(hash_table[index].string,string)){
		return index;
	}
	while(i<TABLE_SIZE && hash_table[index].string[0] != '*'){
		index = double_hashing(string,i);
		if (compare_string(hash_table[index].string,string)){
			*step = i;
			return index;
		}
		i++;	
	}
	*step = i;
	return index;
}

/* 
Hash tablosuna verilen stringi yerle�tiren fonksiyon

a��klama: Verilen kelimenin bulundu�u veya koyulmas� gereken hash de�eri search_hash_table fonksiyonu ile hesapland�
	tablo dolu ise eleman eklenemedi ve eklenemeyen kelime kullan�c�ya yazd�r�ld�
	tablo %80 dolu ise kullan�c�ya tablonun doldu�una dair uyar� verildi
	e�er bulunan index bo� ise yeni kelime ve d�k�man ad� o indexe yerle�tirildi
	her yeni kelime eklendi�inde doluluk oran� kullan�c�ya yazd�r�ld�
	e�er index dolu ise sadece d�k�man ad� o indexe eklendi
	
parametreler:
	string = verilen string de�eri
	document = d�k�man ad� 
	hash_table = hash tablosu
	load_factor = tablodaki dolu eleman say�s�
	step = i�lem ad�m say�s�
	index = hesaplanan hash de�erini tutan de�i�ken
	ratio = tablonun doluluk oran�n� saklayan de�i�ken
*/
void insert_hash_table(char* string, char* document, word* hash_table,int* load_factor){
	int step = 0;
	int index = search_hash_table(string,hash_table,&step);
	float ratio;
	if (*load_factor >= TABLE_SIZE){
		printf("Tablo Dolu: %s kelimesi eklenemedi.\n",string);
	}
	else{
		if (hash_table[index].string[0] == '*'){
			hash_table[index].string = (char*) malloc(sizeof(char)*strlen(string));
			strcpy(hash_table[index].string,string);
			*load_factor+=1;
			ratio = (float)*load_factor/TABLE_SIZE;
			printf("- %s kelimesi %d adimda eklendi - ",string,step);
			if(ratio>=0.8){
				printf("Tablo Dolmak Uzere - ");
			}
			printf("Load Factor: %0.3f\n",ratio);
		}
		add_document(&hash_table[index],document);
	}
}

/* 
Aranan kelimeyi hash tablosunda bulup hangi dosyalarda oldu�unu yazd�ran fonksiyon

a��klama: Verilen kelimenin hash de�eri search_hash_table fonksiyonu ile hesapland�
	Step de�eri ile bu i�lemin ka� ad�mda hesapland��� yazd�r�ld�
	E�er d�nd�r�len indexteki kelime hatal�ysa kelime bulunamad���na dair kullan�c�ya ��kt� verildi
	E�er indexde eleman varsa kelimenin bulundu�una dair kullan�c�ya ��kt� verildi
	��lem ad�m say�s� kullan�c�ya verildi
		
parametreler:
	string = verilen string de�eri
	hash_table = hash tablosu
	step = i�lem ad�m say�s�n� tutan de�i�ken
	index = hesaplanan hash de�erini tutan de�i�ken
*/

void find_word(char* string,word* hash_table){
	int step = 0;
	int index = search_hash_table(string,hash_table,&step);
	if(compare_string(hash_table[index].string,string)){
		list* tmp = hash_table[index].documents;
		printf("- %s kelimesi %d adimda, ",string,step);
		while(tmp->next!=NULL){
			printf("%s, ",tmp->value);
			tmp = tmp->next;
		}
		printf("%s dosyalarinda bulundu.\n",tmp->value);
	}	
	else
		printf("- %s kelimesi bulunamadi. Islem %d adim surdu.\n",string,step);
}

/* 
Kullan�c�ya input hakk�nda bilgi yazd�ran fonksiyon
*/
void print_info(){
	printf("\nLutfen Yapilacak Islemi Secin:\n");
	printf("1. Dosya Ekle\n");
	printf("2. Kelime Ara\n");
	printf("3. Veritabanini Temizle\n");
	printf("4. Ekrani Temizle\n");
	printf("Q. Cikis\n");
	printf("Input: ");
}

/* 
Veritaban�ndan hash tablosunu ve load fakt�r� yazan fonksiyon

a��klama: load_factor yazd�r�lmak �zere string de�erine �evrildi
	Database dosyas�na veritaban� dosyas� yazma modunda a��ld� a��lamazsa false d�nd�r�ld�
	Dosya d�zeni a��s�ndan verilerin neye ait oldu�u da dosyaya yazd�r�ld�
	�lk ba�ta load fakt�r sonras�nda ise hash tablosundaki elemanlar s�ras� ile dosyaya yazd�r�ld�
	Hash tablosunda her sat�r� ay�rt etmek i�in sat�r ba��na # de�eri konuldu
	Sat�r�n ilk eleman� olarak kelime de�eri sonraki elemanlar� olarak dosya adlar� s�ras� ile yazd�r�ld�
	Dosya yazma sonras�nda kapat�ld�
		
parametreler:
	hash_table = yazd�r�lacak hash tablosu
	load_factor = tablodaki dolu eleman say�s�
	i = iterasyon i�in kullan�ld�
	database = yazd�r�lacak veritaban� dosyas�n� saklayan pointer
	load_str = load fakt�r�n string de�eri
	return = yaz�m i�leminin ba�ar�l� veya ba�ar�s�z oldu�unu ifade eden boolean de�eri
*/
bool write_database(word* hash_table,int load_factor){
	int i;
	FILE* database;
	char load_str[12];
	sprintf(load_str, "%d", load_factor);
	database = fopen("18011028.txt","w");
	if (!database)
		return false;
	fputs("LOAD FACTOR: \n",database);
	fputs(load_str,database);
	fputc('\n',database);
	fputs("HASH TABLE: \n",database);
	for(i=0;i<TABLE_SIZE;i++){
		fputs("# ",database);
		fputs(hash_table[i].string,database);
		fputc(' ',database);
		list* tmp = hash_table[i].documents;
		while(tmp!=NULL){
			fputs(tmp->value,database);
			fputc(' ',database);
			tmp = tmp->next;
		}
		fputc('\n',database);
	}
	fclose(database);
	return true;
}


/* 
Veritaban�ndan hash tablosunu ve load fakt�r� okuyan fonksiyon

a��klama: Hash tablosu i�in memory alan� olu�turuldu
	Veritaban� dosyas� okundu e�er bulunamazsa create_new_hash fonksiyonu ile bo� veri taban� dosyas� olu�turuldu
	Veri a��s�ndan bilgilendirici sat�rlar g�rmezden gelindi
	Load factor string olarak okunup atoi fonksiyonu ile integera �evrildi
	S�ras� ile her sat�r ba��ndaki # de�erine g�re ayr��t�r�ld�
	Sat�r�n ilk eleman� kelime olarak di�er elemanlar� ise d�k�man olarak okundu ve ilgili fonksiyonlarla
	word structu olu�turuldu
	Olu�turulan structlar hash tablosuna s�ras� ile eklendi
	Dosya okuma sonras�nda kapat�ld�

		
parametreler:
	load_factor = tablodaki dolu eleman say�s�
	i = iterasyon i�in kullan�ld�
	hash_table = olu�tutulacak hash tablosu
	database = okunacak veritaban� dosyas�n� saklayan pointer
	load_str = load fakt�r�n string de�eri
	temp,word = ge�ici olarak input de�erlerini saklayan bufferlar
	string = okunan kelime
	return = okuma sonucu olu�an hash tablosu
*/
word* read_database(int* load_factor){
	int i;
	word* hash_table = (word*)malloc(sizeof(word)*TABLE_SIZE);
	word new_word;
	FILE* database;
	char load_str[12];
	char temp[100];
	char word[100];
	database = fopen("18011028.txt","r");
	if (!database){
		printf("Eski veritabani bulunamadi sifirdan olusturuluyor...\n\n");
		return create_new_hash();
	}	
	fgets(temp, 100, database);
	fgets(load_str, 12, database);
	*load_factor = atoi(load_str);
	fgets(temp, 100, database);
	fscanf(database, " %100s", temp);
	for(i=0;i<TABLE_SIZE;i++){
		fscanf(database, " %100s", word);
		new_word = create_word(word,"");
		while(fscanf(database, " %100s", temp)==1 && strcmp(temp,"#")!=0){
			add_document(&new_word,temp);
		}
		hash_table[i] = new_word;
	}
	fclose(database);
	return hash_table;
}

/* 
Herhangi bir dosyay� okuyup i�indeki kelimeleri hash tablosuna kaydeden fonksiyon

a��klama: Verilen kelimenin hash de�eri search_hash_table fonksiyonu ile hesapland�
	Step de�eri ile bu i�lemin ka� ad�mda hesapland��� yazd�r�ld�
	E�er bo� index bulduysak kelime bulunamad���na dair kullan�c�ya ��kt� verildi
	E�er indexde eleman varsa kelimenin bulundu�una dair kullan�c�ya ��kt� verildi
	��lem ad�m say�s� kullan�c�ya verildi
		
parametreler:
	file_name = okunacak dosyan�n ad�
	hash_table = olu�tutulacak hash tablosu
	load_factor = tablonun doluluk say�s�
	file = okunacak dosyay� saklayan pointer
	string = okunan kelime
	return = dosya okuma i�leminin ba�ar�l� veya ba�ar�s�z oldu�unu g�steren boolean de�er
*/
bool read_document(char* file_name, word* hash_table,int *load_factor){
	FILE* file;
	char string[100];
	file = fopen(file_name,"r");
	if (!file){
		printf("Dosya bulunamadi\n");
		return false;	
	}
	while (fscanf(file, " %100s", string) == 1) {
        insert_hash_table(string,file_name,hash_table,load_factor);
    }
	fclose(file);
	write_database(hash_table,*load_factor);
	printf("Dosya basari ile eklendi\n");
	return true;
}

/* 
Memory deallocate eden fonksiyon

a��klama: Hash tablodaki t�m de�i�kenler statik boyutta de�il dinamik olarak olu�turuldu�undan
	program ��k���nda ilgili memory deallocate edilmesi gerekti�inden s�ras� ile t�m allocasyonlar
	silindi
		
parametreler:
	hash_table = hash tablosu
	tmp1,tmp2,tmp3 = ge�ici de�i�kenler
*/
void deallocate(word* hash_table){
	int i;
	for(i=0;i<TABLE_SIZE;i++){
		word tmp1 = hash_table[i];
		free(tmp1.string);
		list* tmp2 = tmp1.documents;
		while(tmp2!=NULL){
			list* tmp3 = tmp2;
			tmp2 = tmp2->next;
			free(tmp3->value);
			free(tmp3);
		}
	}
	free(hash_table);
	printf("Program sonlandirildi.");
}





