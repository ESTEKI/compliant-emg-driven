function y = LSTMnet(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('LSTMfinal1network.mat');
end
 y = predict(mynet,x);
end