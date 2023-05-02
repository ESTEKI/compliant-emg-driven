function y = LSTMnetwork17esfand400(x)
persistent mynet;

if isempty(mynet)
    mynet = coder.loadDeepLearningNetwork('LSTMnetwork17esfand400.mat');
end
 y = predict(mynet,x);
end