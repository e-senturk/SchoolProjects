% İsimler ve fiiller dosyasını import etme bölümü
:-include('isimler.pl').
:-include('fiiller.pl').

% İlişkiyi istenilen formatta yazdıran fonksiyon
printconnection(Value1,Connection,Value2):-
	write('iliski('),write(Value1),
	write(', '),write(Connection),
	write(', '),write(Value2),write('), ').

% Sonuç elemanlarını yazdıran ve sonuç boyutunu tutan fonksiyon.
print_list([_],[],N):- write('\tuzunluk: '),write(N),write('.\n\n').
print_list([Head,Middle|Tail],[H|T],N):-
	printconnection(Head,H,Middle),
	K is N+1,
	print_list([Middle|Tail],T,K).

% Bağlantının input olarak alınan dizide olup olmadığını tespit eden fonksiyon.
find(Connection,[Connection|_]).
find(Connection,[Head|Tail]) :-
	Connection \== Head,
	find(Connection,Tail).
	
% Dizide arama modu kontrol fonksiyonu.
check(Connection,Array,Mode) :- Mode = 1 -> find(Connection,Array).
check(Connection,Array,Mode) :- Mode = 2 -> \+find(Connection,Array).


% Çift taraflı ilişki
connection(Value1,Connecter,Value2):-
		(iliski(Value1,Connecter,Value2);iliski(Value2,Connecter,Value1)).


% Breadth-first search algoritması
  
% Yeni hedef destination ise dur.
% Gerekli yazdırma fonksiyonları çağırıldı.
% 1. sıra zaten yazdırıldığından dizi boyutu 1 olarak yazdırma fonksiyonuna gönderildi.
bfs(Destination,[n(Destination,[PathHead|PathTail],[ElemHead|ElemTail])|_],_,_,_) :- 
	printconnection(Destination,PathHead,ElemHead),print_list([ElemHead|ElemTail],PathTail,1).
	

/*
 Algoritmada kullanılan değişkenler:
	- Destination : Hedefe varılması istenilen nokta.
	- n(Target,Path,Elements) : Bu 3 lü yapıda Target olarak elde edilen düğümlerin varması beklenen hedefi,
		Path : Hedef için izlenecek yolu, Elements : Hedefe giderken geçilen elemanlar.
		PrevCollection: Önceki loopta elde edilen tüm değerleri saklayan yapı.
		Acquired: Tekrarsız olarak geçilen elemanları saklayan yapı
		Array: Dahil edilecek veya görmezden gelincek dizi
		Mode: Dizinin hangi modda işleneceği
	- Birinci adımda find all ile ilişki bağlantısı olan,
	check metodundan geçebilecek ilişki türlerindeki bağlantılar,
	Acqired dizisinde daha önce tekrar etmiyorsa diziye eklendi.
	findall ile bulunan tüm sonuçlar NewCollection Dizisinde saklandı.
	append fonksiyonu ile önceki adımda bulunan Prev Collection ile birleştirilip 
	Total Collection dizisi halinde sonraki iterasyona gönderildi.
	Target önceki adımda oluşturulan Acquired dizisine eklendi.
	İşlem adımları Target değeri destinationa eşit oluncaya kadar tekrarlandı.
*/
	
bfs(Destination,[n(Target,Path,Elements)|PrevCollection],Acquired,Array,Mode) :- 
	findall(n(NewElement,[Connection|Path],[Target|Elements]),
	(connection(Target,Connection,NewElement),check(Connection,Array,Mode), \+ member(NewElement,Acquired)),
	NewCollection),
	append(PrevCollection,NewCollection,TotalCollection),
	bfs(Destination,TotalCollection,[Target|Acquired],Array,Mode). 
	
% Arayüzden çağırılan ana fonksiyon
calculate(Start,Destination,Array,Mode) :-
	bfs(Destination,[n(Start,[],[])],[],Array,Mode).

% Kullanıcı için input output almak amacı ile bir fonksiyon oluşturuldu.
main():-
	write('..Prolog ile en kisa yol bulma uygulamasina hos geldiniz..\n'),
	write('Lutfen ilk kelimeyi giriniz.\n'),
	read(Start),
	write('Lutfen ikinci kelimeyi giriniz.\n'),
	read(End),
	write('Diziyi giriniz.\n'),
	read(Array),
	write('Lutfen arama metodunu giriniz\n\t1- Icindekileri ara\n\t2- Disindakileri ara\n'),
	read(Mode),
	calculate(Start,End,Array,Mode),
	write('Tekrar arama yapmak icin 1 e basin\n'),
	read(K),
	( K=1 -> main();  write('Program sonlandirildi\n')).
	
?- main().
