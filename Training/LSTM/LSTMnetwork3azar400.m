function y = LSTMnetwork3azar400(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('LSTMnetwork3azar400.mat');
end
 y = predict(mynet,x);
end