.SECTION .DATA
    COLOR0: .WORD 0000000
    COLOR1: .WORD 0000077
    COLOR2: .WORD 0001700
    COLOR3: .WORD 0176000
    COLOR4: .WORD 0001777
    COLOR5: .WORD 0177700
    COLOR6: .WORD 0176077
    COLOR7: .WORD 0000052
    COLOR8: .WORD 0001200
    COLOR9: .WORD 0124000
    COLOR10: .WORD 0000025
    COLOR11: .WORD 0000500
    COLOR12: .WORD 0052000
    COLOR13: .WORD 0000525
    COLOR14: .WORD 0052530
    COLOR15: .WORD 0177777

.SECTION .CODE
.GLOBAL MAIN

MAIN:
    CLR R0
    CLR R1
    CLR R2
    MOV 07, (0176300)
    MOV COLOR0, R0
    MOV 0176200, R1
    LOOP_PALETTE:
        MOV (R0)+, (R1)+
        INC R2
        CMP R2, 020
        BNE LOOP_PALETTE

    CLR R0
    CLR R1
    CLR R4
    CLR R5
    MOV 0111, R2
    MOV 066, R3
    CLR 01000(R0)
    CLR 01002(R0)
    CLR 01004(R0)
    CLR 01006(R0)
    MOV 0010000, (01010)

    LOOP_ST:
        MOV (01006), R4
        ADD R1, R4
        MOV R4, (0176306)

    LOOP_QUAD:
        MOV (01004), R4
        ADD R0, R4
        MOV R4, (0176304)
        MOV (01010), (0176302)

        INC R0
        CMP R0, R2
        BNE LOOP_QUAD
        CLR R0
        INC R1
        MOV (01006), R4
        ADD R1, R4
        MOV R4, (0176306)
        CMP R1, R3
        BNE LOOP_QUAD
        CLR R1

        INC (01010)
        INC (01000)
        ADD R2, (01004)
        CMP (01000), 04
        BNE LOOP_ST
        CLR (01000)
        CLR (01004)
        INC (01002)
        ADD R3, (01006)
        CMP (01002), 04
        BNE LOOP_ST
        HALT