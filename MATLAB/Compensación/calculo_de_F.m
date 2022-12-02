Vref = [3.92 4.18 4.44 4.7];
Kp = dcgain(integrador*tlc*Hestim);
F = (1-Kp)/Kp;
erp = Vref*F/(1-Kp);
Vest = erp - Vref*F
Yreal = (Vest-3.4)/259.6