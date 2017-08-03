;=====================================================================
; File Name : 2440slib.s
; Function  : S3C2440  (Assembly)
; Date      : March 09, 2002
; Revision	: Programming start (February 26,2002) -> SOP
; Revision	: 03.11.2003 ver 0.0	Attatched for 2440
;=====================================================================
;Interrupt, FIQ/IRQ disable
NOINT  EQU 0xc0    ; 1100 0000

;Check if tasm.exe(armasm -16 ...@ADS 1.0) is used.
   GBLL    THUMBCODE
   [ {CONFIG} = 16
THUMBCODE SETL  {TRUE}
	 CODE32
   |
THUMBCODE SETL  {FALSE}
   ]

   MACRO
     MOV_PC_LR
     [ THUMBCODE
       bx lr
     |
       mov pc,lr
     ]
   MEND

   AREA |C$$code|, CODE, READONLY
;====================================
; MMU Cache/TLB/etc on/off functions
;====================================
R1_I	EQU	(1<<12)
R1_C	EQU	(1<<2)
R1_A	EQU	(1<<1)
R1_M    EQU	(1)
R1_iA	EQU	(1<<31)
R1_nF   EQU	(1<<30)


;void MMU_SetFastBusMode(void)
; FCLK:HCLK= 1:1
  EXPORT MMU_SetFastBusMode
MMU_SetFastBusMode
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_iA:OR:R1_nF
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

;void MMU_SetAsyncBusMode(void)
; FCLK:HCLK= 1:2
   EXPORT MMU_SetAsyncBusMode
MMU_SetAsyncBusMode
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_nF:OR:R1_iA
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
   END
