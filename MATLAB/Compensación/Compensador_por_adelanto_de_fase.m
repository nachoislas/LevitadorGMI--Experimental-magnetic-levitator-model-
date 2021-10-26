%% Controlador po adelanto de fase
s = tf('s');
m = 10;                                             %masa que levanta el electroimán
L = 22.64/1000;
Kin = 0.32;
H = 53.33/1000;
R = 0.2;
y0=2/1000;
k=1.76715e-5;
Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
%Gplanta = (1342e-6/sqrt(m)) / (1 - (s/70)^2);
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );

Gtotal = Gplanta * GiL * Hestim;
%% Ganancia de Gtotal
dcgain(Gtotal)
%% polos de Gtotal
pole(Gtotal)
%% Bode y Nyquist de Gtotal
figure (1)
grid on
bode(Gtotal)
figure (2)
nyquist(Gtotal)
%% Lugar de raices de Gtotal
figure(3)
rlocus(Gtotal)

%% Agregando el compensador por adelanto de fase diseñado
Kc = 20;
Gc = Kc * (20.346 * (s + 47.4)/(s + 965.4))^2;

 

%% Bode de la planta con compensador
Gcompensado = Gtotal * Gc;
bode(Gcompensado)
%% Nyquist de la planta con compensador 
nyquist(Gcompensado)

%% Transferencia de lazo cerrado
tlc = zpk(minreal(feedback(Gc * Gplanta * GiL, -Hestim)))
dcgain(tlc)
%% Respuesta al escalon
step(tlc)
%% error
error=1/(1+dcgain(Gc * Gplanta * GiL * Hestim))
%%step(error)