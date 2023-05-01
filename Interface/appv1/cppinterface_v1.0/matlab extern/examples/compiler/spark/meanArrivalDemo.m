%
% Copyright 2016 MathWorks, Inc
%
function meanArrivalDemo(master,inputFile,outputFile)
%%
% Set up datastore for airlinesmall.csv
%
ds = datastore( ...
    inputFile, ...
    'datastoretype','tabulartext', ...
    'TreatAsMissing','NA', ...
    'SelectedVariableNames','ArrDelay' ...
    );
%%
% Set up spark to use yarn resource manager
%   1 thread per executor task
%   1 executor task
sparkProperties = containers.Map( ...
    {'spark.executor.cores','spark.executor.instances'}, ...
    {'1','1'});
%%
% Indicate that environment is deployment
mr = matlab.mapreduce.DeploySparkMapReducer( ...
    'SparkProperties', sparkProperties, ...
    'AppName', 'meanArrivalDemo', ...
    'Master', master ...
    );
mapreducer(mr)
%%
% Create tall table and compute mean on ArrDelay column
%
t = tall(ds);
meanresult=mean(t.ArrDelay, 1, 'omitnan');
write(outputFile,meanresult);
