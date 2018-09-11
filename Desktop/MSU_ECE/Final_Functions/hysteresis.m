function [OUTPUT, bumps ] = hysteresis(data, higherthreshold, lowerthreshold)
% This function takes in the data and the thresholds
% It toggles on when it reaches the upperthreshold in the data and converts
% all the datapoints to 1. When it reaches the lower threshold, it toggles
% off and sets everything to 0 until it reaches the upper threshold again. 
below = data(data<=lowerthreshold); % below threshold
above = data(data>=higherthreshold); % above threshold
bumps = 0;
OUTPUT = [];

while ~isempty(data)
    aboveid = find((data>=higherthreshold) ==1);
    if isempty(aboveid)
        return
    end
    ia = aboveid(1);
    data(1:ia-1) = 0;
    OUTPUT = [OUTPUT;data(1:ia-1)];
    data = data(ia:end);
    if isempty(data)
        return 
    end
    belowid = find((data<=lowerthreshold) ==1);
     if isempty(belowid)
         return
     end
   ib = belowid(1);

   data(1:ib) = 1;
   bumps = bumps + 1;
   OUTPUT = [OUTPUT;data(1:ib)];
   if ib == length(data)
       return
   end
    data = data(ib+1:end);
end
    

        