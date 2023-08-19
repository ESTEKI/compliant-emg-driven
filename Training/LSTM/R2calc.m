%Y = net(inputs);
%Y2 = net1(inputstest);
%buttinputs_norm = [datafilt(15000:end,3) datafilt(15000:end,7) datafilt(15000:end,8)]' ; 
%butttorques_norm =  transpose(datafilt4(15000:end));
%buttinputs_norm = (1-(-1)).*(buttinputs_norm-mn)./(mx-mn)-1;
%buttinputs_norm = mapminmax(buttinputs,0,2);
%buttinputs_norm = mapminmax(buttinputs_norm,-1,1);
%y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin;
y = predict(net,buttinputs);
y = mapminmax('reverse',y,maxminOutput);

hold on 
%plot(torqes);
butttorques_norm = mapminmax('reverse',butttorques,maxminOutput) ;
plot(butttorques_norm);
% butttorques_norm = butttorques;
%y = -1*y;
%y=y*sigT+muT;
plot(y);
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
mse=0;
for i=1:length(butttorques_norm)
mse=mse+(y(i)- butttorques_norm(i))^2;
end
mse=mse/length(butttorques_norm)
r = regression(butttorques_norm,y)
Rsq2 = 1 - sum((butttorques_norm - y).^2)/sum((butttorques_norm - mean(butttorques_norm)).^2)
legend('original','network output');
%title('60deg');