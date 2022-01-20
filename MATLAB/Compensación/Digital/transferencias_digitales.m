s = tf('s');
Gil = 6 / ( 1 + s/13.42);               %del controlador de corriente
Gp30 = -1.2019 / (s^2 - 4905);          %para m = 30 kg
Gp2 = Gp30*sqrt(30/2);            %para m = 2 kg

Gtotal30 = zpk(Gp30*Gil)
Gtotal2 = zpk(Gp2*Gil)

%% Pasar a z por invarianza al impulso

fs = 50e3;
Ts = 1/fs;

Gtotal30z = zpk(c2d(Gtotal30, Ts, 'zoh'))
Gtotal2z = zpk(c2d(Gtotal2, Ts, 'zoh'))

%% pasar a w por bilineal

Gtotal30w = zpk(d2c(Gtotal30z, 'tustin'))
Gtotal2w = zpk(d2c(Gtotal2z, 'tustin'))
