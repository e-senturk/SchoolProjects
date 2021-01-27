#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_TEXT 10000
#define EMPTY ' '


//Node yapýsýna insertion sortta kullanmak için prev node eklendi.
typedef struct Node{
	char harf;
	int frequency;
	struct Node* next;
	struct Node* prev;
	struct Node* left;
	struct Node* right;	
}List;

//Linkli liste yapýsýný verilen karakter deðeri ile oluþturan fonksiyon.
List* create_list(char a){
	List* new_list = NULL;
	new_list = (List*) malloc (sizeof(List));
	if(!new_list){
		printf("Malloc Failed \n");
	}
	//Verilen karakter struct yapýsýna frekansý 1 olacak þekilde eklendi.
	//Baðlý nodlar nullptr ye atandý.
	new_list -> harf = a;
	new_list -> frequency = 1;
	new_list -> next = NULL;
	new_list -> prev = NULL;
	new_list -> left = NULL;
	new_list -> right = NULL;
	return new_list;
}

//Linkli Listeye verilen karaktere uygun þekilde eleman oluþturup ekleyen fonksiiyon. 
List* add_char_to_list(List* head,char x){
	List *p = head;
	//Listedeki elemanlar içerisinde verilen harf varmý diye arandý.
	while(p -> next != NULL && p ->harf != x){
		p = p -> next;
	}
	//Eðer harf bulunmuþsa o nodun frekansý arttýrýldý.
	if(p ->harf == x || p -> next != NULL){
		p -> frequency ++;
	}
	//Eðer harf bulunamamýþsa yeni nod oluþturulup listeye eklendi.
	else {
		List *temp = create_list(x);
		p->next = temp;
		temp -> prev = p;
	}
	return head;
}

//Verilen linkli listeyi insertion sort metodu ile sýralayan fonksiyon.
List* insertion_sort(List* head){
	//p nodunu insertion sortta 1. elemanda iþlem yapýlmadýðýndan 2. elemana atandý.
	List* p = head -> next;
	List* q;
	List* temp;
	//Listedeki elemanlar 2. elemandan baþlayarak sýrasý ile kendindeki elemandan öncekilerle karþýlaþtýrýldý.
	while(p != NULL ){
		q = p -> prev;
		//Kendinden sonraki kaçýncý elemandan daha büyük olduðunu bulmak için geriye doðru ilerlendi. Küçük olan eleman varsa Eleman q ya kaydedildi.
		while(q != NULL && p -> frequency < q -> frequency){
			q = q ->prev;
		}
		//p deðerini temp deðiþkeninde saklayarak iþlem sonunda tekrar p ye atandý bu sayede p deðeri üzerinde deðiþim olmamasý saðlandý.
		temp = p;
		//p deðerinden önceki eleman daha büyükse ilerleme olmayacaðýndan q nun üstteki while döngüsüne girip girmediði kontrol edildi.
		//Eðer döngüye girilmiþse p elemanýnýn son eleman olup olmadýðý veya q elemanýnýn ilk elemandan önceki NULL pointera ulaþýp ulaþmadýðýna göre
		//gerekli iþlemler yapýlarak p nodu gerekli yere taþýndý. Bu iþlem tüm elemanlar için tekrarlandý ve linkli liste sýralandý.
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
//Verilen linkli listeyi huffman treeye çeviren fonksiyon
List* convert_huffman(List* head){
	List* p = head;
	List* temp;
	List* root;
	// Tree adýnda bir aðaç oluþturuldu.
	List* tree = create_list(EMPTY);
	//Boþ 1 ve 2 elemanlý listeler için gerekli exceptionlar oluþturuldu.
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
		//p'nin son 2 eleman kalýncaya kadar ilerleme yapmasý için kendinden 2sonraki elemanýn NULL olup olmadýðýna bakýldý. 
		while(p ->next -> next != NULL){
			//Root nodu harf deðeri boþ olacak þekilde oluþturulup listenin ilk iki elemaný left ve right nodlara eklendi.
			root = create_list(EMPTY);
			root -> frequency =  p -> frequency + p -> next -> frequency;
			root -> left = p;
			root -> right = p -> next;			 
			p = p -> next -> next;
			root -> left -> next = NULL;
			root -> left -> prev = NULL;
			root -> right -> next = NULL;
			root -> right -> prev = NULL;	
			//p nodunu deðiþtirmemek için temp nodu oluþturuldu. 
			temp = p;
			//temp nodu üzerinde root nodu kendinden daha büyük bir nod bulununcaya kadar ilerleme yapýldý.
			while(temp -> next != NULL && temp -> frequency <= root -> frequency){
				temp = temp -> next;
			}
			//Eðer öyle bir nod bulunmamýþsa root nodu en sona eklendi.
			if(temp -> next == NULL && temp -> frequency <= root -> frequency){
					temp -> next = root;
					root -> prev = temp;
			}
			//Eðer nod bulunduysa root nodu araya eklendi.
			else{
				root -> prev = temp ->prev;
				root -> next = temp;
				root -> prev -> next = root;
				root -> next -> prev = root;
			}
		}
		//Kalan son 2 noddan küçük olan tree nodunun soluna büyük olan saðýna eklendi. Tree frekansý kalan 2 nodun toplamý olacak þekilde ayarlandý.
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
//Verilen binary treenin yüksekliðini bulan fonksiyon
int tree_height(List* root)  
{ 
	//Eðer null noduna ulaþýlmýþsa o noddan sonraki yükseklik 0 olduðundan 0 döndürüldü.
   if (root==NULL)  
       return 0; 
   else 
   { 
   	//Eðer null noduna ulaþýlmamýþsa o nodun sol ve sað nodlarýnýn derinliði rekürsif olarak hesaplanýp büyük olana dönüldü büyük olana dönüldü.
    	int left_depth = tree_height(root->left); 
    	int right_depth = tree_height(root->right); 
		if (left_depth > right_depth)  
			return(left_depth+1); 
		else 
	   		return(right_depth+1); 
   } 
}  
//Verilen binary treenin istenilen seviyesini null karakterleri de dahil ederek bastýran fonksiyon
void print_level(List* root, int level) { 
	//root verilen nod null deðilse o nodda level 1 oluncaya kadar bütün left ve right nodlarý bulmak için else if kýsmýný kullandýk eðer level 1 olursa istediðimiz
	//levele ulaþmýþ olacaðýmýzdan print yaptýk bu sayede sýrasý ile o seviyedeki tüm nodlarý yazdýrmýþ olduk.
    if (root != NULL) {
       if (level == 1)
          printf("%d%c ",root->frequency,root->harf);
       else if (level > 1) {
       	  print_level(root->left, level-1);
       	  print_level(root->right, level-1);
       }
    }
    //Eðer level üzerindeki nod NULL ise boþluk yazdýrýldý.
    else
    	printf("   ");
}

//print_level ve tree_height fonksiyonunu kullarak huffman treeyi yazdýran fonksiyon.
void print_huffman(List* node){
	//Aðacýn uzunluðunu bulup her uzunluðu tek tek yazdýrdýk.
	int height = tree_height(node),i; 
	for(i = 1;i <= height; i++){
		print_level(node,i);
		printf("\n");
	}
} 

//Kullanýcýdan text alýp üstteki fonksiyonlara yönlendirilerek gerekli iþlemlerin yapýldýðý ana fonksiyon.
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
