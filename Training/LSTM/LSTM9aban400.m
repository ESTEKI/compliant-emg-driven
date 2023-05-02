function y = LSTM9aban400(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('LSTM9aban400.mat');
end
 y = predict(mynet,x);
end