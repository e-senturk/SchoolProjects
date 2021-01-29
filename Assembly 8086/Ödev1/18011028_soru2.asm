myss	SEGMENT	PARA	STACK	'yigin'
		DW	20	DUP(?)
myss	ENDS

myds	SEGMENT	PARA	'stack'
	n		DW	(?)	
	temp	DW	(?)
	string	DB	5 DUP(?)
	a		DW  (0)
	b		DW	(?)
	c		DW	(?)
	d_in	DW	10 DUP(?)
	dizi 	DW	100 DUP(?)
	eleman	DB	'Lutfen Dizinin eleman sayisini giriniz.',13,10,'$'
	exp1	DB	'Dizinin elemanlarini giriniz.',13,10,'$'
	err		DB	'Verilen dizide ucgen olusturabilecek eleman yok !',13,10,'$'
	err2	DB	13,10,'Lutfen 0 ila 9 arasinda rakamlar giriniz!',13,10,'$'
	err3	DB	'Sayi limit asimi! Tekrar giris yapin',13,10,'$'
	min		DW	0FFFFh
	ran_min DW  (?)
	ran_max DW	(?)
myds	ENDS



GETINPUT MACRO
	LOCAL M0, M1, M2, M3, M4, M5, M6, M7, M8, M9
		MOV BL,10
M0:		LEA SI,d_in
		XOR CX,CX
		MOV temp,0
M1:		MOV AH,1
		INT 21h
		CMP CX,0
		JNE M2
		CMP AL,0Dh
		JE	M0
M2:		CMP AL,0Dh
		JE M4		
		CMP AL,30h
		JB	M3
		CMP AL,39h
		JA M3	
		SUB AL,30h
		MOV [SI],AL
		INC SI
		INC CX
		JMP M1		
M3:		MOV AH,9
		LEA DX,err2
		INT 21h		
		JMP M0	
M4:		LEA SI,d_in
M5:		MOV AL,[SI]
		XOR AH,AH
		PUSH CX
	M6:		CMP CX,1
			JBE M7
			MUL BL
			DEC CX
			JMP M6
	M7:	POP CX
		ADD temp,AX
		INC SI
		LOOP M5
		MOV AX,temp
		CMP AX,ran_min
		JL	M8
		CMP AX,ran_max
		JLE	M9
	M8:	MOV AH,9	
		LEA DX,err3
		INT 21h	
		JMP M0
	M9:	
ENDM

WORDTOS MACRO sayi,string
	LOCAL W0, W1
		XOR CX,CX
		LEA DI,string
		ADD DI,4
		MOV AX,sayi
		MOV BL,10
W0:		DIV BL
		INC CX
		ADD AH,30h
		MOV BYTE PTR[DI],AH
		DEC DI
		XOR AH,AH
		CMP AX,0
		JNE	W0
		INC DI
		MOV AH,2
W1:		MOV DL,BYTE PTR[DI]
		INC DI
		INT 21h
		LOOP W1
ENDM	

mycs	SEGMENT	PARA	'code'
		ASSUME SS:myss,DS:myds,CS:mycs
UCGEN	PROC FAR
		PUSH DS
		XOR AX,AX
		PUSH AX
		MOV AX,myds
		MOV DS,AX
		MOV AH,9	
		LEA DX,eleman
		INT 21h
		MOV ran_min,1
		MOV ran_max,100
		GETINPUT
		MOV AX,temp
		MOV n,AX
		MOV AH,9	
		LEA DX,exp1
		INT 21h	
		MOV ran_min,0
		MOV ran_max,1000
		MOV CX,n
		LEA DI,dizi
L0:		PUSH CX
		GETINPUT
		POP CX
		MOV AX,temp
		MOV WORD PTR [DI],AX
		INC DI
		INC DI
		LOOP L0
		LEA SI,dizi
		MOV CX,n
		CMP CX,0
		JE L7		
L1:		MOV DI,SI
		PUSH CX
		DEC CX
		CMP CX,0
		JE L6
		L2:	INC DI
			INC DI
			MOV BX,DI
			PUSH CX
			DEC CX
			CMP CX,0
			JE L5
			L3:	INC BX
				INC BX
				MOV AX,WORD PTR [SI]
				ADD AX,WORD PTR [DI]
				CMP AX,WORD PTR [BX]
				JBE L4
				MOV AX,WORD PTR [SI]
				ADD AX,WORD PTR [BX]
				CMP AX,WORD PTR [DI]
				JBE L4
				MOV AX,WORD PTR [DI]
				ADD AX,WORD PTR [BX]
				CMP AX,WORD PTR [SI]
				JBE L4
				ADD AX,WORD PTR [SI]
				CMP AX,min
				JAE L4
				MOV min,AX
				MOV AX,WORD PTR [SI]
				MOV a,AX
				MOV AX,WORD PTR [DI]
				MOV b,AX
				MOV AX,WORD PTR [BX]
				MOV c,AX
			L4:	LOOP L3
		L5:	POP CX
			LOOP L2	
	L6:	INC SI
		INC SI
		POP CX	
		LOOP L1
L7:     MOV AX,a
		CMP AX,0
		JNE L8
		MOV AH,9
		LEA DX,err
		INT 21h
		JMP L9	
L8:		MOV AH,2
		MOV DL,'('
		INT 21h
		WORDTOS a,string
		MOV DL,','
		INT 21h
		WORDTOS b,string
		MOV DL,','
		INT 21h
		WORDTOS c,string
		MOV DL,')'
		INT 21h
L9:		RETF
UCGEN  	ENDP
mycs	ENDS
END		UCGEN