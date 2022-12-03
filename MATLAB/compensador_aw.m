s = tf('s');
cx=1e-6;
rx=1100;
ra=200000;
rb=200000;
r1=21500;
a_w=1778279/((s/(2*3.14*20))+1);
H=((s*cx*rx+1)/(s*cx*(rx+r1)+1))*(rb/(ra+rb));
gmen=rb/(ra+rb);

%bode(a_w,1/H)
