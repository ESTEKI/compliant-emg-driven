[file,path]  = uigetfile('*.mat'); % Navigate to 'datasets' folder and select any dataset
                                   % like 'rawforceEmgdata90deg_22_esfand_400.mat'
data = struct2cell(load(fullfile(path,file)));
data = data{1};

rectEMG = abs(data(:,1:8));
close all
%%

%tiledlayout(8,1);
%4+ 2- 1- for dataset A
%3+ 5- 8- for dataset B
%4+ 7- 8- for Dataset C rabii
%3+ 8 7- for final lstm1
%3+ 7- 8- for rawforceEmgdata90degFinal_LSTM2_1aban400
%3+ 7- 8- for rawforceEmgdata90degFinalLSTM3
%3+ 1- 8- rawforceEmgdata90deg_11aban400
%3+ 1- 8- rawforceEmgdata90deg_3_azar_400
%4+ 1- 8- rawforceEmgdata90deg_17_esfanf_400
%4+ 1- 8- rawforceEmgdata90deg_22_esfand_400
%5+ 1- 8- DatasetE 8shahrivar 1401
%4+ 7- 8- for zoh1 and 2
%1- 8- 5+ for mey1 K
%3+ 5- 6- for mey2 
%3- 6- 7+ for sey1
%1- 2- 4+ sey2



%11 is for forcez
%IMPORTANT**
%
force_col = 9; % force columns in dataset from G to last is 9th
datapoints =3399;% length(data);%3399;
% for ii = 1:8
%     
%     figure
%     hold on
%     plot(data(:,ii)); 
%     plot(data(:,11));
%     title(['EMG',num2str(ii)], 'FontWeight', 'bold', 'FontSize', 14);
% end
figure
for ii = 1:8
    subplot(9,1,ii);
    hold on
    plot(data(1:datapoints,ii),'-b'); 
    
    % Remove XTicks
    set(gca,'XTick',[]); 
    
    % Set constant Y-axis limits for subplots 1 to 8
    ylim([-100 100]);
    
    % Set Y-axis label on the left side
    ylabel(['EMG', num2str(ii)], 'FontWeight', 'bold', 'FontSize', 12);
    
    % Add a secondary Y-axis on the right side
    yyaxis right;
    
    % Set the Y-axis limits for the right side (same as the left)
    ylim([-100 100]);
    
    % Set the Y-axis on the right to display tick labels
    set(gca, 'YColor', 'k'); % Set color of the Y-axis ticks to black
    
    % Hide the tick labels on the left side
    yyaxis left;
    set(gca, 'YTickLabel', []);

end
% Last subplot for the elbow force (no Y-axis limits specified)
subplot(9,1,9);
plot(data(1:datapoints,force_col),'-b','LineWidth',2);

% Set Y-axis label for the last plot on the left side
ylabel('Force', 'FontWeight', 'bold', 'FontSize', 12);
set(gca, 'YTickLabel', []);

% Add a secondary Y-axis on the right side for the last subplot
yyaxis right;

% Optionally set Y-axis limits for the last subplot if needed
%ylim([-50 25]);  % Add a specific range if required

% Set the Y-axis on the right to display tick labels
set(gca, 'YColor', 'k');
  %  print('FillPageFigure','-dpdf','-fillpage')
%title('EMG3 vs torque in emg_elbowTorque60degAban98'); 

