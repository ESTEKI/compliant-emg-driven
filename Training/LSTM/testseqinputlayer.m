
% inp = num2cell(buttinputs);
% outp= num2cell(butttorques);
numFeatures = size(buttinputs,1);
numResponses = size(butttorques ,1);

% numFeatures = size(inp,1);
% numResponses = size(outp ,1);

numHiddenUnits = 8;

% mu = mean(buttinputs,2);
% sig = std(buttinputs,0,2);
% 
% for i = 1:3
%     buttinputs_norm(i,:) = (buttinputs(i,:) - mu(i)) ./ sig(i);
%        % buttinputs_norm(i,:) = (buttinputs(i,:) - mu(i));
% 
% end
% 
% muT = mean(butttorques,2);
% sigT = std(butttorques,0,2);
% butttorques_norm = (butttorques - muT) ./ sigT;

%buttinputs_norm = mapminmax(buttinputs,-1,1);
%butttorques_norm = mapminmax(butttorques,-1,1);
%butttorques_norm = butttorques;

% buttinputs_norm = mapminmax(buttinputs_norm,0,1);
% butttorques_norm = mapminmax(butttorques_norm,0,1);

layers = [ ...
    sequenceInputLayer(numFeatures)
    %batchNormalizationLayer
    lstmLayer(numHiddenUnits,'OutputMode','sequence')
    fullyConnectedLayer(numResponses)
    regressionLayer];
   

maxEpochs = 600;
miniBatchSize = 20;

options = trainingOptions('adam', ...
    'MaxEpochs',maxEpochs, ...
    'MiniBatchSize',miniBatchSize, ...
    'InitialLearnRate',0.02, ...
    'GradientThreshold',1, ...
    'Shuffle','never', ...
    'Plots','training-progress',...
    'Verbose',0);

net = trainNetwork(buttinputs,butttorques,layers,options);
y = predict(net,buttinputs);
plot(y);
hold on ;
plot(butttorques);
legend('y','targets');