%% Controlador por adelanto de fase
%%Variables, cambiar segun se desee
m=1; %%kilogramos
y=1; %%ya esta en mm;

%Constantes
s = tf('s');
Kin = 0.32;
H = 53.33/1000;
R = 0.2;
k=1.76715e-5;
[y0,L] = distandinduc(y);


%% Calulo de las transferencias de cada bloque
Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );

Gtotal = Gplanta * GiL * Hestim;

%% Ganancia de Gtotal
dcgain(Gtotal);

%% polos de Gtotal
pole(Gtotal);

%% Bode y Nyquist de Gtotal
figure (1);
grid on;
bode(Gtotal);
figure (2);
nyquist(Gtotal);

%% Lugar de raices de Gtotal
figure(3);
rlocus(Gtotal);

%% Agregando el compensador por adelanto de fase diseñado
Kc = 10;
Gc = Kc * (20.346 * (s + 47.4)/(s + 965.4))^2;

%% Bode de la planta con compensador
Gcompensado = Gtotal * Gc;
bode(Gcompensado);

%% Nyquist de la planta con compensador 
nyquist(Gcompensado);

%% Transferencia de lazo cerrado
tlc = zpk(minreal(feedback(Gc * Gplanta * GiL, -Hestim)));
dcgain(tlc);

%% Respuesta al escalon
step(tlc);

%% error
errorEscalon=1/(1+dcgain(Gc * Gplanta * GiL * Hestim));

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
            case 1
            y0=y/1000;
            L=0.0149;
        otherwise
        disp('y value not valid')
    end    
end