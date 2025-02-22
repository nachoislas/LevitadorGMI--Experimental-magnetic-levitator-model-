%% Controlador por adelanto de fase
%%Variables, cambiar segun se desee
m=30; %%kilogramos
y=5; %%ya esta en mm;

%Constantes
s = tf('s');
Kin = 0.32;
H = 53.33/1000;
R = 0.2;
k=1.76715e-5;
%Descomentar y comentar para los casos deseados
%% Caso m=1kg y=2 mm
m = 4;                 %masa que levanta el electroimán
y0=2/1000;              %Distancia de separacion 
L = 22.64/1000;         %Valor de Inductancia para y0

%% Caso m=15kg y=2 mm
m = 15;                 %masa que levanta el electroimán
y0=2/1000;              %Distancia de separacion 
L = 22.64/1000;         %Valor de Inductancia para y0

%% Caso m=30kg y=2 mm
m = 30;                 %masa que levanta el electroimán
y0=2/1000;              %Distancia de separacion 
L = 22.64/1000;         %Valor de Inductancia para y0

%% Caso m=15kg y=5 mm
m = 15;                 %masa que levanta el electroimán
y0=5/1000;              %Distancia de separacion 
L = 14.9/1000;         %Valor de Inductancia para y0

%% Caso m=30kg y=5 mm
m = 4;                 %masa que levanta el electroimán
y0=5/1000;              %Distancia de separacion 
L = 14.9/1000;         %Valor de Inductancia para y0

%% Caso m=30kg y=4 mm
%(Este es el caso que se simula en el documento de Compensacion analogica)
m = 30;                 %masa que levanta el electroimán
y0=4/1000;              %Distancia de separacion 
L = 16.44/1000;         %Valor de Inductancia para y0

%% Calulo de las transferencias de cada bloque
Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );

Gtotal = Gplanta * GiL;

%% Agregando el compensador por adelanto de fase diseñado
Kc = 10;
Gc = Kc * (20.346 * (s + 47.4)/(s + 965.4))^2;

%% Bode de la planta con compensador
Gcompensado = Gtotal * Gc;

tlc = minreal(feedback(Gcompensado, Hestim, 1));

%% Integrador
Gint = 1 * 1/s;

rlocus(-tlc*Hestim*Gint)

%%
Gint = 5 / s;

tlctotal = -dcgain(Hestim) * feedback(Gint*tlc, Hestim, 1);
step(tlctotal)