function [frequency] = calculatefrequency(OUTPUTlist, Fs)
% This function takes the outputlist and calculates the frequency of the
% oscillations from it using periodogram function
    periodogramlist = OUTPUTlist;
    [pxx,f] = periodogram(periodogramlist,[],[],Fs);
    pxx = pxx(3:end);
    f = f(3:end);
    [~,loc] = max(pxx);
    frequency = f(loc);
end