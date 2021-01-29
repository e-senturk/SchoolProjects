#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFSIZE 4096  //al�nan bir sat�r i�in kullan�lcak buffer boyutunu g�sterir.
	

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//Dosyadan okunan akt�rleri tutan struct yap�s�
typedef struct Node{
	char actorName [100]; 	//Akt�r ad�n� i�erir
	struct Node* next; 		//Akt�rden sonraki nodu i�erir.
}Actor;

//Filmleri akt�rleriye tutan struct yap�s�
typedef struct Node2{
	char movieName[100]; 	//Film ad�n� i�erir.
	bool kevinBacon;  		//Filmin kevinBacon listesine eklenip eklenmedi�ini saklar.
	Actor* actorList; 		//Filmde oynayan akt�rlerin listesi
	struct Node2* next;
}Movie;

//KevinBacon listesini tutan struct yap�s� //Actor listesi de kullan�labilirdi ancak ek veri yap�s� zaman ve bellek optimizasyonu i�in olu�turuldu.
typedef struct Node3{
	char actorName [100];	//Akt�r ad�n� i�erir
	Movie* baconFilm;		//Film ad�n� i�erir. //Bu i�emde strcpy yerine nesne kulland�. 
	struct Node3* prev;		//Kevin bacon s�ralamas�nda kendinden �nce gelen akt�r� tutar. -> Bu de�i�ken kevin bacon listesinin eski elemanlar�n� listeyi taramadan bulmaya yar�yor.
	struct Node3* next;		//Liste s�ras�nda sonraki akt�r� tutar.->Bu de�i�ken iterasyonu sa�lamak i�in kullan�l�yor.
}ActorInfo;


//Actor nesnesi olu�turan fonksiyon
	//De�i�ken olarak akt�r�n ad�n� al�p sonraki nodu NULL olacak �ekilde ayarlar
Actor* createNewActor(char* actorName){
	Actor* newElement = (Actor*) malloc(sizeof(Actor));
	strcpy(newElement -> actorName, actorName);
	newElement -> next = NULL;
	return newElement;
}

//Actor listesine eleman ekleyen fonksiyon
	//De�i�ken olarak act�r�n ad�n� ve akt�r listesinin head nodunu al�r.
	//Yeni bir nod olu�turup linkli listenin en ba��na ba�layarak linkli listenin her yeni eklenen eleman�n�n en ba�a eklenmesini sa�lar.
	//Liste bo�sa yeni eleman olu�turur.
Actor* addActorList(Actor* head, char* name){
	Actor* newActor = createNewActor(name);
	if(head == NULL){
		newActor->next = NULL;
	}
	else{		
		newActor->next = head;
	}
	return newActor;
}

//AktorInfo listesi olu�turan fonksiyon
	//Actor listesine benzer bir yap�dad�r.
	//Film ad� strcpy daha yava� oldu�undan filmin pointer� olacak �ekilde tutuldu.
	//prev nod olarak bu nodda kendisini kevinbacon listesine ba�layan eleman tutuldu.
ActorInfo* createNewActorInfo(char* actorName, Movie* baconFilm, ActorInfo* prev) {
	ActorInfo* newElement = (ActorInfo*) malloc(sizeof(ActorInfo));
	strcpy(newElement -> actorName, actorName);
	newElement->baconFilm = baconFilm;
	newElement->prev=prev;
	newElement -> next = NULL;
	return newElement;
}

//ActorInfo listesine eleman ekleyen fonksiyon
	//De�i�ken olarak act�r�n ad�n� ve akt�r listesinin head nodunu al�r.
	//Yeni bir nod olu�turup linkli listenin en ba��na ba�layarak linkli listenin her yeni eklenen eleman�n�n en ba�a eklenmesini sa�lar.
	//Liste bo�sa yeni eleman olu�turur.

ActorInfo* addActorInfoList(ActorInfo* head, char* actorName, Movie* baconFilm, ActorInfo* prev){
	ActorInfo* newActor = createNewActorInfo(actorName, baconFilm, prev);
	if(head == NULL){
		newActor->next = NULL;
	}
	else{		
		newActor->next = head;
	}
	return newActor;
}

//Verilen line �zerinden film ad�n� ve akt�rlerini �ekip yeni film olu�turan fonksiyon.

Movie* createNewMovie(char* line){
	Movie* newMovie = (Movie*) malloc (sizeof(Movie));
	newMovie -> kevinBacon = false;                        //Yeni film kevinbacon listesinde hen�z olmad���ndan de�eri false olarak atand�. 
	int i = 0, j;
	char* temp =(char*)malloc(sizeof(char)*100);           //Ge�ici de�i�kenleri tutmak i�in 100 charl�k yer ayr�ld�.
	while(line[i]!= '\0' &&line[i] != '/'){				   //Filmin ad� ilk while d�ng�s� ile al�nd�. "/" karakterini g�r�nceye kadar elemanlar filme ad�na eklendi.
		temp[i] = line [i]; 
		i++;
	}
	temp[i] = '\0';										   //Film ad�n�n son karakteri string sonu karakterine atand�.
	i++;											       // '/' i�areti atland�.
	strcpy(newMovie -> movieName, temp);                   //Olu�turulan film ad� yeni nesneye girildi.
	//act�r
	newMovie->actorList = NULL;						       //Act�r listesi olu�turuldu.
	while(line[i]!= '\0'){
		j=0;
		while(line[i] !='/' && line[i+1]!= '\0'){		   //Film ad� hesaplamaya benzer bir bi�imde her '/' karakteri �ncesinde kalan substring tempde sakland�.
			temp[j] = line[i];
			i++; j++;										//i iterat�r� line i�in j iterat�r� temp i�in kullan�ld�.
		}
		temp[j] = '\0';										//substringin sonuna string sonu karakteri eklendi.
		Actor*tempActor = createNewActor(temp);				//tempActor listesine her substing tan�ml� metoddar aracl��� ile olu�turulup eklendi.
		tempActor->next = newMovie->actorList;				
		newMovie->actorList =tempActor;
		i++;												// '/' karakteri atland�.
	}
	return newMovie;										//Olu�turulan yeni film nesnesi d�nd�r�ld�.
}

//Film listesine eleman ekleyen fonksiyon
	//De�i�ken olarak film line�n� ve film listesinin head nodunu al�r.
	//Yeni bir nod olu�turup linkli listenin en ba��na ba�layarak linkli listenin her yeni eklenen eleman�n�n en ba�a eklenmesini sa�lar.
	//Liste bo�sa yeni eleman olu�turur.
Movie* addMovieList(Movie* head, char* line){             	
	Movie* newMovie = createNewMovie(line);		
	if(head == NULL){
		newMovie->next = NULL;
	}
	else{
		newMovie->next = head;
	}
	return newMovie;
}

//Verilen bir file pointerdan sat�r sat�r string okuyan ve film listesi olu�turan fonksiyon.
Movie* createMovieList(FILE* input, char* readMode){
	Movie* movieList = NULL;						//Filmler listesi
	char line[BUFFSIZE];                			//BUFFSIZE define b�l�m�nden 4KB olacak �ekilde ayarland�.
	while (fgets(line, sizeof(line),input)) {		//Dosyadan her seferde 1 sat�r okumak i�in kullan�lan kod.
       	movieList=addMovieList (movieList ,line);	//Film listesine film eklemek i�in addMovieList fonksiyonu kullan�ld�.
    }
	return movieList;
}




//KevinBacon Act�r listesini itere ederek boyutunu d�nd�ren fonksiyon.
int sizeActorInfoList(ActorInfo* head){
	ActorInfo* iterator = head;
	int i =0;
	while(iterator != NULL){
		iterator = iterator -> next;
		i++;
	}
	return i;
}



//Verilen textin head nodu verilen kevinBacon listesinde olup olmad���n� listenin t�m elemanlar�n� tarayarak bulan fonksiyon.
bool inList(ActorInfo* head, char* name){
	ActorInfo* iterator = head;
	while(iterator != NULL && strcmp(iterator->actorName,name) != 0){
		iterator=iterator->next;
	}
	if(iterator == NULL){
		return false;
	}
	else{
		return true;
	}
}




ActorInfo* findNextBacon(Movie* list, ActorInfo* prevBacon){
	ActorInfo* nextBacon =prevBacon;       //�nceki kevinBacon listesi yeniListeye atand�.
	bool stop;							   //Looptan ��kmak i�in kullan�lan de�i�ken.
	Movie* i = list;					   //Film listesi i iterat�r�ne al�nd�.
	while(i != NULL){
		//Sadece kevinbacon say�s� 0 olan filmlere bakaca��m�z i�in i'nin daha �nce listeye eklenip eklenmedi�i kevinBacon boolean'� ile kontrol edildi. 
		if(!i -> kevinBacon){
			
			Actor* j = i->actorList;	//kevinbacon listesinde olmayan filmin akt�r listesi j iterat�r�ne al�nd�.
			ActorInfo* k = prevBacon;	//�nceki kavinbeacon dizisi k iterat�r�nde sakland�.			
			stop = false;	//E�er se�ilen filmin akt�r listesinin sonuna gelmemi�sek ve e�le�me olmam��sa loopta kal�nd�. e�le�me durumunda stop ile looptan ��k�ld�.
			
			while(j != NULL && !stop){
				ActorInfo* k = prevBacon;										//k iterat�r� her loopta kevinBacon listesinin ba��na al�nd�.
				
				while(k != NULL && strcmp(j->actorName, k->actorName) != 0){	//beaconlistteki akt�rlerden herhangi biri o filmde varm� kontrol edildi.
					k = k->next;
				}
				
				if(k!= NULL){
					stop =true;													//e�er e�le�me varsa looptan ��k�ld�. 
				}
				else{
					j = j->next;
				}
			
			}
			if( j != NULL){														//e�le�me olupta looptan ��k�lm��sa
				i->kevinBacon = true;											//o filmin kevinBacon de�eri true yap�ld�.
				j = i->actorList;												//j iterat�r� art�k kullanulmad���ndan filmin akt�r listesine tekrar atand�.

				while(j!=NULL){													//filmin t�m akt�rleri incelendi.
					
					if(!inList(nextBacon, j->actorName))						//inList ile zaten listede olup olmad�klar� kontrol edildi�
						nextBacon = addActorInfoList(nextBacon, j->actorName,i,k);	//e�er bulunan akt�rler listede de�illerse listeye elendi.
					j= j->next;													//i pointer� akt�re filmin ad�n� tuttu�undan kaydedildi.
				}																//k pointer� akt�re kendini kevinBeacon a ba�layan akt�r� tuttu�undan prev nod olarak kaydedildi.
				
			}
		}
		i = i->next;
	}
	return nextBacon;															//yeni liste d�nd�r�ld�.
}

//A�da olmayan oyuncular�n say�s�n� hesaplayan fonskiyon.
int findInfinite(Movie* list){
	//Film listesi i�in iterat�r al�nd�.
	Movie* i = list;
	//Bo� akt�r listesi olu�turuldu.
	ActorInfo* infiniteList = NULL;
	while(i != NULL){
		//Sadece kevinbacon say�s� 0 olan filmlere bakaca��z.
		if(!i -> kevinBacon){
			//iteratordeki i filminin akt�r listesi j iterat�r�ne al�nd�.
			Actor* j = i->actorList;
			while(j!=NULL){	
				//j iterat�r�ndeki akt�r listede mi kontrol edildi(inList ile). E�er listede de�ilse listeye eklendi. 
				if(!inList(infiniteList, j->actorName)){
					infiniteList = addActorInfoList(infiniteList, j->actorName,NULL,NULL);
				}
				j= j->next;
			}
		}
		i = i->next;
	}
	//T�m tarama sonucunda olu�an listenin boyutu hesaplan�p d�nd�r�ld�.
	return sizeActorInfoList(infiniteList);
}


//Actor stringini text halinden bizden istenilen formata d�n��t�ren fonksiyon.
char* formatActorString(char* name){
	int size = 0;
	while(name[size] != '\0'){
		size++;
	}
	//text boyutu �l��ld� ve o boyutta bir buff char dizisi olu�turuldu.
	char* buff = (char*) malloc(sizeof(char)*(size+1));
	int i = 0;
	//buff char dizisinde ',' karakterine kadar ilerlendi.
	while(i<size && name[i]!=','){
		i++;
	}
	//',' karakterinden 2 karakter sonras�ndan ba�layarak t�m karakterler  buff dizisinde kopyaland�.
	strcpy(buff,name+i+2);
	//bo�luk karakteri eklendi.
	strcat(buff," "); 
	//i de�i�keninde , karakterine kadar olan karakter say�s� hesaplanm�� oldu�undan o kadar karakter buff de�i�keninin sonuna eklendi.
	strncat(buff,name,i);
	return buff;
}


//Bu fonksiyonla olu�turulan prev nodlar�n� geriye do�ru takip edip ka��nc� kevin becon say�s�
//oldu�unu bulduk.
int calculateBacon(ActorInfo* actor){
	ActorInfo* iterator = actor;
	int i=-1;
	while(iterator!=NULL ){
		iterator = iterator->prev;
		i++;
	}
	return i;
}

//Film textini istenen forma getiren fonksiyon.
char* formatMovieString(char* name){
	//Al�nan metnin boyutu belirlenip ayn� boyutta buff isimli char olu�turuldu.
	int size = 0;
	while(name[size] != '\0'){
		size++;
	}
	char* buff = (char*) malloc(sizeof(char)*(size+1));
	//',' g�r�lene kadar text tarand�.
	int i = 0;
	while(i<size && name[i]!=','){
		i++;
	}
	//',' yoksa d�zenleme yap�lmad�.
	if(name[i]!=','){
		return name;
	}
	//',' ve sonras�ndaki bo�luk karakterleri de atland�.
	i+=2;
	//j iterat�r� �zerinde buff char� itere edildi. 
	// ',' ile '(' aras� buff a kopyaland�.
	int j =0;
	while(name[i+j] != '\0'&&name[i+j] != '('){
		buff[j] = name[i+j];
		j++;
	}
	//end de�i�keninde '(' in konumu sakland�.
	int end = i+j;
	//i en ba�a al�n�p ',' e kadarki k�s�m kopyaland�.
	i=0;
	while(name[i]!=','&&name[i] != '\0'){
		buff[j+i] = name[i];
		i++;
	}
	
	j=i+j;
	buff[j] = ' ';
	j++;
	//i '(' e atanarak. '(' sonras� karakterler k iterat�r� ile kopyaland�.
	i=end;
	int k= 0;
	while(name[i+k]!='\0'){
		buff[j+k]=name[i+k];
		k++;
	}
	buff[j+k]= '\0';
	return buff;
}


int main(int argc, char *argv[]) {
	char fileName[50];     			//Dosya Adn�n� Saklayan De�i�ken
	char name[48];					//Actor Ad�n� Saklayan De�i�ken
	char surname[48];				//Actor Soyad�n� saklayan de�i�ken
	char input[100];				//Actor ismini dosyadaki �ekliyle saklayan de�i�ken
	char selection=' ';				//Tekar arama yap�l�p yap�lmayaca��n� input olarak kaydeden karakter.
	printf("Welcome to the Kevin Beacon Calculator\n");         
	printf("Please enter a file name(Ex : input-1.txt) :");         
	scanf("%s",fileName);
	FILE* inputFile=fopen(fileName, "r");						//Al�nan dosya ad�na g�re input file pointer olu�turuldu
	if (inputFile == NULL) {									//Dosya a��lmazsa program sonland�r�ld�.
    	perror(fileName);
    	return 0;
	}
	Movie* movieList = createMovieList(inputFile,"r");			//A��lan dosyaya g�re film listesi olu�turuldu.
	fclose(inputFile);											//Dosya kapat�ld�.
	printf("Creating Kevin Bacon list...\n");
	printf("This may take some time depending the file size.\n");
	printf("Kevin Bacon Counts:\n");
	ActorInfo* actorList = createNewActorInfo("Bacon, Kevin",NULL,NULL);	//KevinBacon listesi ilk eleman� Kevin Bacon olacak �ekilde prev ve next nodlar� da NULL olacak �ekilde ayarland�.
	int size =0;												//Kevin Beacon akt�r listesinin boyutunu tutar.
	int newSize = 1; 											//Boyutun bir sonraki ad�mdaki durumunu tutar.
	int i=0;													//Bu de�i�ken sadece print i�leminde ka��nc� ad�mda oldu�umuzu g�stermek i�in kullan�ld�.
	while(size != newSize){										//listeye eleman eklenmeyinceye kadar kevinBacon listesi g�ncellendi.
		printf("%d.%d\n", i, newSize - size);					//Eleman say�s� fark� yazd�r�ld�.
		size = sizeActorInfoList(actorList);					//Eleman say�s�n�n ilk miktar� �l��ld�.
		actorList = findNextBacon(movieList,actorList);			//Bir sonraki ad�mdaki elemanlar eklendi.
		newSize = sizeActorInfoList(actorList);					//Eleman say�s�n�n sonraki ad�mdaki miktar� �l��ld�.
		i++;													//Ad�m say�s� artt�r�ld�.
	}
	int infinityCount = findInfinite(movieList);				//Hi� ortak akt�r olmayan filmlerdeki akt�rler findInfinite fonksiyonu ile hesaplan�p yazd�r�ld�.
	printf("Inf: %d\n", infinityCount);
	printf("Kevin Bacon list succesfully created.\n\n");
	while(selection !='n'){
		selection = ' ';										
		printf("Please enter actor's name:");
		scanf("%s",name);												//Kullan�c�dan isim ve soyisim bilgileri al�nd�.
		printf("Please enter actor's surname:");
		scanf("%s",surname);
		strcpy(input,surname);
		strcat(input,", ");
		strcat(input,name);
		ActorInfo* iterator = actorList;								//iterat�r olu�turulan akt�r listesine atand�.
		while(iterator!=NULL && strcmp(iterator->actorName,input)!=0){	//input al�nan isim varm� yok mu kontrol edildi.
			iterator = iterator->next;
		}
		if(iterator!=NULL){ //isim varsa prev nod say�s�yla kevin becon say�s� hesapland�. Prev nodlar s�ras� ile yazd�r�ld�. ��kt� i�in textler d�zenlendi.
			printf("%s's Kevin Bacon number is %d.\n",formatActorString(input),calculateBacon(iterator)); 
			while(iterator->prev !=NULL){
				printf("%s-%s : %s\n",formatActorString(iterator->prev->actorName),formatActorString(iterator->actorName),formatMovieString(iterator->baconFilm->movieName));
				iterator = iterator->prev;
			}
		}
		else{  //isim yoksa kevin becon say�s� sonuz olarak g�sterildi.
			printf("%s's Kevin Bacon number is infinite.\n",formatActorString(input));
		}
		printf("Do you want to search another person with same list?(y/n) :"); //Farkl� bir akt�r aran�p aranmayaca��na dair input al�nd�.
		while(selection != 'y' && selection != 'n'){						   //Farkl� akt�rle de yine ayn� liste kullan�larak h�zl�ca kevin beacon say�s� hesaplanabildi.
			scanf(" %c",&selection);
			if(selection != 'y' && selection != 'n'){
				printf("Invalid Input\n: ");
			}
		}
		printf("\n");
	}
	return 0;
}
