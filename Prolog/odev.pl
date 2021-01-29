% importing database
:-include('isimler.pl').
:-include('fiiller.pl').

% connection printing function
printconnection(Value1,Connection,Value2):-
	write('iliski('),write(Value1),
	write(', '),write(Connection),
	write(', '),write(Value2),write('), ').

% printing result and result size.
print_list([_],[],N):- write('\tlength: '),write(N),write('.\n\n').
print_list([Head,Middle|Tail],[H|T],N):-
	printconnection(Head,H,Middle),
	K is N+1,
	print_list([Middle|Tail],T,K).

% checking is connection in array or not.
find(Connection,[Connection|_]).
find(Connection,[Head|Tail]) :-
	Connection \== Head,
	find(Connection,Tail).
	
% search mode function
check(Connection,Array,Mode) :- Mode = 1 -> find(Connection,Array).
check(Connection,Array,Mode) :- Mode = 2 -> \+find(Connection,Array).


% checks iliski to both sides
connection(Value1,Connecter,Value2):-
		(iliski(Value1,Connecter,Value2);iliski(Value2,Connecter,Value1)).


% bfs algorithm
  
% looping until reach the destintion if you fin.
% called necessary printing functions.
% the first line is already printed. thats why we start with array size 1. 
bfs(Destination,[n(Destination,[PathHead|PathTail],[ElemHead|ElemTail])|_],_,_,_) :- 
	printconnection(Destination,PathHead,ElemHead),print_list([ElemHead|ElemTail],PathTail,1).
	

/*
 variables:
	- Destination : element which we want to reach.
	- n(Target,Path,Elements) : 
			Target: representing target node,
			Path : storing path to reaching target node, 
			Elements : storing elements in the path.
	- PrevCollection: keeping elements in previous loop.
	- Acquired: keeping elements we already checked without any duplication
	- Array: connection array which can be ignored by mode or only they can allowed
	- Mode: mode selector (use only array values or not use array values use others)
	algorithm:
	- in first line we find all iliski connections and controlling them with check method,
	we added each unique element in Acquired array, then findall function stores all results
	in NewCollection array. append function merges previous elements with current elements and
	generates total collection array, and total collection send to next iteration as a previous
	elements. target added into acquired array. the function looped until any of the iteation 
	reaches target value.
*/
	
bfs(Destination,[n(Target,Path,Elements)|PrevCollection],Acquired,Array,Mode) :- 
	findall(n(NewElement,[Connection|Path],[Target|Elements]),
	(connection(Target,Connection,NewElement),check(Connection,Array,Mode), \+ member(NewElement,Acquired)),
	NewCollection),
	append(PrevCollection,NewCollection,TotalCollection),
	bfs(Destination,TotalCollection,[Target|Acquired],Array,Mode). 
	
% calculating function calling from main
calculate(Start,Destination,Array,Mode) :-
	bfs(Destination,[n(Start,[],[])],[],Array,Mode).

% function created for getting input from user.
main():-
	write('..Welcome to shortest path program in prolog..\n'),
	write('Please enter the first word.\n'),
	read(Start),
	write('Please enter the second word.\n'),
	read(End),
	write('Please enter the array.\n'),
	read(Array),
	write('Please enter search method\n\t1- Inside of the Array\n\t2- Outside of the Array\n'),
	read(Mode),
	calculate(Start,End,Array,Mode),
	write('Press 1 to search again\n'),
	read(K),
	( K=1 -> main();  write('Program terminated\n')).
	
?- main().
