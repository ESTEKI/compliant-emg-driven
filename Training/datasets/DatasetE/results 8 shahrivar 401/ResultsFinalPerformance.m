%%
dataPC_ffnn = struct2cell( load('logFile9shahrivar1401_results3_FFNN.mat'));
dataPC_ffnn = dataPC_ffnn{1};
dataKUKA_ffnn = struct2cell( load('UltrasoundImaging_results3_FFAN_9shahrivar1401.mat'));
dataKUKA_ffnn = dataKUKA_ffnn{1};

dataPC_lstm = struct2cell( load('logFile9shahrivar1401_results3_LSTM.mat'));
dataPC_lstm = dataPC_lstm{1};
dataKUKA_lstm = struct2cell( load('UltrasoundImaging_results3LSTM9shahrivar1401.mat'));
dataKUKA_lstm = dataKUKA_lstm{1};
format long

% Parameters
begin_lstm = 1460;
begin_ffnn = 1;
range_num = 3480;

% Initialize figure
figure('Position', [0, 0, 1200, 1200]);

% Process and plot LSTM data
subplot(2, 1, 1); % Create a subplot in the first row
[EstimatedTorque_lstm, ActualTorque_lstm, kukaForcez_lstm] = processForceData(dataPC_lstm, dataKUKA_lstm);
hold on;
plot(ActualTorque_lstm(begin_lstm:begin_lstm+range_num), 'b-', 'LineWidth', 1.5);
plot(EstimatedTorque_lstm(begin_lstm:begin_lstm+range_num), 'r-', 'LineWidth', 1.5);
plot(kukaForcez_lstm(begin_lstm:begin_lstm+range_num), 'g-.', 'LineWidth', 1);
%xlabel('Time Step');
ylabel('Force [N]');
title('LSTM Results');
legend({'Actual', 'Estimated', 'End-effector'}, ...
    'Location', 'southwest', 'Orientation', 'horizontal', 'Box', 'off');
grid on;
hold off;
% Adjust the position of the first subplot to reduce space below it
%subplot(2, 1, 1);
set(gca, 'Position', [0.1, 0.55, 0.85, 0.4]); % [left, bottom, width, height]
% Process and plot FFNN data
subplot(2, 1, 2); % Create a subplot in the second row
[EstimatedTorque_ffnn, ActualTorque_ffnn, kukaForcez_ffnn] = processForceData(dataPC_ffnn, dataKUKA_ffnn);
hold on;
plot(ActualTorque_ffnn(begin_ffnn:begin_ffnn+range_num), 'b-', 'LineWidth', 1.5);
plot(EstimatedTorque_ffnn(begin_ffnn:begin_ffnn+range_num), 'r-', 'LineWidth', 1.5);
plot(kukaForcez_ffnn(begin_ffnn:begin_ffnn+range_num), 'g-.', 'LineWidth', 1);
% plot(ActualTorque_ffnn, 'b-', 'LineWidth', 1.5);
% plot(EstimatedTorque_ffnn, 'r-', 'LineWidth', 1.5);
% plot(kukaForcez_ffnn, 'g-.', 'LineWidth', 1);
xlabel('Time Step');
ylabel('Force [N]');
title('FFNN Results');
legend({'Actual', 'Estimated', 'End-effector'}, ...
    'Location', 'southwest', 'Orientation', 'horizontal', 'Box', 'off');
grid on;
hold off;
% Adjust the position of the second subplot to bring it closer to the first subplot
subplot(2, 1, 2);
set(gca, 'Position', [0.1, 0.05, 0.85, 0.4]); % [left, bottom, width, height]

% Adjust layout for better visualization
%sgtitle('Comparison of LSTM and FFNN Results'); % Super title for the entire figure


%%%%%%%%%%%%
% format long
% 
% begin_lstm = 1460;
% begin_ffnn = 2000;
% range_num = 3480;
% 
% [EstimatedTorque, ActualTorque, kukaForcez] = processForceData(dataPC_lstm, dataKUKA_lstm);
% figure('Position', [100, 100, 1200, 600]);
% hold on 
% %uncomment for drawing LSTM
% plot(ActualTorque(begin_lstm:begin_lstm+range_num), 'b-', 'LineWidth', 1.5);
% plot(EstimatedTorque(begin_lstm:begin_lstm+range_num), 'r-', 'LineWidth', 1.5);
% plot(kukaForcez(begin_lstm:begin_lstm+range_num), 'g-.', 'LineWidth', 1);
% 
% [EstimatedTorque, ActualTorque, kukaForcez] = processForceData(dataPC_ffnn, dataKUKA_ffnn);
% %uncomment for drawing FFNN
% plot(ActualTorque, 'b-', 'LineWidth', 1.5);
% plot(EstimatedTorque, 'r-', 'LineWidth', 1.5);
% plot(kukaForcez, 'g-.', 'LineWidth', 1);
% 
% xlabel('Time Step');
% ylabel('Force [N]');
% title('LSTM results'); 
% 
% grid on;
% 
% legend('Actual Force', 'Estimated Force', 'End-effector Force', 'Location', 'Northwest');
%%%%%%%%%%%%%%%%%%
%saveas(gcf, 'LSTM_realitime_8sh401v2.png');
%xlim([start_time, end_time]); % Set your desired time range

% prompt = 'Save results? : ';
% userIn = input(prompt,'s');
% if (userIn == 'y')
% %  dataModified = data;  
% %  dataModified.ActualForce = [];
% %  dataModified.EstimatedForce = [];
% %  dataModified.KukaForceZ = [];
%   
%  dataModified.ActualForce = ActualTorque;
%  dataModified.EstimatedForce = EstimatedTorque;
%  dataModified.KukaForceZ = kukaForcez;
% 
% % dataModified
% end


function [EstimatedTorque, ActualTorque, kukaForcez] = processForceData(dataPC, dataKUKA)

    % Initial Data Extraction
    EstimatedTorque = dataPC.EstimatedForce;
    ActualTorque = dataPC.ActualForce;         
    kukaForcez = -dataKUKA.fz;

    %% Adjust the size of EstimatedTorque and ActualTorque to match kukaForcez
    numofelement = length(kukaForcez);
    EstimatedTorque(numofelement+1:end) = [];
    ActualTorque(numofelement+1:end) = [];

    %% Calculate the performance to determine the delay shift
    tempKukaForcez = kukaForcez;
    tempEstimated = EstimatedTorque;
    performance_ruleoutDelay = zeros(1, 20);  % Preallocate for speed

    for ii = 1:20  
       tempKukaForcez(1) = [];
       tempEstimated(end) = [];
       r = regression(tempEstimated', tempKukaForcez');
       performance_ruleoutDelay(ii) = r;
    end

    %% Determine and apply the delay shift
    [~, shift] = max(performance_ruleoutDelay);
    kukaForcez(1:shift+1) = [];
    EstimatedTorque(end-shift:end) = [];
    ActualTorque(end-shift:end) = [];

    %% Remove positive EstimatedTorque values and apply limits to ActualTorque
    logicindexing = sign(EstimatedTorque);
    EstimatedTorque(logicindexing == 1) = [];
    ActualTorque(logicindexing == 1) = [];
    kukaForcez(logicindexing == 1) = [];

    ActualTorque(ActualTorque > 2) = 0;          % Remove outliers
    ActualTorque(ActualTorque < -60) = -40;      % Remove outliers
    ActualTorque = movmean(ActualTorque, 2);     % Apply moving mean filter

    EstimatedTorque = EstimatedTorque - 0;       % This seems redundant but kept for completeness

    %% Calculate performance metrics (optional, remove if not needed)
    rmse1 = rmse(ActualTorque, EstimatedTorque); 
    r_1 = regression(ActualTorque', EstimatedTorque'); 
    Rsq21 = 1 - sum((ActualTorque - EstimatedTorque).^2)/sum((ActualTorque - mean(EstimatedTorque)).^2);
    performanceActual_Estimate = [rmse1 r_1 Rsq21];

    rmse2 = rmse(EstimatedTorque, kukaForcez);
    r_2 = regression(EstimatedTorque', kukaForcez');
    Rsq22 = 1 - sum((EstimatedTorque - kukaForcez).^2)/sum((EstimatedTorque - mean(kukaForcez)).^2);
    performanceEstimate_kukaforce = [rmse2 r_2 Rsq22];

    rmse3 = rmse(ActualTorque, kukaForcez);
    r_3 = regression(ActualTorque', kukaForcez');
    Rsq23 = 1 - sum((ActualTorque - kukaForcez).^2)/sum((ActualTorque - mean(kukaForcez)).^2);
    performanceActual_kukaForce = [rmse3 r_3 Rsq23];
    
    kukaForcez(kukaForcez<-50)=-40;%remove outliars for better illustration

end