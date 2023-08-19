fid=fopen('UltrasoundImaging3results2LSTM17esfand400_2.txt');
C = textscan(fid,'%s');
C=C{1};
C(1:13)=[];
i = 6;
bC = C;
while (i<numel(C))%
   %if (strlength(C(i))> 10)
     idx = strfind(C(i),'.');
     idx = idx{1}(1); %first '.' in string
     C{i} = [C{i}(1:idx+6), ' ',C{i}(idx+6+1:end)];
     cellstring = split(C{i});
     C1 = C(1:i-1);
     C2 = C(i+1:end);
     C1(end+1) = cellstring(1);
     C1(end+1) = cellstring(2);
     C = [C1 ; C2];
   %end
 i = i+6;   
end

%%
j=1;
k=1;
for i = 1:numel(C)
  %for j=1:6
     Cmod(j,k) = C(i); 
     k = k+1;
     if (k==7)
         j=j+1;
         k=1;
     end
  %end
end

cc = str2double(Cmod);
plot ( cc(:,3));
%fid = fopen( 'results.txt', 'wt' );
%%
fclose(fid);