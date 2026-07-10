% Columns are: RMSE, R, R-squared

FFNN = [
   
    0.2963    0.7824    0.4315
    0.3108    0.7624    0.3687
    0.2083    0.8882    0.7835
    0.3111    0.8504    0.6586
    0.2148    0.8825    0.7580
    0.2917    0.8432    0.6493
    0.20      0.93      0.86
];

LSTM = [
    0.0329    0.9761    0.9613
    0.0740    0.9676    0.9394
    0.0406    0.9762    0.9598
    0.0190    0.9786    0.9671
    0.0145    0.9785    0.9667
    0.0340    0.9770    0.9632
    0.13      0.97      0.94
];

FFNN_mean = mean(FFNN, 1);
FFNN_sd   = std(FFNN, 0, 1);

LSTM_mean = mean(LSTM, 1);
LSTM_sd   = std(LSTM, 0, 1);

fprintf('FFNN mean ± SD:\n');
fprintf('RMSE: %.4f ± %.4f\n', FFNN_mean(1), FFNN_sd(1));
fprintf('R: %.4f ± %.4f\n', FFNN_mean(2), FFNN_sd(2));
fprintf('R-squared: %.4f ± %.4f\n\n', FFNN_mean(3), FFNN_sd(3));

fprintf('LSTM mean ± SD:\n');
fprintf('RMSE: %.4f ± %.4f\n', LSTM_mean(1), LSTM_sd(1));
fprintf('R: %.4f ± %.4f\n', LSTM_mean(2), LSTM_sd(2));
fprintf('R-squared: %.4f ± %.4f\n', LSTM_mean(3), LSTM_sd(3));