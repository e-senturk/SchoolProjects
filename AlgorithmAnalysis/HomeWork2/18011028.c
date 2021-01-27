#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Tablo boyutu 1000'e en yakýn asal sayý olarak seçildi
#define TABLE_SIZE 997

// Horner deðeri hesaplanýrken üssü alýnacak sayý
#define R 31

// Struct Yapýlarý
/* 
Liste yapýsý document nesnelerini saklamak için oluþturulan struct yapýsý

parametreler:	
	value = Listenin o elemanýnýn içinde saklanacak string deðerini tutar
	next = Sonraki elemanýn pointer deðerini tutar
*/
struct list{
	char* value;
	struct list* next;
};
typedef struct list list;

/* 
Word yapýsý herhangi bir kelimeyi ve kelimenin geçtiði dökümanlarý saklamak için oluþturulan struct yapýsý

parametreler:
	string = Kelimenin içeriðini saklamak için oluþturulan yapý
	documents = Kelimenin geçtiði dökümanlarýn listesini saklayan linkli liste
*/
struct {
	char* string;
	list* documents;
}typedef word;

// Fonksiyon Headerlarý

// String Fonksiyonlarý 
int order(char character);
char lowercase(char character);
bool compare_char(char character1,char character2);
bool compare_string(char* string1, char* string2);

// Liste Fonksiyonlarý
list* create_list(char* value);
bool find_list(list* head, char* value);
list* add_front_list(list* head, char* value);

// Kelime Fonksiyonlarý
word create_word(char* string,char* document);
word add_document(word *element,char* document);
bool is_in_document(word element,char*document);
void print_word(word element);

// Hash Fonksiyonlarý
word* create_new_hash();
int horner(char* text);
int double_hashing(char* string,int i);
int search_hash_table(char* string, word* hash_table,int* step);
void insert_hash_table(char* string, char* document, word* hash_table,int* load_factor);
void find_word(char* string,word* hash_table);

// Input/Output Fonksiyonlarý
void print_info();
bool write_database(word* hash_table,int load_factor);
word* read_database(int* load_factor);
bool read_document(char* file_name, word* hash_table,int *load_factor);
void deallocate();

// Ana Fonksiyon
/* 
Kullanýcýdan alýnan inputlara göre dosyalarý hash tablosuna ekleyip dosyalar üzerinde arama yapan fonksiyon

parametreler:
	hash_table = hash tablosunu tutan dizi
	load_factor = tablodaki dolu eleman sayýsýný tutan deðiþken
	select = kullanýcýdan input alýrken kullanýlan karakter
	file_name = okunacak dosya adýný tutan string
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

// Yardýmcý fonksiyonlar

/* 
Verilen karakterin alfabetik olarak sýrasýný 0 dan baþlayacak þekilde hesaplayan fonksiyon
Eðer karakter alfabede yoksa ascii tablosundaki deðeri kullanýldý

parametreler:
	c = verilen karakter
	return = karakterin alfabetik sýrasýnýn integer deðeri
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
Case sensitive kuralý gözetmeden 2 karakteri karþýlaþtýran fonksiyon

parametreler:
	c1 = 1. karakter
	c2 = 2. karakter
	return = karakterlerin eþit olup olmadýðýný döndüren boolean deðeri
*/
bool compare_char(char c1,char c2){
	return lowercase(c1) == lowercase(c2);
}

/* 
Case sensitive kuralý gözetmeden 2 stringi karþýlaþtýran fonksiyon

parametreler:
	s1 = 1. string
	s2 = 2. string
	i = iterasyon için kullanýldý
	return = stringlerin eþit olup olmadýðýný döndüren boolean deðeri
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
Listenin head nodunu verilen deðere göre oluþturan fonksiyon

açýklama: Head noduna memory allocation ile yer açýldý
	Value için memory allocation ile eklenen kelime kadar yer açýldý
	Head deðerinin içine deðer olarak value deðeri kaydedildi
	Head nodunun next nodu olarak NULL atandý
	Sonuç olarak oluþturulan head deðerine geri dönüþ yapýldý

parametreler:
	value = listenin head noduna eklenecek kelime
	return = oluþturlan yeni listenin head nodu
*/
list* create_list(char* value){
	list* head = (list*) malloc(sizeof(list));
	head->value = (char*) malloc(sizeof(char)*strlen(value));
	strcpy(head->value,value);
	head->next = NULL;
	return head;
}

/* 
Verilen liste içinde kelime arayan fonksiyon

açýklama: Verilen liste içerisindeki tüm nodlar son noda kadar itere edildi
	Eðer son noda ulaþýlmadan kelime bulunursa true, son noda ulaþýlýrsa 
	false döndürüldü

parametreler:
	head = listenin head nodu
	value = aranacak kelime
	return = kelimenin bulup bulunmadýðýnýn bilgisini içeren boolean deðeri
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
listenin baþýna eleman ekleyen fonksiyon

açýklama: Listenin içinde find_list fonksiyonu kullanýlarak döküman adý arandý
	Eðer döküman yoksa yeni bir head nodu oluþturuldu ve listenin baþýna eklendi
	Eðer ilk head node boþsa yeni oluþturlan node head olarak atandý eski head node silindi
	Eðer ilk head node doluysa yeni head nodun sonrasýna eski head nodu atandý
	Yeni head nodu döndürüldü

parametreler:
	head = listenin head nodu
	new_head = listenin yeni head_nodu;
	value = aranacak kelime
	return = kelimenin bulup bulunmadýðýnýn bilgisini içeren boolean deðeri
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
Verilen kelime ve döküman için yeni kelime oluþturan fonksiyon

açýklama: Word tipinde bir struct oluþturuldu
	Memory allocation ile bu structa yer açýldý
	Yeni kelime word struct yapýsýna kopyalandý
	Word yapýsýndaki listeye döküman yeni liste oluþtularak eklendi
	Sonuç olarak oluþturulan struct yapýsýna dönüldü

parametreler:
	string = eklenecek kelime
	document = kelimenin bulunduðu dökümanýn adý
	new_word = kelime ve dökümanýn içeriðinin bulunduðu struct yapýsý
	return = yeni oluþturulan struct yapýsý
*/
word create_word(char* string,char* document){
	word new_word;
	new_word.string = (char*) malloc(sizeof(char)*strlen(string));
	strcpy(new_word.string,string);
	new_word.documents = create_list(document);
	return new_word;
}

/* 
Verilen kelime ve döküman için yeni kelime oluþturan fonksiyon

açýklama: Word tipinde bir struct oluþturuldu
	Memory allocation ile bu structa yer açýldý
	Yeni kelime word struct yapýsýna kopyalandý
	Word yapýsýndaki listeye döküman yeni liste oluþtularak eklendi
	Sonuç olarak oluþturulan struct yapýsýna dönüldü

parametreler:
	string = eklenecek kelime
	document = kelimenin bulunduðu dökümanýn adý
	new_word = kelime ve dökümanýn içeriðinin bulunduðu struct yapýsý
	return = yeni oluþturulan struct yapýsý
*/
word add_document(word *element,char* document){
	element->documents = add_front_list(element->documents,document);
	return *element;
}

/* 
Kelimenin içinde herhangi bir dökümanýn olup olmadýðýný arayan yardýmcý fonksiyon

açýklama: Verilen bir word structý içindeki döküman listesinde verilen dökümanýn
	olup olmaduðýný find_list fonksiyonunu kullanarak arandý

parametreler:
	word = arama yapýlacak kelime structu
	document = aranan döküman
	return = dökümanýn o word structunda olup olmadýðýný gösteren boolean deðiþkeni
*/
bool is_in_document(word element,char*document){
	return find_list(element.documents,document);
}

/* 
Tablo boyutuna göre hash tablosunu boþ elemanlarla oluþturan fonksiyon

açýklama: Define ile belirlenen tablo boyutunda word structundan oluþan bir dizi memory allocation ile oluþturuldu
	Her eleman tek tek memory allocation ile oluþturulmasý gerektiðinden create_word fonksiyonu çaðýrýldý
	Çaðýrýlan fonksiyona kelime "*" olarak döküman adý ise boþ olarak atandý
	write_database fonksiyonu ile database dosyasý güncellendi
	Dönüþ deðeri olarak oluþturulan tabloya dönüldü  

parametreler:
	hash_table = yeni oluþturulan hash tablosu
	i = iterasyon için kullanýldý
	return = yeni oluþturulan hash tablosu
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
Verilen bir texti horner metodu ile integera çeviren fonksiyon

açýklama: Verilen bir text bir for loopu ile baþtan sona doðru horner metodu
	kullanýlarak elde edilen toplam her adýmda R ile çarpýlarak karakterin farklý katsayýda toplama etki etmesi
	saðlandý bulunan toplam negatif çýkarsa (toplamýn integer boyutunu geçme ihtimali yüzünden) sonuç -1 ile çarpýldý.

parametreler:
	text = verilen text deðeri 
	i = iterasyon için kullanýldý
	sum = horner için hesaplanan toplamý tutan deðiþken
	size = verilen deðiþkenin
	return = bulunan toplam sonucunun pozitif deðeri bulunan integer deðeri 
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
Verilen bir stringi verilen bir integer deðeri ile birlikte double hashing iþleminden geçiren fonksiyon

açýklama: String horner metoduna göre bir integer sayýya dönüþtürüldü
	Tablo boyutuna göre h1 ve h2 fonksiyonlarý double hashing için oluþturuldu
	h2 deðeri i deðeri ile çarpýlýp h1 deðeri ile toplandý ve bulunan sonucun tablo boyutuna göre modu alýndý
	Kullanýcýya bu sonuç deðeri döndürüldü
	
parametreler:
	string = verilen string deðeri 
	i = h2 deðeri katsayýsý
	horn = horner metoduyla hesaplanan integer sayý
	h1 = h1 fonksiyonunun sonucunu tutan deðiþken
	h2 = h2 fonksiyonunun sonucunu tutan deðiþken
	hash = hesaplanan hash deðerini tutan deðiþken 
	return = hesaplanan hash deðeri
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
Hash tablosu içinde verilen bir stringin geçip geçmediðini bulan geçiyorsa o indisi geçmiyorsa 
bulunan ilk boþ indisi döndüren fonksiyon

açýklama: Aranýlan stringin double_hashing fonksiyonu ile hash deðeri i=0 için hesaplandý
	i deðiþkeni step deðiþkeninde daha sonra kaç adýmda iþlem yapýldýðýný bulmak için saklandý
	Eðer bulduðumuz hash deðerindeki string aradýðýmýz stringle ayný ise o index deðeri döndürüldü
	Eðer eþit deðilse boþ hash elemaný buluncaya kadar i deðeri arttýrýlarak sonraki hash deðerlerine bakýldý
	Eðer eleman herhangi bir hash deðerinde bulunursa o indexe döndürüldü
	Eðer boþ eleman bulunursa da yine o indexe döndürülüdü bu sayede sonraki elemaný yerleþtirebileceðimiz indexi de bulmuþ olduk
		
parametreler:
	string = verilen string deðeri 
	hash_table = hash tablosu
	step = iþlem adým sayýsý
	index = hesaplanan hash deðerini tutan deðiþken
	i = iterasyon için kullanýldý
	return = hesaplanan index deðeri
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
Hash tablosuna verilen stringi yerleþtiren fonksiyon

açýklama: Verilen kelimenin bulunduðu veya koyulmasý gereken hash deðeri search_hash_table fonksiyonu ile hesaplandý
	tablo dolu ise eleman eklenemedi ve eklenemeyen kelime kullanýcýya yazdýrýldý
	tablo %80 dolu ise kullanýcýya tablonun dolduðuna dair uyarý verildi
	eðer bulunan index boþ ise yeni kelime ve döküman adý o indexe yerleþtirildi
	her yeni kelime eklendiðinde doluluk oraný kullanýcýya yazdýrýldý
	eðer index dolu ise sadece döküman adý o indexe eklendi
	
parametreler:
	string = verilen string deðeri
	document = döküman adý 
	hash_table = hash tablosu
	load_factor = tablodaki dolu eleman sayýsý
	step = iþlem adým sayýsý
	index = hesaplanan hash deðerini tutan deðiþken
	ratio = tablonun doluluk oranýný saklayan deðiþken
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
Aranan kelimeyi hash tablosunda bulup hangi dosyalarda olduðunu yazdýran fonksiyon

açýklama: Verilen kelimenin hash deðeri search_hash_table fonksiyonu ile hesaplandý
	Step deðeri ile bu iþlemin kaç adýmda hesaplandýðý yazdýrýldý
	Eðer döndürülen indexteki kelime hatalýysa kelime bulunamadýðýna dair kullanýcýya çýktý verildi
	Eðer indexde eleman varsa kelimenin bulunduðuna dair kullanýcýya çýktý verildi
	Ýþlem adým sayýsý kullanýcýya verildi
		
parametreler:
	string = verilen string deðeri
	hash_table = hash tablosu
	step = iþlem adým sayýsýný tutan deðiþken
	index = hesaplanan hash deðerini tutan deðiþken
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
Kullanýcýya input hakkýnda bilgi yazdýran fonksiyon
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
Veritabanýndan hash tablosunu ve load faktörü yazan fonksiyon

açýklama: load_factor yazdýrýlmak üzere string deðerine çevrildi
	Database dosyasýna veritabaný dosyasý yazma modunda açýldý açýlamazsa false döndürüldü
	Dosya düzeni açýsýndan verilerin neye ait olduðu da dosyaya yazdýrýldý
	Ýlk baþta load faktör sonrasýnda ise hash tablosundaki elemanlar sýrasý ile dosyaya yazdýrýldý
	Hash tablosunda her satýrý ayýrt etmek için satýr baþýna # deðeri konuldu
	Satýrýn ilk elemaný olarak kelime deðeri sonraki elemanlarý olarak dosya adlarý sýrasý ile yazdýrýldý
	Dosya yazma sonrasýnda kapatýldý
		
parametreler:
	hash_table = yazdýrýlacak hash tablosu
	load_factor = tablodaki dolu eleman sayýsý
	i = iterasyon için kullanýldý
	database = yazdýrýlacak veritabaný dosyasýný saklayan pointer
	load_str = load faktörün string deðeri
	return = yazým iþleminin baþarýlý veya baþarýsýz olduðunu ifade eden boolean deðeri
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
Veritabanýndan hash tablosunu ve load faktörü okuyan fonksiyon

açýklama: Hash tablosu için memory alaný oluþturuldu
	Veritabaný dosyasý okundu eðer bulunamazsa create_new_hash fonksiyonu ile boþ veri tabaný dosyasý oluþturuldu
	Veri açýsýndan bilgilendirici satýrlar görmezden gelindi
	Load factor string olarak okunup atoi fonksiyonu ile integera çevrildi
	Sýrasý ile her satýr baþýndaki # deðerine göre ayrýþtýrýldý
	Satýrýn ilk elemaný kelime olarak diðer elemanlarý ise döküman olarak okundu ve ilgili fonksiyonlarla
	word structu oluþturuldu
	Oluþturulan structlar hash tablosuna sýrasý ile eklendi
	Dosya okuma sonrasýnda kapatýldý

		
parametreler:
	load_factor = tablodaki dolu eleman sayýsý
	i = iterasyon için kullanýldý
	hash_table = oluþtutulacak hash tablosu
	database = okunacak veritabaný dosyasýný saklayan pointer
	load_str = load faktörün string deðeri
	temp,word = geçici olarak input deðerlerini saklayan bufferlar
	string = okunan kelime
	return = okuma sonucu oluþan hash tablosu
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
Herhangi bir dosyayý okuyup içindeki kelimeleri hash tablosuna kaydeden fonksiyon

açýklama: Verilen kelimenin hash deðeri search_hash_table fonksiyonu ile hesaplandý
	Step deðeri ile bu iþlemin kaç adýmda hesaplandýðý yazdýrýldý
	Eðer boþ index bulduysak kelime bulunamadýðýna dair kullanýcýya çýktý verildi
	Eðer indexde eleman varsa kelimenin bulunduðuna dair kullanýcýya çýktý verildi
	Ýþlem adým sayýsý kullanýcýya verildi
		
parametreler:
	file_name = okunacak dosyanýn adý
	hash_table = oluþtutulacak hash tablosu
	load_factor = tablonun doluluk sayýsý
	file = okunacak dosyayý saklayan pointer
	string = okunan kelime
	return = dosya okuma iþleminin baþarýlý veya baþarýsýz olduðunu gösteren boolean deðer
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

açýklama: Hash tablodaki tüm deðiþkenler statik boyutta deðil dinamik olarak oluþturulduðundan
	program çýkýþýnda ilgili memory deallocate edilmesi gerektiðinden sýrasý ile tüm allocasyonlar
	silindi
		
parametreler:
	hash_table = hash tablosu
	tmp1,tmp2,tmp3 = geçici deðiþkenler
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





