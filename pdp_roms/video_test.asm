.SECTION .CODE
.GLOBAL MAIN

MAIN:
    CLR R0
    CLR R1
    CLR R2
    CLR R3
    CLR R4
    MOV 01114, R3
    MOV 0671, R4
    MOV R0, 0176300(R0)
    LOOP:
        MOV R1, 0176304(R0)
        MOV R2, 0176306(R0)
        MOV R1, R5
        ADD R2, R5
        ASR R5
        BCS LIGHT_ON
        MOV 010000, 0176302(R0)
        BR LOOP_INC
    LIGHT_ON:
        MOV 010001, 0176302(R0)
    LOOP_INC:
    	INC R1
    	CMP R3, R1
    	BEQ Y_INC
    	BR LOOP
    Y_INC:
    	CLR R1
    	INC R2
    	CMP R4, R2
    	BEQ END
    	BR LOOP
    END:
    	HALT