data = load('rawforceEmgdata90deg_22_esfand_400');
data = data.rawforceEmgdata90deg_22_esfand_400;
rectEMG = abs(data(:,1:8));
close all


%tiledlayout(8,1);

%3+ 8 7- for final lstm1
%3+ 7- 8- for rawforceEmgdata90degFinal_LSTM2_1aban400
%3+ 7- 8- for rawforceEmgdata90degFinalLSTM3
%3+ 1- 8- rawforceEmgdata90deg_11aban400
%3+ 1- 8- rawforceEmgdata90deg_3_azar_400
%4+ 1- 8- rawforceEmgdata90deg_17_esfanf_400
%4+ 1- 8- rawforceEmgdata90deg_22_esfand_400
%11 is for forcez
%IMPORTANT**
%

for ii = 1:8
    
    figure
    hold on
    plot(data(:,ii)); 
    plot(data(:,11));
    title(['EMG',num2str(ii)])
end
figure
for ii = 1:8
    subplot(9,1,ii);
  % 
    hold on
    plot(data(1:4000,ii),'-b');% view(0,0); 
    
    %if ii>1
        set(gca,'XTick',[]);
     %   set(gca,'YTick',[]);
    %end
    title(['EMG',num2str(ii)])
end
subplot(9,1,9);
%for better visualization  multiply torques by 4 no
    plot(data(1:4000,11),'-b','LineWidth',1);
    title('Elbow ForceZ');
  %  print('FillPageFigure','-dpdf','-fillpage')
%title('EMG3 vs torque in emg_elbowTorque60degAban98'); 

