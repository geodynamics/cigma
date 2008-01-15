      SUBROUTINE MEXFUNCTION(NLHS, PLHS, NRHS, PRHS)
 
C     USE MSFLIB

C     These are arrays of pointers. Size needs to match size of pointers.
C     Use INTEGER*8 on 64-bit machines, INTEGER*4 on 32-bit machines.
      INTEGER*4  PLHS(*), PRHS(*)

      INTEGER NLHS, NRHS

C     These are functions that return pointers. Size needs to match size
C     of pointers.  Use INTEGER*8 on 64-bit machines, INTEGER*4 on
C     32-bit machines.
      INTEGER*4  MXCREATEFULL, MXGETPR

      INTEGER MXGETM, MXGETN
C
C KEEP THE ABOVE SUBROUTINE, ARGUMENT, AND FUNCTION DECLARATIONS FOR USE
C IN ALL YOUR FORTRAN MEX FILES.
C---------------------------------------------------------------------
C

C     These are actually pointers. Size needs to match size of pointers.
C     Use INTEGER*8 on 64-bit machines, INTEGER*4 on 32-bit machines.
      INTEGER*4  pModel, pStat, pMu, pNU
      INTEGER*4 UOUT, DOUT, SOUT, FLAGOUT, FLAGOUT2

      INTEGER M, N, NSTAT, NMOD, I, J
      REAL*8 MODEL(10), STAT(3), MU, LAMBDA, NU
      REAL*8 ALPHA, STRIKE, DIP, DEG2RAD, SD, CD, SS, CS, DEPTH
      REAL*8 X,Y,Z,DISL1,DISL2,DISL3,AL1,AL2,AW1,AW2
      REAL*8 U(3),D(9),S(6),UX,UY,UZ,UXX,UYX,UZX,UXY,UYY,UZY,UXZ,UYZ,UZZ
      REAL*8 UXT,UYT,UZT,UXXT,UYXT,UZXT,UXYT,UYYT,UZYT,UXZT,UYZT,UZZT
      REAL*8 FLAG
      REAL*8 A, B, C, AAA, BBB, CCC, DDD, EEE, FFF, GGG, HHH, III
      REAL*8 FLAG2(5000,5000)
     
C For Windows only!
C This resets the floating point exception to allow divide by zero,
C overflow and invalid numbers. 
C
C      INTEGER(2) CONTROL
C      CALL GETCONTROLFPQQ(CONTROL)
C      CONTROL = CONTROL .OR. FPCW$ZERODIVIDE
C      CONTROL = CONTROL .OR. FPCW$INVALID
C      CONTROL = CONTROL .OR. FPCW$OVERFLOW
C      CALL SETCONTROLFPQQ(CONTROL)

C
C CHECK FOR PROPER NUMBER OF ARGUMENTS
C
      IF (NRHS .NE. 4) THEN
       CALL MEXERRMSGTXT('Usage: [U,D,S,flag]=disloc3d(m,x,mu,nu)')
      ENDIF
C
C CHECK THE ARGUMENTS
C
      M = MXGETM(PRHS(1))
      NMOD = MXGETN(PRHS(1))
      IF (M .NE. 10) THEN
       CALL MEXERRMSGTXT('m must be 10x1 model vector')
      ENDIF

      M = MXGETM(PRHS(2))
      NSTAT = MXGETN(PRHS(2))
      IF (M .NE. 3) THEN
       CALL MEXERRMSGTXT('x must be 3xn.')
      ENDIF

      M = MXGETM(PRHS(3))
      N = MXGETN(PRHS(3))
      IF ((M .NE. 1) .OR. (N .NE. 1)) THEN
       CALL MEXERRMSGTXT('mu must be a scalar.')
      ENDIF

      M = MXGETM(PRHS(4))
      N = MXGETN(PRHS(4))
      IF ((M .NE. 1) .OR. (N .NE. 1)) THEN
       CALL MEXERRMSGTXT('nu must be a scalar.')
      ENDIF

C
C CREATE A MATRIX FOR RETURN ARGUMENT
C
      PLHS(1) = MXCREATEFULL(3,NSTAT,0)
      PLHS(2) = MXCREATEFULL(9,NSTAT,0)
      PLHS(3) = MXCREATEFULL(6,NSTAT,0)
      PLHS(4) = MXCREATEFULL(NSTAT,1,0)
      PLHS(5) = MXCREATEFULL(NMOD,NSTAT,0)
C
C ASSIGN POINTERS TO THE VARIOUS PARAMETERS
C
      UOUT = MXGETPR(PLHS(1))
      DOUT = MXGETPR(PLHS(2))
      SOUT = MXGETPR(PLHS(3))
      FLAGOUT = MXGETPR(PLHS(4))
      FLAGOUT2= MXGETPR(PLHS(5))
      
C
      pMODEL = MXGETPR(PRHS(1))
      pSTAT = MXGETPR(PRHS(2))
      pMU = MXGETPR(PRHS(3))
      pNU = MXGETPR(PRHS(4))
C
C COPY RIGHT HAND ARGUMENTS TO LOCAL ARRAYS OR VARIABLES
C      
      CALL MXCOPYPTRTOREAL8(pMU, MU, 1)
      CALL MXCOPYPTRTOREAL8(pNU, NU, 1)
      LAMBDA = 2*MU*NU/(1-2*NU)

C LOOP OVER STATIONS

      DO 111 I=1,NSTAT 

      CALL MXCOPYPTRTOREAL8(pSTAT+(I-1)*24, STAT, 3)

C GENERATE WARNINGS FOR POSITIVE DEPTHS

      IF (STAT(3) .GT. 0) THEN
	CALL MEXEVALSTRING("warning('Positive depth given.')")
      ELSE

C LOOP OVER MODELS

      DO 222 J=1,NMOD

      CALL MXCOPYPTRTOREAL8(pMODEL+(J-1)*80, MODEL, 10)
     
      DEG2RAD=3.14159265358979/180
      ALPHA = (LAMBDA+MU)/(LAMBDA+2*MU)
      STRIKE = (MODEL(5)-90)*DEG2RAD
      CS = DCOS(STRIKE)
      SS = DSIN(STRIKE)
      DIP = MODEL(4)
      CD = DCOS(DIP*DEG2RAD)
      SD = DSIN(DIP*DEG2RAD)
      DISL1=MODEL(8)
      DISL2=MODEL(9)
      DISL3=MODEL(10)
      DEPTH=MODEL(3)-0.5*MODEL(2)*SD
      AL1=MODEL(1)/2
      AL2=AL1
      AW1=MODEL(2)/2
      AW2=AW1
      X=CS*(-MODEL(6) + STAT(1)) - SS*(-MODEL(7) + STAT(2))
      Y=-0.5*CD*MODEL(2) + SS*(-MODEL(6) + STAT(1)) + CS*(-MODEL(7) +
     -  STAT(2))
      Z=STAT(3)

C GENERATE WARNINGS FOR UNPHYSICAL MODELS

      IF ((MODEL(3)-SD*MODEL(2) .LT. 0) .OR.
     -    (MODEL(1) .LE. 0) .OR.
     -    (MODEL(2) .LE. 0) .OR.
     -    (MODEL(3) .LT. 0)) THEN
          CALL MEXEVALSTRING("warning('Unphysical model.')")
      ELSE

      CALL DC3D(ALPHA,X,Y,Z,DEPTH,DIP,
     *          AL1,AL2,AW1,AW2,DISL1,DISL2,DISL3,
     *          UX,UY,UZ,UXX,UYX,UZX,UXY,UYY,UZY,UXZ,UYZ,UZZ,FLAG)
 
C fill flag2 - the rows contain flags for each station, one row per model/fault
      
      
	FLAG2(J,I) = FLAG + 5 + 5*FLAG
  
     

C ROTATE THEN ADD
      A = CS*UX +  SS*UY
	B = -SS*UX + CS*UY
	C = UZ
	AAA = CS**2*UXX + CS*SS*(UXY + UYX) + SS**2*UYY
	BBB = CS**2*UXY - SS**2*UYX + CS*SS*(-UXX + UYY)
	CCC = CS*UXZ + SS*UYZ
	DDD = -(SS*(CS*UXX + SS*UXY)) + CS*(CS*UYX + SS*UYY)
	EEE = SS**2*UXX - CS*SS*(UXY + UYX) + CS**2*UYY
	FFF = -(SS*UXZ) + CS*UYZ
      GGG = CS*UZX + SS*UZY
	HHH = -(SS*UZX) + CS*UZY
	III = UZZ
  

     
     

      UXT=UXT + A
      UYT=UYT + B
      UZT=UZT + C

      UXXT=UXXT + AAA
      UXYT=UXYT + BBB
      UXZT=UXZT + CCC
      UYXT=UYXT + DDD
      UYYT=UYYT + EEE
      UYZT=UYZT + FFF
      UZXT=UZXT + GGG
      UZYT=UZYT + HHH
      UZZT=UZZT + III
      ENDIF

  222 CONTINUE  

C ASSIGN OUTPUTS

      U(1)=UXT
      U(2)=UYT
      U(3)=UZT

      D(1)=UXXT
      D(2)=UXYT
      D(3)=UXZT
      D(4)=UYXT
      D(5)=UYYT
      D(6)=UYZT
      D(7)=UZXT
      D(8)=UZYT
      D(9)=UZZT
      
      

C CALCULATE STRESSES

      THETA=D(1)+D(5)+D(9)

      S(1)=LAMBDA*THETA+2*MU*D(1)
      S(2)=MU*(D(2)+D(4))
      S(3)=MU*(D(3)+D(7))
      S(4)=LAMBDA*THETA+2*MU*D(5)
      S(5)=MU*(D(6)+D(8))
      S(6)=LAMBDA*THETA+2*MU*D(9)

C COPY TO MATLAB

      CALL MXCOPYREAL8TOPTR(U, UOUT+(I-1)*24, 3)
      CALL MXCOPYREAL8TOPTR(D, DOUT+(I-1)*72, 9)
      CALL MXCOPYREAL8TOPTR(S, SOUT+(I-1)*48, 6)
      CALL MXCOPYREAL8TOPTR(FLAG, FLAGOUT+(I-1)*8, 1)

      UXT=0
      UYT=0
      UZT=0

      UXXT=0
      UXYT=0
      UXZT=0
      UYXT=0
      UYYT=0
      UYZT=0
      UZXT=0
      UZYT=0
      UZZT=0
      ENDIF
  111 CONTINUE  
      
      CALL MXCOPYREAL8TOPTR(FLAG2, FLAGOUT2, NSTAT*NMOD)
C
      RETURN
      END
