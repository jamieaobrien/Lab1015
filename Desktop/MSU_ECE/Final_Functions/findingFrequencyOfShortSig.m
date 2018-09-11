

function [D] = findingFrequencyOfShortSig(signalloc, centerFreq, convValue, startloc, endloc)
% Summary : This function is similar to findTimesInLongSig except its used
% for a short signal- an audio file with only a Red-Winged Blackbird song. 
%
%
% This is a final function in which you put in your reference signal, you
% set the parameters: set the right center frequency and convolution size,
% set the time limits. The function will take all of that and produce a
% frequency and number of bumps. It will then compare the frequency and
% number of bumps to all the regions in the database and return a guess as
% to which region it is from

%% Convolutions, Center Frequency and Preprocessing Data

[medSmootheddata, Fs] = preparingreferencesignal(signalloc, centerFreq, convValue);

 medSmootheddata1 = medSmootheddata(startloc:endloc);
 %% Calculating Number of Bumps and Frequency

round = length(medSmootheddata1) - rem(length(medSmootheddata1), 2000);
    rounded = medSmootheddata1(1:round);
    reshapeddata = reshape(rounded, [], length(rounded)/2000);
    M = mean(reshapeddata);
    Mother = M + M.*0.1;
    OUTPUTlist = [];
    windowinterval = 2000;
    allbumps = 0;

    for j=1:size(reshapeddata,2) 
        if mean(reshapeddata(:,j)) < 0.3*max(reshapeddata(:,j))
            continue
        end
        [OUTPUT,bumps] = hysteresis(reshapeddata(:,j), Mother(j), M(j));
        if length(OUTPUT) ~= 2000 
            OUTPUT = padarray(OUTPUT, [2000 - length(OUTPUT) 0], 'post');
        elseif isempty(OUTPUT)
            continue
        end
        OUTPUTlist = [OUTPUTlist; OUTPUT];
        allbumps = allbumps+bumps;
    end

    if isempty(OUTPUTlist)
       error('File could not be analyzed.');
    end
    frequency = calculatefrequency(OUTPUTlist, Fs);
    D = {signalloc, frequency, allbumps};
end
  
