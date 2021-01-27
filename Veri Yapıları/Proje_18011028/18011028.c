#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFSIZE 4096  //alýnan bir satýr için kullanýlcak buffer boyutunu gösterir.
	

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//Dosyadan okunan aktörleri tutan struct yapýsý
typedef struct Node{
	char actorName [100]; 	//Aktör adýný içerir
	struct Node* next; 		//Aktörden sonraki nodu içerir.
}Actor;

//Filmleri aktörleriye tutan struct yapýsý
typedef struct Node2{
	char movieName[100]; 	//Film adýný içerir.
	bool kevinBacon;  		//Filmin kevinBacon listesine eklenip eklenmediðini saklar.
	Actor* actorList; 		//Filmde oynayan aktörlerin listesi
	struct Node2* next;
}Movie;

//KevinBacon listesini tutan struct yapýsý //Actor listesi de kullanýlabilirdi ancak ek veri yapýsý zaman ve bellek optimizasyonu için oluþturuldu.
typedef struct Node3{
	char actorName [100];	//Aktör adýný içerir
	Movie* baconFilm;		//Film adýný içerir. //Bu iþemde strcpy yerine nesne kullandý. 
	struct Node3* prev;		//Kevin bacon sýralamasýnda kendinden önce gelen aktörü tutar. -> Bu deðiþken kevin bacon listesinin eski elemanlarýný listeyi taramadan bulmaya yarýyor.
	struct Node3* next;		//Liste sýrasýnda sonraki aktörü tutar.->Bu deðiþken iterasyonu saðlamak için kullanýlýyor.
}ActorInfo;


//Actor nesnesi oluþturan fonksiyon
	//Deðiþken olarak aktörün adýný alýp sonraki nodu NULL olacak þekilde ayarlar
Actor* createNewActor(char* actorName){
	Actor* newElement = (Actor*) malloc(sizeof(Actor));
	strcpy(newElement -> actorName, actorName);
	newElement -> next = NULL;
	return newElement;
}

//Actor listesine eleman ekleyen fonksiyon
	//Deðiþken olarak actörün adýný ve aktör listesinin head nodunu alýr.
	//Yeni bir nod oluþturup linkli listenin en baþýna baðlayarak linkli listenin her yeni eklenen elemanýnýn en baþa eklenmesini saðlar.
	//Liste boþsa yeni eleman oluþturur.
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

//AktorInfo listesi oluþturan fonksiyon
	//Actor listesine benzer bir yapýdadýr.
	//Film adý strcpy daha yavaþ olduðundan filmin pointerý olacak þekilde tutuldu.
	//prev nod olarak bu nodda kendisini kevinbacon listesine baðlayan eleman tutuldu.
ActorInfo* createNewActorInfo(char* actorName, Movie* baconFilm, ActorInfo* prev) {
	ActorInfo* newElement = (ActorInfo*) malloc(sizeof(ActorInfo));
	strcpy(newElement -> actorName, actorName);
	newElement->baconFilm = baconFilm;
	newElement->prev=prev;
	newElement -> next = NULL;
	return newElement;
}

//ActorInfo listesine eleman ekleyen fonksiyon
	//Deðiþken olarak actörün adýný ve aktör listesinin head nodunu alýr.
	//Yeni bir nod oluþturup linkli listenin en baþýna baðlayarak linkli listenin her yeni eklenen elemanýnýn en baþa eklenmesini saðlar.
	//Liste boþsa yeni eleman oluþturur.

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

//Verilen line üzerinden film adýný ve aktörlerini çekip yeni film oluþturan fonksiyon.

Movie* createNewMovie(char* line){
	Movie* newMovie = (Movie*) malloc (sizeof(Movie));
	newMovie -> kevinBacon = false;                        //Yeni film kevinbacon listesinde henüz olmadýðýndan deðeri false olarak atandý. 
	int i = 0, j;
	char* temp =(char*)malloc(sizeof(char)*100);           //Geçici deðiþkenleri tutmak için 100 charlýk yer ayrýldý.
	while(line[i]!= '\0' &&line[i] != '/'){				   //Filmin adý ilk while döngüsü ile alýndý. "/" karakterini görünceye kadar elemanlar filme adýna eklendi.
		temp[i] = line [i]; 
		i++;
	}
	temp[i] = '\0';										   //Film adýnýn son karakteri string sonu karakterine atandý.
	i++;											       // '/' iþareti atlandý.
	strcpy(newMovie -> movieName, temp);                   //Oluþturulan film adý yeni nesneye girildi.
	//actör
	newMovie->actorList = NULL;						       //Actör listesi oluþturuldu.
	while(line[i]!= '\0'){
		j=0;
		while(line[i] !='/' && line[i+1]!= '\0'){		   //Film adý hesaplamaya benzer bir biçimde her '/' karakteri öncesinde kalan substring tempde saklandý.
			temp[j] = line[i];
			i++; j++;										//i iteratörü line için j iteratörü temp için kullanýldý.
		}
		temp[j] = '\0';										//substringin sonuna string sonu karakteri eklendi.
		Actor*tempActor = createNewActor(temp);				//tempActor listesine her substing tanýmlý metoddar araclýðý ile oluþturulup eklendi.
		tempActor->next = newMovie->actorList;				
		newMovie->actorList =tempActor;
		i++;												// '/' karakteri atlandý.
	}
	return newMovie;										//Oluþturulan yeni film nesnesi döndürüldü.
}

//Film listesine eleman ekleyen fonksiyon
	//Deðiþken olarak film lineýný ve film listesinin head nodunu alýr.
	//Yeni bir nod oluþturup linkli listenin en baþýna baðlayarak linkli listenin her yeni eklenen elemanýnýn en baþa eklenmesini saðlar.
	//Liste boþsa yeni eleman oluþturur.
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

//Verilen bir file pointerdan satýr satýr string okuyan ve film listesi oluþturan fonksiyon.
Movie* createMovieList(FILE* input, char* readMode){
	Movie* movieList = NULL;						//Filmler listesi
	char line[BUFFSIZE];                			//BUFFSIZE define bölümünden 4KB olacak þekilde ayarlandý.
	while (fgets(line, sizeof(line),input)) {		//Dosyadan her seferde 1 satýr okumak için kullanýlan kod.
       	movieList=addMovieList (movieList ,line);	//Film listesine film eklemek için addMovieList fonksiyonu kullanýldý.
    }
	return movieList;
}




//KevinBacon Actör listesini itere ederek boyutunu döndüren fonksiyon.
int sizeActorInfoList(ActorInfo* head){
	ActorInfo* iterator = head;
	int i =0;
	while(iterator != NULL){
		iterator = iterator -> next;
		i++;
	}
	return i;
}



//Verilen textin head nodu verilen kevinBacon listesinde olup olmadýðýný listenin tüm elemanlarýný tarayarak bulan fonksiyon.
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
	ActorInfo* nextBacon =prevBacon;       //Önceki kevinBacon listesi yeniListeye atandý.
	bool stop;							   //Looptan çýkmak için kullanýlan deðiþken.
	Movie* i = list;					   //Film listesi i iteratörüne alýndý.
	while(i != NULL){
		//Sadece kevinbacon sayýsý 0 olan filmlere bakacaðýmýz için i'nin daha önce listeye eklenip eklenmediði kevinBacon boolean'ý ile kontrol edildi. 
		if(!i -> kevinBacon){
			
			Actor* j = i->actorList;	//kevinbacon listesinde olmayan filmin aktör listesi j iteratörüne alýndý.
			ActorInfo* k = prevBacon;	//Önceki kavinbeacon dizisi k iteratöründe saklandý.			
			stop = false;	//Eðer seçilen filmin aktör listesinin sonuna gelmemiþsek ve eþleþme olmamýþsa loopta kalýndý. eþleþme durumunda stop ile looptan çýkýldý.
			
			while(j != NULL && !stop){
				ActorInfo* k = prevBacon;										//k iteratörü her loopta kevinBacon listesinin baþýna alýndý.
				
				while(k != NULL && strcmp(j->actorName, k->actorName) != 0){	//beaconlistteki aktörlerden herhangi biri o filmde varmý kontrol edildi.
					k = k->next;
				}
				
				if(k!= NULL){
					stop =true;													//eðer eþleþme varsa looptan çýkýldý. 
				}
				else{
					j = j->next;
				}
			
			}
			if( j != NULL){														//eþleþme olupta looptan çýkýlmýþsa
				i->kevinBacon = true;											//o filmin kevinBacon deðeri true yapýldý.
				j = i->actorList;												//j iteratörü artýk kullanulmadýðýndan filmin aktör listesine tekrar atandý.

				while(j!=NULL){													//filmin tüm aktörleri incelendi.
					
					if(!inList(nextBacon, j->actorName))						//inList ile zaten listede olup olmadýklarý kontrol edildiç
						nextBacon = addActorInfoList(nextBacon, j->actorName,i,k);	//eðer bulunan aktörler listede deðillerse listeye elendi.
					j= j->next;													//i pointerý aktöre filmin adýný tuttuðundan kaydedildi.
				}																//k pointerý aktöre kendini kevinBeacon a baðlayan aktörü tuttuðundan prev nod olarak kaydedildi.
				
			}
		}
		i = i->next;
	}
	return nextBacon;															//yeni liste döndürüldü.
}

//Aðda olmayan oyuncularýn sayýsýný hesaplayan fonskiyon.
int findInfinite(Movie* list){
	//Film listesi için iteratör alýndý.
	Movie* i = list;
	//Boþ aktör listesi oluþturuldu.
	ActorInfo* infiniteList = NULL;
	while(i != NULL){
		//Sadece kevinbacon sayýsý 0 olan filmlere bakacaðýz.
		if(!i -> kevinBacon){
			//iteratordeki i filminin aktör listesi j iteratörüne alýndý.
			Actor* j = i->actorList;
			while(j!=NULL){	
				//j iteratöründeki aktör listede mi kontrol edildi(inList ile). Eðer listede deðilse listeye eklendi. 
				if(!inList(infiniteList, j->actorName)){
					infiniteList = addActorInfoList(infiniteList, j->actorName,NULL,NULL);
				}
				j= j->next;
			}
		}
		i = i->next;
	}
	//Tüm tarama sonucunda oluþan listenin boyutu hesaplanýp döndürüldü.
	return sizeActorInfoList(infiniteList);
}


//Actor stringini text halinden bizden istenilen formata dönüþtüren fonksiyon.
char* formatActorString(char* name){
	int size = 0;
	while(name[size] != '\0'){
		size++;
	}
	//text boyutu ölçüldü ve o boyutta bir buff char dizisi oluþturuldu.
	char* buff = (char*) malloc(sizeof(char)*(size+1));
	int i = 0;
	//buff char dizisinde ',' karakterine kadar ilerlendi.
	while(i<size && name[i]!=','){
		i++;
	}
	//',' karakterinden 2 karakter sonrasýndan baþlayarak tüm karakterler  buff dizisinde kopyalandý.
	strcpy(buff,name+i+2);
	//boþluk karakteri eklendi.
	strcat(buff," "); 
	//i deðiþkeninde , karakterine kadar olan karakter sayýsý hesaplanmýþ olduðundan o kadar karakter buff deðiþkeninin sonuna eklendi.
	strncat(buff,name,i);
	return buff;
}


//Bu fonksiyonla oluþturulan prev nodlarýný geriye doðru takip edip kaçýncý kevin becon sayýsý
//olduðunu bulduk.
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
	//Alýnan metnin boyutu belirlenip ayný boyutta buff isimli char oluþturuldu.
	int size = 0;
	while(name[size] != '\0'){
		size++;
	}
	char* buff = (char*) malloc(sizeof(char)*(size+1));
	//',' görülene kadar text tarandý.
	int i = 0;
	while(i<size && name[i]!=','){
		i++;
	}
	//',' yoksa düzenleme yapýlmadý.
	if(name[i]!=','){
		return name;
	}
	//',' ve sonrasýndaki boþluk karakterleri de atlandý.
	i+=2;
	//j iteratörü üzerinde buff charý itere edildi. 
	// ',' ile '(' arasý buff a kopyalandý.
	int j =0;
	while(name[i+j] != '\0'&&name[i+j] != '('){
		buff[j] = name[i+j];
		j++;
	}
	//end deðiþkeninde '(' in konumu saklandý.
	int end = i+j;
	//i en baþa alýnýp ',' e kadarki kýsým kopyalandý.
	i=0;
	while(name[i]!=','&&name[i] != '\0'){
		buff[j+i] = name[i];
		i++;
	}
	
	j=i+j;
	buff[j] = ' ';
	j++;
	//i '(' e atanarak. '(' sonrasý karakterler k iteratörü ile kopyalandý.
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
	char fileName[50];     			//Dosya Adnýný Saklayan Deðiþken
	char name[48];					//Actor Adýný Saklayan Deðiþken
	char surname[48];				//Actor Soyadýný saklayan deðiþken
	char input[100];				//Actor ismini dosyadaki þekliyle saklayan deðiþken
	char selection=' ';				//Tekar arama yapýlýp yapýlmayacaðýný input olarak kaydeden karakter.
	printf("Welcome to the Kevin Beacon Calculator\n");         
	printf("Please enter a file name(Ex : input-1.txt) :");         
	scanf("%s",fileName);
	FILE* inputFile=fopen(fileName, "r");						//Alýnan dosya adýna göre input file pointer oluþturuldu
	if (inputFile == NULL) {									//Dosya açýlmazsa program sonlandýrýldý.
    	perror(fileName);
    	return 0;
	}
	Movie* movieList = createMovieList(inputFile,"r");			//Açýlan dosyaya göre film listesi oluþturuldu.
	fclose(inputFile);											//Dosya kapatýldý.
	printf("Creating Kevin Bacon list...\n");
	printf("This may take some time depending the file size.\n");
	printf("Kevin Bacon Counts:\n");
	ActorInfo* actorList = createNewActorInfo("Bacon, Kevin",NULL,NULL);	//KevinBacon listesi ilk elemaný Kevin Bacon olacak þekilde prev ve next nodlarý da NULL olacak þekilde ayarlandý.
	int size =0;												//Kevin Beacon aktör listesinin boyutunu tutar.
	int newSize = 1; 											//Boyutun bir sonraki adýmdaki durumunu tutar.
	int i=0;													//Bu deðiþken sadece print iþleminde kaçýncý adýmda olduðumuzu göstermek için kullanýldý.
	while(size != newSize){										//listeye eleman eklenmeyinceye kadar kevinBacon listesi güncellendi.
		printf("%d.%d\n", i, newSize - size);					//Eleman sayýsý farký yazdýrýldý.
		size = sizeActorInfoList(actorList);					//Eleman sayýsýnýn ilk miktarý ölçüldü.
		actorList = findNextBacon(movieList,actorList);			//Bir sonraki adýmdaki elemanlar eklendi.
		newSize = sizeActorInfoList(actorList);					//Eleman sayýsýnýn sonraki adýmdaki miktarý ölçüldü.
		i++;													//Adým sayýsý arttýrýldý.
	}
	int infinityCount = findInfinite(movieList);				//Hiç ortak aktör olmayan filmlerdeki aktörler findInfinite fonksiyonu ile hesaplanýp yazdýrýldý.
	printf("Inf: %d\n", infinityCount);
	printf("Kevin Bacon list succesfully created.\n\n");
	while(selection !='n'){
		selection = ' ';										
		printf("Please enter actor's name:");
		scanf("%s",name);												//Kullanýcýdan isim ve soyisim bilgileri alýndý.
		printf("Please enter actor's surname:");
		scanf("%s",surname);
		strcpy(input,surname);
		strcat(input,", ");
		strcat(input,name);
		ActorInfo* iterator = actorList;								//iteratör oluþturulan aktör listesine atandý.
		while(iterator!=NULL && strcmp(iterator->actorName,input)!=0){	//input alýnan isim varmý yok mu kontrol edildi.
			iterator = iterator->next;
		}
		if(iterator!=NULL){ //isim varsa prev nod sayýsýyla kevin becon sayýsý hesaplandý. Prev nodlar sýrasý ile yazdýrýldý. Çýktý için textler düzenlendi.
			printf("%s's Kevin Bacon number is %d.\n",formatActorString(input),calculateBacon(iterator)); 
			while(iterator->prev !=NULL){
				printf("%s-%s : %s\n",formatActorString(iterator->prev->actorName),formatActorString(iterator->actorName),formatMovieString(iterator->baconFilm->movieName));
				iterator = iterator->prev;
			}
		}
		else{  //isim yoksa kevin becon sayýsý sonuz olarak gösterildi.
			printf("%s's Kevin Bacon number is infinite.\n",formatActorString(input));
		}
		printf("Do you want to search another person with same list?(y/n) :"); //Farklý bir aktör aranýp aranmayacaðýna dair input alýndý.
		while(selection != 'y' && selection != 'n'){						   //Farklý aktörle de yine ayný liste kullanýlarak hýzlýca kevin beacon sayýsý hesaplanabildi.
			scanf(" %c",&selection);
			if(selection != 'y' && selection != 'n'){
				printf("Invalid Input\n: ");
			}
		}
		printf("\n");
	}
	return 0;
}
