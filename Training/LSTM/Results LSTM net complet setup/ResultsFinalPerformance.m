%%
dataPC = struct2cell( load('logFile3azar1400LSTMresults1.mat'));
dataPC = dataPC{1};
dataKUKA = struct2cell( load('UltrasoundImaging2resultsLSTM3azar1.mat'));
dataKUKA = dataKUKA{1};
%%
EstimatedTorque = dataPC.EstimatedForce;
ActualTorque = dataPC.ActualForce;         
kukaForcez = -dataKUKA.fz;

%%
close all
tempKukaForcez = kukaForcez;
tempEstimated = EstimatedTorque;

for ii=1:20  % results a mtrix containig regressions. The step having
%maximum r value is the delay shift >> 8 steps
   tempKukaForcez(1)=[];
   tempEstimated(end)=[];
    r = regression(tempEstimated',tempKukaForcez');
    performance_ruleoutDelay(ii) = r;
end

%% shift step delay to remove delay effect
[maxReg,shift] = max(performance_ruleoutDelay);
kukaForcez(1:shift+1)=[];
EstimatedTorque(end-shift:end)=[];
ActualTorque(end-shift:end)=[];
%%
logicindexing = sign(EstimatedTorque);
EstimatedTorque(logicindexing==1) = [];
ActualTorque(logicindexing==1) = [];
kukaForcez(logicindexing==1) = [];

mse1 = immse(ActualTorque,EstimatedTorque); 
r_1 = regression(ActualTorque',EstimatedTorque'); 
Rsq21 = 1 - sum((ActualTorque - EstimatedTorque).^2)/sum((ActualTorque - mean(EstimatedTorque)).^2);
performanceActual_Estimate = [ mse1 r_1 Rsq21]

mse2 = immse(EstimatedTorque,kukaForcez);
r_2 = regression(EstimatedTorque',kukaForcez');
Rsq22 = 1 - sum(( EstimatedTorque-kukaForcez ).^2)/sum(( EstimatedTorque- mean(kukaForcez)).^2);
performanceEstimate_kukaforce = [ mse2 r_2 Rsq22]

mse3 = immse(ActualTorque,kukaForcez);
r_3 = regression(ActualTorque',kukaForcez');
Rsq23 = 1 - sum((ActualTorque - kukaForcez ).^2)/sum((ActualTorque - mean(kukaForcez)).^2);
performanceActual_kukaForce = [ mse3 r_3 Rsq23]

format long
hold on 
plot(ActualTorque);
plot(EstimatedTorque);
plot(kukaForcez);
legend('Actual Force','Estimated Force','kukaforce')

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


%Data8shahrivar1400rec3ALLresults_modified = 