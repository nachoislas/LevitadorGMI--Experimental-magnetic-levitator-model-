s = tf('s');
p1 =2 * pi * 20;           %primer polo del operacional en rads
%p2 = 2* pi * 60e6;          %segundo polo del operacional en rads
Ao = 10^(94/20)
Aw = Ao * 1 / ((1+s/p1)) ;   %transferencia del operacional


R1 = 25e3;                      
C = 1e-6;                   %R1 y C para tau = 30 ms
R2 = 0;                 %por ahora lo considero 0

H = (1 + s*C*R2) / (1 + s*C*(R1+R2));   


R2 = 10; % calculada para polo en 16 kHz

Hcomp = (1 + s*C*R2) / (1 + s*C*(R1+R2));   

bode(Aw, 1/Hcomp)


%%

GH = Aw*Hcomp;

margin(GH)

%%

step(GH)

%%
tlc = Aw/ ( 1+ Aw * Hcomp);

bode(tlc)

%%

step(tlc)

%%

Gmenos = -s*C*R1/(1+s*C*(R2+R1));
tlcmenos = minreal(Gmenos*tlc) ;

zpk(tlcmenos)


bode(tlcmenos)

%%
step(tlcmenos)