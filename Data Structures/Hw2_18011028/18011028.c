#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_TEXT 10000
#define EMPTY ' '


//Node yap�s�na insertion sortta kullanmak i�in prev node eklendi.
typedef struct Node{
	char harf;
	int frequency;
	struct Node* next;
	struct Node* prev;
	struct Node* left;
	struct Node* right;	
}List;

//Linkli liste yap�s�n� verilen karakter de�eri ile olu�turan fonksiyon.
List* create_list(char a){
	List* new_list = NULL;
	new_list = (List*) malloc (sizeof(List));
	if(!new_list){
		printf("Malloc Failed \n");
	}
	//Verilen karakter struct yap�s�na frekans� 1 olacak �ekilde eklendi.
	//Ba�l� nodlar nullptr ye atand�.
	new_list -> harf = a;
	new_list -> frequency = 1;
	new_list -> next = NULL;
	new_list -> prev = NULL;
	new_list -> left = NULL;
	new_list -> right = NULL;
	return new_list;
}

//Linkli Listeye verilen karaktere uygun �ekilde eleman olu�turup ekleyen fonksiiyon. 
List* add_char_to_list(List* head,char x){
	List *p = head;
	//Listedeki elemanlar i�erisinde verilen harf varm� diye arand�.
	while(p -> next != NULL && p ->harf != x){
		p = p -> next;
	}
	//E�er harf bulunmu�sa o nodun frekans� artt�r�ld�.
	if(p ->harf == x || p -> next != NULL){
		p -> frequency ++;
	}
	//E�er harf bulunamam��sa yeni nod olu�turulup listeye eklendi.
	else {
		List *temp = create_list(x);
		p->next = temp;
		temp -> prev = p;
	}
	return head;
}

//Verilen linkli listeyi insertion sort metodu ile s�ralayan fonksiyon.
List* insertion_sort(List* head){
	//p nodunu insertion sortta 1. elemanda i�lem yap�lmad���ndan 2. elemana atand�.
	List* p = head -> next;
	List* q;
	List* temp;
	//Listedeki elemanlar 2. elemandan ba�layarak s�ras� ile kendindeki elemandan �ncekilerle kar��la�t�r�ld�.
	while(p != NULL ){
		q = p -> prev;
		//Kendinden sonraki ka��nc� elemandan daha b�y�k oldu�unu bulmak i�in geriye do�ru ilerlendi. K���k olan eleman varsa Eleman q ya kaydedildi.
		while(q != NULL && p -> frequency < q -> frequency){
			q = q ->prev;
		}
		//p de�erini temp de�i�keninde saklayarak i�lem sonunda tekrar p ye atand� bu sayede p de�eri �zerinde de�i�im olmamas� sa�land�.
		temp = p;
		//p de�erinden �nceki eleman daha b�y�kse ilerleme olmayaca��ndan q nun �stteki while d�ng�s�ne girip girmedi�i kontrol edildi.
		//E�er d�ng�ye girilmi�se p eleman�n�n son eleman olup olmad��� veya q eleman�n�n ilk elemandan �nceki NULL pointera ula��p ula�mad���na g�re
		//gerekli i�lemler yap�larak p nodu gerekli yere ta��nd�. Bu i�lem t�m elemanlar i�in tekrarland� ve linkli liste s�raland�.
		if(p -> prev != q ){
			if(p -> next != NULL)
				p -> next -> prev = p -> prev;
			p -> prev -> next = p -> next;
			if(q == NULL){
				p -> next = head;
				head -> prev = p;
				p -> prev = NULL;
				head = p;
			}
			else{
				p -> next = q -> next;
				p -> prev = q;
				p -> next -> prev = p;
				q -> next = p;
			}
		}
		p = temp -> next;		
	}
	return head;
}
//Verilen linkli listeyi huffman treeye �eviren fonksiyon
List* convert_huffman(List* head){
	List* p = head;
	List* temp;
	List* root;
	// Tree ad�nda bir a�a� olu�turuldu.
	List* tree = create_list(EMPTY);
	//Bo� 1 ve 2 elemanl� listeler i�in gerekli exceptionlar olu�turuldu.
	if(head == NULL){
		perror("Head is Empty");
		return head;
	}
	if(head -> next == NULL){
		tree = head;
		return head;
	}
	if(head -> next -> next == NULL){
		tree -> right = head;
		tree -> left = head -> next;
		tree -> frequency = head -> frequency + head -> next -> frequency;
		return tree;
	}
	else{
		//p'nin son 2 eleman kal�ncaya kadar ilerleme yapmas� i�in kendinden 2sonraki eleman�n NULL olup olmad���na bak�ld�. 
		while(p ->next -> next != NULL){
			//Root nodu harf de�eri bo� olacak �ekilde olu�turulup listenin ilk iki eleman� left ve right nodlara eklendi.
			root = create_list(EMPTY);
			root -> frequency =  p -> frequency + p -> next -> frequency;
			root -> left = p;
			root -> right = p -> next;			 
			p = p -> next -> next;
			root -> left -> next = NULL;
			root -> left -> prev = NULL;
			root -> right -> next = NULL;
			root -> right -> prev = NULL;	
			//p nodunu de�i�tirmemek i�in temp nodu olu�turuldu. 
			temp = p;
			//temp nodu �zerinde root nodu kendinden daha b�y�k bir nod bulununcaya kadar ilerleme yap�ld�.
			while(temp -> next != NULL && temp -> frequency <= root -> frequency){
				temp = temp -> next;
			}
			//E�er �yle bir nod bulunmam��sa root nodu en sona eklendi.
			if(temp -> next == NULL && temp -> frequency <= root -> frequency){
					temp -> next = root;
					root -> prev = temp;
			}
			//E�er nod bulunduysa root nodu araya eklendi.
			else{
				root -> prev = temp ->prev;
				root -> next = temp;
				root -> prev -> next = root;
				root -> next -> prev = root;
			}
		}
		//Kalan son 2 noddan k���k olan tree nodunun soluna b�y�k olan sa��na eklendi. Tree frekans� kalan 2 nodun toplam� olacak �ekilde ayarland�.
		if(root -> frequency < temp ->frequency){
			tree -> left = root;
			tree -> right = temp;	
		}
		else{
			tree -> right = root;
			tree -> left = temp;
		}
		tree -> frequency = root -> frequency + temp -> frequency;
	}
	return tree;
}
//Verilen binary treenin y�ksekli�ini bulan fonksiyon
int tree_height(List* root)  
{ 
	//E�er null noduna ula��lm��sa o noddan sonraki y�kseklik 0 oldu�undan 0 d�nd�r�ld�.
   if (root==NULL)  
       return 0; 
   else 
   { 
   	//E�er null noduna ula��lmam��sa o nodun sol ve sa� nodlar�n�n derinli�i rek�rsif olarak hesaplan�p b�y�k olana d�n�ld� b�y�k olana d�n�ld�.
    	int left_depth = tree_height(root->left); 
    	int right_depth = tree_height(root->right); 
		if (left_depth > right_depth)  
			return(left_depth+1); 
		else 
	   		return(right_depth+1); 
   } 
}  
//Verilen binary treenin istenilen seviyesini null karakterleri de dahil ederek bast�ran fonksiyon
void print_level(List* root, int level) { 
	//root verilen nod null de�ilse o nodda level 1 oluncaya kadar b�t�n left ve right nodlar� bulmak i�in else if k�sm�n� kulland�k e�er level 1 olursa istedi�imiz
	//levele ula�m�� olaca��m�zdan print yapt�k bu sayede s�ras� ile o seviyedeki t�m nodlar� yazd�rm�� olduk.
    if (root != NULL) {
       if (level == 1)
          printf("%d%c ",root->frequency,root->harf);
       else if (level > 1) {
       	  print_level(root->left, level-1);
       	  print_level(root->right, level-1);
       }
    }
    //E�er level �zerindeki nod NULL ise bo�luk yazd�r�ld�.
    else
    	printf("   ");
}

//print_level ve tree_height fonksiyonunu kullarak huffman treeyi yazd�ran fonksiyon.
void print_huffman(List* node){
	//A�ac�n uzunlu�unu bulup her uzunlu�u tek tek yazd�rd�k.
	int height = tree_height(node),i; 
	for(i = 1;i <= height; i++){
		print_level(node,i);
		printf("\n");
	}
} 

//Kullan�c�dan text al�p �stteki fonksiyonlara y�nlendirilerek gerekli i�lemlerin yap�ld��� ana fonksiyon.
int main() {
	printf("Metni Giriniz: ");
	char text [MAX_TEXT];
	printf("%s\n",text);
	gets(text);
	List* head;
	if(text[0] == '\0'){
		head = NULL;
		perror("Metin Bos Olamaz");
		return 0;
	}
	else{
		head = create_list(text[0]);
	}
	int i=1;
	while(text[i] != '\0'){
		head = add_char_to_list(head, text[i]);
		i++;
	}
	head = insertion_sort(head);
	printf("\n\t\tHUFFMAN TREE\n\n");
	head = convert_huffman(head);
	print_huffman(head);
	return 0;
}
