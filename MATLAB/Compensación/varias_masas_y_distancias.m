%% Controlador por adelanto de fase
%Constantes
s = tf('s');
Kin = 0.32;
H = 53.33/1000;
R = 0.2;
k=1.76715e-5;

% Caso m=4kg y=2 mm
m = 4;                 %masa que levanta el electroimán
y0=2/1000;              %Distancia de separacion 
L = 22.64/1000;         %Valor de Inductancia para y0

Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );

Gtotal4_2 = Gplanta * GiL * Hestim;

% Caso m=4kg y=5 mm
m = 4;                 %masa que levanta el electroimán
y0=5/1000;              %Distancia de separacion 
L = 14.9/1000;         %Valor de Inductancia para y0

Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );
Gtotal4_5 = Gplanta * GiL * Hestim;

% Caso m=30kg y=2 mm
m = 30;                 %masa que levanta el electroimán
y0=2/1000;              %Distancia de separacion 
L = 22.64/1000;         %Valor de Inductancia para y0
Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );
Gtotal30_2 = Gplanta * GiL * Hestim;


% Caso m=30kg y=5 mm
m = 30;                 %masa que levanta el electroimán
y0=5/1000;              %Distancia de separacion 
L = 14.9/1000;         %Valor de Inductancia para y0

Gplanta = -(2/y0)*(sqrt(k*9.8/m))/(s^2-(2*9.8/y0));
GiL = (Kin/H) / (1 + s/(R/L));                  
Hestim = 259.6 / ((1 + s/1e3) * (1 + s/60e3)^2 );
Gtotal30_5 = Gplanta * GiL * Hestim;


Gtotal = [Gtotal4_2 Gtotal4_5 Gtotal30_2 Gtotal30_5];

% Agregando el compensador por adelanto de fase diseñado
Kc = 10;
Gc = Kc * (20.346 * (s + 47.4)/(s + 965.4))^2;

% Bode de la planta con compensador
Gcompensado = Gtotal * Gc;
bode(Gcompensado);


%% Transferencia de lazo cerrado
tlc = zpk(minreal(feedback(Gc * Gplanta * GiL, -Hestim)));
dcgain(tlc);

% Respuesta al escalon
step(tlc);


