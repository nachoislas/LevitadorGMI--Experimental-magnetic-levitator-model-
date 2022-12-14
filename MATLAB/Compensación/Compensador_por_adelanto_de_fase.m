%% Controlador por adelanto de fase
s = tf('s');

%%Variables, cambiar segun se desee
m=30; %%kilogramos
y=4; %%ya esta en mm;

%BLOQUE INTEGRADOR
pint=0.1;
Kint=50;

%fltro butter
W0=126457;
alpha=89703;
GfiltroButter=(W0*W0)/(s^2+2*alpha*s+W0^2);
GfiltroButter=1;

%Constantes
Kin = 0.32;
H = 53.33/1000;
R = 0.2;
k=1.76715e-5;
[y0,L] = distandinduc(y);

%% Calulo de las transferencias de cada bloque
Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = (259.6 / ((1 + s/1e3)))*GfiltroButter;

Gtotal = Gplanta * GiL * Hestim;

%% nyquist falopa
%ggg=        4.7692e08*(s+60.74)*(s+35.37)/((s+902.1)*(s+877.2)*(s+528.4)*(s+364.3))
%nyquist(Gtotal*ggg);
%txt = 'Zona 1';
%text(0.15,0,txt,'FontSize',14)
%txt = 'Zona 2';
%text(0.4,0,txt,'FontSize',14)


%% Ganancia de Gtotal
dcgain(Gtotal);

%% polos de Gtotal
pole(Gtotal);

%% Bode y Nyquist de Gtotal
figure (1);
grid on;
bode(Gtotal);
txt = 'Zona 1';
text(100,10,txt,'FontSize',14)
figure (2);
nyquist(Gtotal);
txt = 'Zona 1';
text(0.15,0,txt,'FontSize',14)
txt = 'Zona 2';
text(0.4,0,txt,'FontSize',14)

%% Lugar de raices de Gtotal
figure(3);
rlocus(Gtotal);

%% Agregando el compensador por adelanto de fase diseñado
Kc = 10;
Gc = Kc * (20.346 * (s + 44.3)/(s + 902.14))^2;

%% Bode de la planta con compensador
grid on;
figure(4);
Gcompensado = Gtotal * Gc;
bode(Gcompensado);

%% Nyquist de la planta con compensador 
figure(5);
nyquist(Gcompensado);

%% Transferencia de lazo cerrado
tlc = zpk(minreal(feedback(Gc * Gplanta * GiL, -Hestim)));
dcgain(tlc);

%% Respuesta al escalon
figure(6);
step(tlc);

%% error
errorEscalon=1/(1-dcgain(Gc * Gplanta * GiL * Hestim));

%% Bloque Integrador
integrador=Kint/(1+(s/pint));

figure(7)
rlocus(-tlc*Hestim*integrador)
figure(8)
tlcConIntegrador = feedback(integrador*tlc, Hestim, 1);
errorEscalonintegrador=1/(1-dcgain(integrador*tlc*Hestim));
error_porcentual=dcgain(integrador*tlc)*errorEscalonintegrador;
step(tlcConIntegrador);


%% Transformadas Digitales
Gtotalzpk = zpk(Gplanta*GiL);
fs = 25e3/7;
Ts = 1/fs;
Gtotalz = zpk(c2d(Gtotalzpk, Ts, 'zoh'));
%pasar a w por bilineal
Gtotalw = zpk(d2c(Gtotalz, 'tustin'));

%% Compensador Digital de adelanto atraso
w0=200;
gradosmax=65;
phimax=gradosmax*pi/180;
alpha=(1+sin(phimax))/((1-sin(phimax)));
wc=w0/sqrt(alpha);
wp=sqrt(alpha)*w0;
k=2630;
CdigitalW=(1+s/wc)/(1+s/wp);
CtotalW=k*CdigitalW*CdigitalW;
gcompensado=k*Gtotalw*CdigitalW*CdigitalW;

figure(9)
bode(Gtotalw)
figure(10)
nyquist(Gtotalw)
figure(11)
bode(gcompensado)
figure(12)
nyquist(gcompensado)

%% Transferencia de lazo cerrado Digital
TlcDigitalSinIntegrador = zpk(minreal(feedback(gcompensado, -1)));
dcgain(tlc);
figure(13)
step(TlcDigitalSinIntegrador)


%% Bloque Integrador Digital
KintD=20;
integradorDigital=KintD/(s);

figure(14)
rlocus(-TlcDigitalSinIntegrador*integradorDigital)
figure(15)
tlcConIntegradorDigital = -1 * feedback(TlcDigitalSinIntegrador*integradorDigital, 1, 1);
step(tlcConIntegradorDigital);

%%Paso compensador digital a Z para simulink
CtotalZ = zpk(c2d(CtotalW, Ts,'tustin'));
intZ    = zpk(c2d(integradorDigital, Ts,'tustin'));


%% funciones
function [y0,L] = distandinduc(y)
    switch y
        case 0
            y0=y/1000;
            L=0.07645;
        case 1
            y0=y/1000;
            L=0.03342;
        case 2
            y0=y/1000;
            L=0.02264;
        case 3
            y0=y/1000;
            L=0.0188;
        case 4
            y0=y/1000;
            L=0.01644;
        case 5
            y0=y/1000;
            L=0.0149;
        otherwise
        disp('y value not valid')
    end    
end