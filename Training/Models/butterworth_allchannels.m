%% run this section first
 %then run any other two sections 
[file,path]  = uigetfile('*.mat'); % Navigate to 'datasets' folder and select any dataset
                                   % like 'rawforceEmgdata90deg_22_esfand_400.mat'
data = struct2cell(load(fullfile(path,file)));
data = data{1};

rectEMG = abs(data(:,1:8));
selectedChannels = [rectEMG(:,5) rectEMG(:,1) rectEMG(:,8)]' ; %use onlyplotandsee.m to choose which channels to input to network
close all
fc = 3;%hz cut off frequency
fs = 178;%sampling frequency
order = 3;
[b,a] = butter(order,fc/(fs/2));
%% filter then range
%This is the method used in project
datafilt(1,:) = filter(b,a,selectedChannels(1,:));
datafilt(2,:) = filter(b,a,selectedChannels(2,:));
datafilt(3,:) = filter(b,a,selectedChannels(3,:));
datafiltoutput = filter(b,a,data(:,11));
plot(datafilt(1,:));


[buttinputs,maxminInput] = mapminmax(datafilt,-1,1);
[butttorques, maxminOutput] = mapminmax( datafiltoutput',-1,1);%force along Z axis is 11th column of data 
figure
plot(buttinputs(1,:));
%% first range data then filter
%  [dataRanged,maxminInput] = mapminmax(selectedChannels,-1,1);
%  [dataOut, maxminOutput] = mapminmax( data(:,11)',-1,1);%force along Z axis is 11th column of data 
% 
%  
% % maxminInput.xmax
% % maxminInput.xmin
% % 
% % maxminOutput.xmax
% % maxminOutput.xmin
% plot(dataRanged(2,:));
% figure
% buttinputs(1,:) = filter(b,a,dataRanged(1,:));
% buttinputs(2,:) = filter(b,a,dataRanged(2,:));
% buttinputs(3,:) = filter(b,a,dataRanged(3,:));
butttorques = filter(b,a,dataOut);

plot(buttinputs(2,:));
%figure
%plot(butttorques);

% plot(dataRanged(:,1));
%  plot(data(:,9));
%  plot(datafilt4(1+7:end));
%  plot(datafilt4(1:end));
%  legend('orig','filtered8','0');
%buttinputs = [datafilt(:,8) datafilt(:,1:7)]' ;
%buttinputs = [datafilt(1:15000,3) datafilt(1:15000,7) datafilt(1:15000,8)]' ;
%buttinputs = [datafilt(:,3) datafilt(:,1) datafilt(:,8)]' ;
%buttinputs = datafilt;
% mx = max(buttinputs,[],2)
% mn = min(buttinputs,[],2)
%butttorques =  (datafilt4);

format long
a
b