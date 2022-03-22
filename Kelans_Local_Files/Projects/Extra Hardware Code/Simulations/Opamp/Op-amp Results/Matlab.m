clear
clc

load Opamp.txt

time = Opamp(:,1);
Input = Opamp(:,3);
Output = Opamp(:,2);
    
figure();
plot(time, Input, time, Output)

figure();
Xjw = fft(Input);
Yjw = fft(Output);
H = Yjw ./ Xjw;
plot(abs(H))

ifft()