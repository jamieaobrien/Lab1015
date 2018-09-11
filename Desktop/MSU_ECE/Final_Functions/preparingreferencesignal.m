function [datatobeplotted, Fs] = preparingreferencesignal(datastring, centerFreq, convValue)
% This is a function that helps you prepare a cut reference
% signal for modulation testing. This is a helper function for the plotting
% GUI and it will take the reference signal , conv number, and center
% frequency and it will return the data that will be plotted
[datamono, Fs] = converttomono(datastring);
%preparing data : putting through envelope and octave filter
[datatobeplotted, ~, ~] = preparingwindow(datamono, Fs, centerFreq, convValue);

end