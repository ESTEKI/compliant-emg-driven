%%
data = struct2cell( load('logFile8shahrivar1400rec3ALLresults.mat'));
data = data{1};

%%
EstimatedTorque = movmean(data.EstimatedForce+4.0,25);%% +4.0 and 25 for moving average
ActualTorque = movmean( data.ActualForce,5);           % 5 for movmean
kukaForcez = movmean(-data.KukaForceZ+4.0,25);

hold on 
subplot(3,1,1);
plot(ActualTorque,'-b','LineWidth',1);
title('Actual Force (F_a_c_t)');
ylabel('Force (N)');

subplot(3,1,2);
plot(EstimatedTorque,'-b','LineWidth',1);
title('Estimated Force (F_e_s_t)');
ylabel('Force (N)');

subplot(3,1,3);
plot(kukaForcez,'-b','LineWidth',1);
title('Robot cartesian force along Z (F_r)');
xlabel('Step');
ylabel('Force (N)');

print('FillPageLSTM','-dpdf','-fillpage')
