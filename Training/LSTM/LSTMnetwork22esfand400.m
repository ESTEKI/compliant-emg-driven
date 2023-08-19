function y = LSTMnetwork22esfand400(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('LSTMnetwork22esfand400.mat');
end
 y = predict(mynet,x);
end