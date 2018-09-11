function [candidates] = reference_longaudio(referencesignal, audiofile, variability, destdirectory)
% this function takes in a reference signal and a long audio file. A GUI
% pops up asking the user to specify the convValue, centerFrequency, start
% and stop times. It then uses this to calculate the frequency and number
% of bumps it should check for. Finally, it checks the whole audio file with these specifications, using the 
% findTimesInLongSig.m function.

% Optional Parameters:
%   -Variability : +/- how many of the reference frequency and reference
%   number of bumps should your program test for. Inputs are : 1 (+/- 3) ,
%   2 (+/- 5), 3 (+/- 10). Will default to 1 (+/- 3).
%   -destdirectory: Saves the GUI plot to a directory of your choice. It
%   can make a new directory, if you type in what you want your new
%   directory to be called. Type 0 (zero) for no graph.
    
    %% Analyzing Reference Signal
    referencesignalApp(referencesignal, destdirectory); %starting GUI
    f = warndlg('Continue?', 'Click to Continue'); %Method to wait before guidatabase
    drawnow     % Necessary to print the message
    waitfor(f);
    D = calculateFandNbumps(referencesignal, evalin('base','centerFreq'), evalin('base','convValue'), evalin('base','startloc'), evalin('base','endloc'));
    frequencyRef = D{1,2};
    numberBumps = D{1,3};
    %% Comparing to long audio file
    tic
    [candidates] = findingcandidatesinlongsignal(audiofile,evalin('base','centerFreq'), evalin('base','convValue'), frequencyRef,  variability);
    toc
end