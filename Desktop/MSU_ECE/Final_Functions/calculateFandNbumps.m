function [D] = calculateFandNbumps(signalloc, centerFreq, convValue, startloc, endloc)
% This function takes in all necessary parameters to calculate the
% frequency and number of bumps of a reference signal and returns them as a
% cell. 
% Optional parameters:
%   -Graphing the 

%% Preprocessing Data
[medSmootheddata, Fs] = preparingreferencesignal(signalloc, centerFreq, convValue);

 medSmootheddata1 = medSmootheddata(startloc:endloc);
 %% Calculating Number of Bumps and Frequency
    %reshaping
    [reshapeddata, M, Mother] = reshapingdatareferencesignal(medSmootheddata1);
    %calculating number of bumps and preparing data for calculatefrequency
    [OUTPUTlist, allbumps] = hysteresisloop(reshapeddata,  M, Mother);

    if isempty(OUTPUTlist)
       error('File could not be analyzed.');
    end
    
    frequency = calculatefrequency(OUTPUTlist, Fs);
    D = {signalloc, frequency, allbumps};
end