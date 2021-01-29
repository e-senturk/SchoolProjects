#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/*Verilen text için Boyer-Moore skip tablosunu oluþturan fonksiyon:
	1. ASCII karakter dizisinin boyutu 256 byte olduðundan skipTable dizisi oluþturuldu tüm deðerleri -1 e eþitlendi. 
	2. Text içerisindeki tüm karakterler sýrasý ile incelenerek textteki görüldüðü noktalar bu diziye kaydedildi.
	3. Kaydedilen dizide ayný karakterle her karþýlaþýldýðýnda karakterin son konumu dizide saklandý.
	4. Case sensitive seçilmesi halinde harfin konumu tablo üzerinde hem küçük harfin hem de büyük harfin deðerine atandý. 
Bu sayede tabloda aranan elemanýn küçük harf veya büyük harf olmasýný herhangi bir etki oluþturmamasý saðlandý.
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


/*Verilen iki karakterin ayný olup olmadýðýný case sensitive olarak veya olmayarak bulan fonksiyon.
	1. Eðer case sensitive seçilmemiþse iki karakter de lower case olarak çevrilip karþýlaþtýrýlýr.
	2- Eðer case sensitive seçilmiþse iki karakter deðiþtirilmeden karþýlaþtýrýlýr.
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
/*Boyer-Moore algoritmasýna göre string içinde substring arayarak bulduðu eþleþmelerin ilk elemanýný bir dizide saklayan fonksiyon:
		1. Aranýlacak kelime için createSkipTable fonksiyonu ile skip table oluþturuldu.
		2. Kelimenin text üzerinde eþleþtiði bölümlerdeki ilk harfi tutan instance dizisi oluþturuldu
		3. Aranan textin kaç kez tekrar edeceðini bilmediðimizden dizinin kapsitesi dizi her dolduðunda 2 katýna çýkartýlarak logaritmik olarak arttýrýldý.
		4. i deðiþkeni ile text üzerinde her seferinde skip deðeri kadar arama yaptýk. 
		5. skip deðerini son karakterden ne kadar geriye atayacaðýmýzý skip table kullanarak belirledik.
		6. skip deðerinin 0 dan küçük olduðu durumda 1 karakter geriye atladýk.
		7. altadýðýmýz yerden geriye doðru aradýðýmýz kelime boyunca eþleþme olduðunda j -1 deðerini alacaðýndan bu durumda ulaþtýðýmýz konumu instance dizisine kaydettik.
		8. iþlemi dizi bitinceye kadar tekrarladýk bu sayede text üzerindeki tüm eþleþmelerin ilk karakterinin konumu instances dizisinde kaydedilmiþ oldu.
		
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

/* Boyer moore ile bulduðumuz eþleþmelerin yerine replace textini yerleþtirmeye ve dizinin boyutunu düzenlemeye yarayan fonksiyon.
	1. Eðer deðiþtirilecek text aranan textten uzunsa sondan baþa, eðer kýsa ise baþtan sona yer deðiþtirme yapýldý.
	2. diff deðeri ile bu durum kaydedildi.
*/
char* changeFindings(char* text, char* replace, int *sizeText , int sizeWord, int* instances, int sizeInstances){
	int sizeReplace = strlen(replace), diff = sizeReplace - sizeWord;
	/*Yer deðiþtirilecek eleman önceki elemandan daha küçükse dizi baþtan sona doðru düzenlendi.
		1. k deðiþkeni instances dizisini sýra ile incelemek için kullanýldý.
		2. Yerleþecek kelime daha küçük olduðunda texti baþtan sona deðiþtiridiðimizde herhangi bir harfi override etmiyoruz.
		3. if kýsmýnda eðer instances üzerinde herhangi bir elemana ulaþýldýysa deðiþtirilecek text dizinin o konumuna baþtan sona kopyalandý. 
		4. else kýsmýnda t deðeri kullanýlarak fark kadar textin elemanlarýný geriye kaydýrmak için kullanýldý.
		5. t deðeri her bulunan elemana gelindiðine fark kadar arttýrýldý bu sayede dizi yeni boyutu için küçültülmüþ oldu.
		6. Boþta kalan elemanlarý bellekten boþaltmak için dizi realloc yapýldý.
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
	/*Yer deðiþtirilecek eleman önceki elemandan daha büyükse dizi sondan baþa doðru düzenlendi.
		1. dizi realloc ile "instances sayýsý*(yerleþecek-aranan)" kadar büyütüldü.
		2. k deðiþkeni instances dizisini sýra ile incelemek için kullanýldý.
		3. Yerleþecek kelime daha büyük olduðunda texti sondan baþa deðiþtiridiðimizde herhangi bir harfi override etmiyoruz.
		4. if kýsmýnda eðer instances üzerinde herhangi bir elemanýn son harfine ulaþýldýysa deðiþtirilecek text dizinin o konumuna sondan baþa kopyalandý. 
		5. else kýsmýnda t deðeri kullanýlarak fark kadar textin elemanlarýný ileriye kaydýrmak için kullanýldý.
		6. t deðeri sondan baþladýðýmýzdan "instances sayýsý*(yerleþecek-aranan)" deðerine atandý ve her eþleþmede aradaki fark kadar azaltýldý.
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
	1. Verilen isimdeki dosya okuma modunda açýldý.
	2. Dosya ismi bulunamamasý durumunda kullanýcýya hata mesajý verilidi.
	3. fseek fonksiyonu ile textin son elemanýna gidildi ve textin size'ý bir integer deðiþkene kaydedildi.
	4. text rewind fonksiyonu ile baþa alýndý.
	5. malloc ile text boyutundan 2 fazla boyutta bir dizi açýldý.
	6. text diziye kaydedildi.
	7. textin sonlandýðýný belirtmek için string sonu karakteri textin sonuna eklendi.
	8. text dosyasý kapatýldý.
*/	
char* readFile(char* fileName){
	FILE *file = fopen(fileName, "r");
	if(!file){
		printf("Error while reading file\n");
		return "";
	}
	//Dosyadaki metnin karakter boyutu hesaplanýp bir deðiþkende saklandý. Ardýndan dosyanýn baþýna dönüldü.
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

/*Verilen diziyi verilen dosya adýndaki dosyaya kaydeden fonksiyon.
	1. Verilen isimdeki dosya yazma modunda açýldý.
	2. Dosya oluþturulamamasý durumunda kullanýcýya hata verilidi.
	6. Dizinin verilen size kadar elemaný texte kaydedildi.
	8. text dosyasý kapatýldý.
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
	1. Kullanýcýdan dosya adý, aranýlacak text, deðiþtirilecek text ve case sensitive durumlarý input olarak alýndý.
	2. caseSensitive için alýnan inputa uygun bir boolean deðer oluþturuldu hatalý input için program sonlandýrýldý.
	3. clock fonksiyonu ile o anki saat deðeri kaydedildi. Input alýmýný iþlem süresine dahil etmemek için baþlangýç clock deðeri input iþlemlerinden sonra alýndý.
	4. text dizisine readFile ile dosyadan metin okunup kaydedildi. Text okunanmamasý veya boþ olmasý durumunda program sonlandýrýldý.
	5. input texti, aranacak text ve deðiþtirilecek text'in boyutlarý hesaplandý.
	6. boyerMoore fonksiyonu ile instances dizisi oluþturuldu.
	7. changeFindings fonksiyonu ile text düzenlemesi yapýldý.
	8. write file fonksiyonu ile text dosyaya yazýldý.
	9. clock fonksiyýnu tekrar kullanýlarak aradaki fark bir double üzerinde hesaplanýp saniye ve milisaniye cinsinden yazdýrýldý. 
*/
int main() {
	char fileName [100], word [100], replace [100],temp,test[100];
	bool caseSensitive;
	//Input Alma Bölümü
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
	//Clock Baþlangýcý
	clock_t begin = clock();
	//Dosya okuma	
	char* text =readFile(fileName);
	if(!strlen(text)) return 0;
	int i,sizeText = strlen(text), sizeWord = strlen(word), * instances,sizeInstances;
	//Boyer-Moore listesi oluþturma
	instances = boyerMoore(text, sizeText, word, sizeWord, caseSensitive, &sizeInstances);	  
	//Oluþturulan listeye göre texti þekillendirme 
	text = changeFindings(text, replace, &sizeText, sizeWord, instances, sizeInstances);
	//Dosyaya tekrar yazma
	writeFile(fileName,text,sizeText);
	//Clock Sonu
	clock_t end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	//Çýktý Yazdýrma Bölümü
	printf("Found and Replaced: %d\n",sizeInstances);
	printf("Running Time: %lg miliseconds or %lg seconds.",timeSpent*1000,timeSpent);
	return 0;
}
