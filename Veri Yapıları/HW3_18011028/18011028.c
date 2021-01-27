#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/*Verilen text i�in Boyer-Moore skip tablosunu olu�turan fonksiyon:
	1. ASCII karakter dizisinin boyutu 256 byte oldu�undan skipTable dizisi olu�turuldu t�m de�erleri -1 e e�itlendi. 
	2. Text i�erisindeki t�m karakterler s�ras� ile incelenerek textteki g�r�ld��� noktalar bu diziye kaydedildi.
	3. Kaydedilen dizide ayn� karakterle her kar��la��ld���nda karakterin son konumu dizide sakland�.
	4. Case sensitive se�ilmesi halinde harfin konumu tablo �zerinde hem k���k harfin hem de b�y�k harfin de�erine atand�. 
Bu sayede tabloda aranan eleman�n k���k harf veya b�y�k harf olmas�n� herhangi bir etki olu�turmamas� sa�land�.
*/
void createSkipTable(const char* text,int skipTable[256],bool caseSensitive){
	int i;
	for(i=0; i< 256; i++){
		skipTable[i] = -1;
	}
	i=0;
	while(text[i]!='\0'){
		skipTable[text[i]] = i;
		if(!caseSensitive){
			if(text[i]>='a' && text[i]<='z'){
				skipTable[text[i]+'A'-'a'] = i;
			}
			if(text[i]>='A' && text[i]<='Z'){
				skipTable[text[i]+'a'-'A'] = i;
			}
		}
		i++;
	}
}


/*Verilen iki karakterin ayn� olup olmad���n� case sensitive olarak veya olmayarak bulan fonksiyon.
	1. E�er case sensitive se�ilmemi�se iki karakter de lower case olarak �evrilip kar��la�t�r�l�r.
	2- E�er case sensitive se�ilmi�se iki karakter de�i�tirilmeden kar��la�t�r�l�r.
*/
bool isEqual(char x, char y, bool caseSensitive){
	if(!caseSensitive){
		if(x>='A' && x<='Z'){
			x= x+'a'-'A';
		}
		if(y>='A' && y<='Z'){
			y= y+'a'-'A';
		}
	}
	return x==y;		
}
/*Boyer-Moore algoritmas�na g�re string i�inde substring arayarak buldu�u e�le�melerin ilk eleman�n� bir dizide saklayan fonksiyon:
		1. Aran�lacak kelime i�in createSkipTable fonksiyonu ile skip table olu�turuldu.
		2. Kelimenin text �zerinde e�le�ti�i b�l�mlerdeki ilk harfi tutan instance dizisi olu�turuldu
		3. Aranan textin ka� kez tekrar edece�ini bilmedi�imizden dizinin kapsitesi dizi her doldu�unda 2 kat�na ��kart�larak logaritmik olarak artt�r�ld�.
		4. i de�i�keni ile text �zerinde her seferinde skip de�eri kadar arama yapt�k. 
		5. skip de�erini son karakterden ne kadar geriye atayaca��m�z� skip table kullanarak belirledik.
		6. skip de�erinin 0 dan k���k oldu�u durumda 1 karakter geriye atlad�k.
		7. altad���m�z yerden geriye do�ru arad���m�z kelime boyunca e�le�me oldu�unda j -1 de�erini alaca��ndan bu durumda ula�t���m�z konumu instance dizisine kaydettik.
		8. i�lemi dizi bitinceye kadar tekrarlad�k bu sayede text �zerindeki t�m e�le�melerin ilk karakterinin konumu instances dizisinde kaydedilmi� oldu.
		
*/
int* boyerMoore(const char* text,int sizeText, char* word, int sizeWord, bool caseSensitive, int* sizeInstances){
	int i, j, k = 0, skip = 0, size = 2, skipTable [256];
	createSkipTable(word, skipTable, caseSensitive);	
	int* instances = (int*)malloc(sizeof(int)*2);
	for(i = 0; i <= sizeText-sizeWord ; i += skip){
		j = sizeWord-1;
		while(j>=0 && isEqual(word[j], text[i+j], caseSensitive))
			j--;
		if(j >= 0){
			skip = j - skipTable[text[i+j]];
			if(skip < 0){
				skip = 1;
			}	
		}
		else{
			skip = sizeWord;
			if(k>=size){
				size*=2;
				instances=(int*)realloc(instances,size*sizeof(int));
			}
			instances[k] = i;
			k++;
		}
	}
	*sizeInstances = k;
	return instances;
}

/* Boyer moore ile buldu�umuz e�le�melerin yerine replace textini yerle�tirmeye ve dizinin boyutunu d�zenlemeye yarayan fonksiyon.
	1. E�er de�i�tirilecek text aranan textten uzunsa sondan ba�a, e�er k�sa ise ba�tan sona yer de�i�tirme yap�ld�.
	2. diff de�eri ile bu durum kaydedildi.
*/
char* changeFindings(char* text, char* replace, int *sizeText , int sizeWord, int* instances, int sizeInstances){
	int sizeReplace = strlen(replace), diff = sizeReplace - sizeWord;
	/*Yer de�i�tirilecek eleman �nceki elemandan daha k���kse dizi ba�tan sona do�ru d�zenlendi.
		1. k de�i�keni instances dizisini s�ra ile incelemek i�in kullan�ld�.
		2. Yerle�ecek kelime daha k���k oldu�unda texti ba�tan sona de�i�tiridi�imizde herhangi bir harfi override etmiyoruz.
		3. if k�sm�nda e�er instances �zerinde herhangi bir elemana ula��ld�ysa de�i�tirilecek text dizinin o konumuna ba�tan sona kopyaland�. 
		4. else k�sm�nda t de�eri kullan�larak fark kadar textin elemanlar�n� geriye kayd�rmak i�in kullan�ld�.
		5. t de�eri her bulunan elemana gelindi�ine fark kadar artt�r�ld� bu sayede dizi yeni boyutu i�in k���lt�lm�� oldu.
		6. Bo�ta kalan elemanlar� bellekten bo�altmak i�in dizi realloc yap�ld�.
	*/	
	if(diff <= 0){
		int i, k=0, t = 0;
		for(i = 0; i+t<*sizeText; i++){
			if(i == instances[k]-t && k < sizeInstances){
				int j;
				for(j = 0; j<sizeReplace; j++){
					text[i+j]=replace[j];
				}
				k++;
				t-=diff;
				i=i+sizeReplace-1;
			}
			else{
				text[i]=text[i+t];
			}
		}
		*sizeText=*sizeText-t-2;
		text = realloc(text,(*sizeText)*sizeof(char));
	}
	/*Yer de�i�tirilecek eleman �nceki elemandan daha b�y�kse dizi sondan ba�a do�ru d�zenlendi.
		1. dizi realloc ile "instances say�s�*(yerle�ecek-aranan)" kadar b�y�t�ld�.
		2. k de�i�keni instances dizisini s�ra ile incelemek i�in kullan�ld�.
		3. Yerle�ecek kelime daha b�y�k oldu�unda texti sondan ba�a de�i�tiridi�imizde herhangi bir harfi override etmiyoruz.
		4. if k�sm�nda e�er instances �zerinde herhangi bir eleman�n son harfine ula��ld�ysa de�i�tirilecek text dizinin o konumuna sondan ba�a kopyaland�. 
		5. else k�sm�nda t de�eri kullan�larak fark kadar textin elemanlar�n� ileriye kayd�rmak i�in kullan�ld�.
		6. t de�eri sondan ba�lad���m�zdan "instances say�s�*(yerle�ecek-aranan)" de�erine atand� ve her e�le�mede aradaki fark kadar azalt�ld�.
	*/	
	else{
		int i, k=sizeInstances-1, t = diff*(sizeInstances);
		text = (char*)realloc(text,sizeof(char)*(*sizeText+t));
		for(i = *sizeText-1; i>=0; i--){
			if(i == instances[k]+sizeWord-1 && k >= 0){
				int j;
				for(j=0; j<sizeReplace;j++){
					text[i+t-j] = replace[sizeReplace-j-1];					
				}
				t-=diff;
				k--;
				i-=(sizeWord-1);
			}
			else{
				text[i+t]=text[i];
			}
		}
		*sizeText=*sizeText+diff*(sizeInstances)-2;
	}
	return text;
}
	
/*Dosyadan metni okuyup diziye kaydeden fonksiyon.
	1. Verilen isimdeki dosya okuma modunda a��ld�.
	2. Dosya ismi bulunamamas� durumunda kullan�c�ya hata mesaj� verilidi.
	3. fseek fonksiyonu ile textin son eleman�na gidildi ve textin size'� bir integer de�i�kene kaydedildi.
	4. text rewind fonksiyonu ile ba�a al�nd�.
	5. malloc ile text boyutundan 2 fazla boyutta bir dizi a��ld�.
	6. text diziye kaydedildi.
	7. textin sonland���n� belirtmek i�in string sonu karakteri textin sonuna eklendi.
	8. text dosyas� kapat�ld�.
*/	
char* readFile(char* fileName){
	FILE *file = fopen(fileName, "r");
	if(!file){
		printf("Error while reading file\n");
		return "";
	}
	//Dosyadaki metnin karakter boyutu hesaplan�p bir de�i�kende sakland�. Ard�ndan dosyan�n ba��na d�n�ld�.
	fseek(file, 0L, SEEK_END);
	int sizeFile = ftell(file);
	rewind(file);
	char* text =malloc(sizeof(char)*(sizeFile+2));
	int i=0;
	while(!feof(file)){
		text[i]=getc(file);
		i++;
	}
	text[i]='\0';
	fclose(file);
	return text;
}

/*Verilen diziyi verilen dosya ad�ndaki dosyaya kaydeden fonksiyon.
	1. Verilen isimdeki dosya yazma modunda a��ld�.
	2. Dosya olu�turulamamas� durumunda kullan�c�ya hata verilidi.
	6. Dizinin verilen size kadar eleman� texte kaydedildi.
	8. text dosyas� kapat�ld�.
*/	
void writeFile(char* fileName, char* text, int sizeText){
	FILE* file = fopen(fileName, "w");
	if(!file){
		printf("Error while writing file\n");
		return;
	}
	int i;
	for(i=0;i<=sizeText;i++){
		fputc(text[i],file);
	}
	fclose(file);
}

/*Ana fonksiyon:
	1. Kullan�c�dan dosya ad�, aran�lacak text, de�i�tirilecek text ve case sensitive durumlar� input olarak al�nd�.
	2. caseSensitive i�in al�nan inputa uygun bir boolean de�er olu�turuldu hatal� input i�in program sonland�r�ld�.
	3. clock fonksiyonu ile o anki saat de�eri kaydedildi. Input al�m�n� i�lem s�resine dahil etmemek i�in ba�lang�� clock de�eri input i�lemlerinden sonra al�nd�.
	4. text dizisine readFile ile dosyadan metin okunup kaydedildi. Text okunanmamas� veya bo� olmas� durumunda program sonland�r�ld�.
	5. input texti, aranacak text ve de�i�tirilecek text'in boyutlar� hesapland�.
	6. boyerMoore fonksiyonu ile instances dizisi olu�turuldu.
	7. changeFindings fonksiyonu ile text d�zenlemesi yap�ld�.
	8. write file fonksiyonu ile text dosyaya yaz�ld�.
	9. clock fonksiy�nu tekrar kullan�larak aradaki fark bir double �zerinde hesaplan�p saniye ve milisaniye cinsinden yazd�r�ld�. 
*/
int main() {
	char fileName [100], word [100], replace [100],temp,test[100];
	bool caseSensitive;
	//Input Alma B�l�m�
	printf("Please enter file name(Ex: input.txt): ");
	gets(fileName);
	printf("Please enter a text to find: ");
	gets(word);
	printf("Please enter a text to replace: ");
	gets(replace);
	printf("Check case sensitivity(Y/N): ");
	scanf(" %c", &temp);
	if(temp == 'y' || temp == 'Y'){
		caseSensitive = true;
	}
	else if(temp == 'n' || temp == 'N')	{
		caseSensitive = false;
	}
	else{
		printf("Invalid Input");
		return 0;
	}
	//Clock Ba�lang�c�
	clock_t begin = clock();
	//Dosya okuma	
	char* text =readFile(fileName);
	if(!strlen(text)) return 0;
	int i,sizeText = strlen(text), sizeWord = strlen(word), * instances,sizeInstances;
	//Boyer-Moore listesi olu�turma
	instances = boyerMoore(text, sizeText, word, sizeWord, caseSensitive, &sizeInstances);	  
	//Olu�turulan listeye g�re texti �ekillendirme 
	text = changeFindings(text, replace, &sizeText, sizeWord, instances, sizeInstances);
	//Dosyaya tekrar yazma
	writeFile(fileName,text,sizeText);
	//Clock Sonu
	clock_t end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	//��kt� Yazd�rma B�l�m�
	printf("Found and Replaced: %d\n",sizeInstances);
	printf("Running Time: %lg miliseconds or %lg seconds.",timeSpent*1000,timeSpent);
	return 0;
}
