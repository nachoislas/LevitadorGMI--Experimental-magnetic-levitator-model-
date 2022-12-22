%CONSTANTES
s = tf('s');

%Variables
m=30; %%kilogramos
y=4; %%ya esta en mm;

Kin = 0.32;
H = 53.33/1000;
R = 0.2;
k=1.76715e-5;
[y0,L] = distandinduc(y);

%Etapas
%Compensador
F=1.015
Gextcircuito=-50/((s/0.1)+1)

%FUNCIONES

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
