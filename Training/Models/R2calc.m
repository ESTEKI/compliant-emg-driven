% After training networks, use this program to calculate RMSE, R and
% R-squared performance index
%Y = net(inputs);
%Y2 = net1(inputstest);

ffnn_y = ffnn_net(buttinputs);

RMSE = rmse(ffnn_y,butttorques);
r = regression(ffnn_y ,butttorques);
Rsq = 1 - sum(( ffnn_y-butttorques ).^2)/sum(( ffnn_y- mean(butttorques)).^2);
FFNN_performances = [ RMSE r Rsq]

lstm_y = predict(lstm_net,buttinputs);

RMSE = rmse(lstm_y,butttorques);
r = regression(lstm_y ,butttorques);
Rsq = 1 - sum(( lstm_y-butttorques ).^2)/sum(( lstm_y- mean(butttorques)).^2);
LSTM_performances = [RMSE r Rsq]

hold on
plot(ffnn_y)

plot(butttorques)
legend('original','network output');



%https://www.mathworks.com/matlabcentral/answers/165233-neural-network-how-does-neural-network-calculate-output-from-net-iw-net-lw-net-b
%to test network using net.IW and net.LW 
%matlab uses mapminmax function to nirmalize input and target data so
%remember apply the above function on input and if needed remove
%net.output.processFcns= { }; in netScript.m so that network output falls
%between -1 to +1

%Algorithm : It is assumed that X has only finite real values, and that the elements of each row are not all equal. (If xmax=xmin or if either xmax or xmin are non-finite, then y=x and no change occurs.)
%y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin; ymax=+1,ymin=-1
%---
%[buttinputsNorm , setting] = mapminmax(buttinputs);
%a1 = tansig(net.IW{1} * buttinputsNorm + net.b{1});
%a2 = net.LW{2} * a1 + net.b{2};% same result as y = net(buttinputs)
%---
%M = filter(b,a,y);% movmean(y,[3 0]);
%M = movmean(y,[100 0]);
%plot(M);
%mse_ = mse(net,butttorques,y)