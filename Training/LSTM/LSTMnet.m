function y = LSTMnet(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('net2lstm_miladch235_netWork.mat');
end
 y = predict(mynet,x);
end