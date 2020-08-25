%%Setup Serial Port
clear all;
delete(instrfindall);
clc;
Tiva=serial("COM3",'BaudRate',115200);      
flushinput(Tiva);
fopen(Tiva); 
%% Animated plot
SpeedPlot=animatedline;                 %Create an animedline object
set(SpeedPlot,'color',' r')               %Display options
axis([0 20000 -350 350])                           
xlabel('Sample')
ylabel('Velocity')
title('Velocity Sample')
for i=1:10000                             %Create loop to display
    flushinput(Tiva);
    SpeedData(i)=fread(Tiva,1,'int16')
    addpoints(SpeedPlot,i,SpeedData(i));     %Draw B_curve
    drawnow
end
