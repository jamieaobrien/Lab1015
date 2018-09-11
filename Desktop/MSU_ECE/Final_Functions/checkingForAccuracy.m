% This is a script that I created to check the frequencies of all the
% sample Red-winged Blackbird songs extracted from a long audio file into
% separate wav files. 
% I created this script to check the frequencies of the outputs of
% reference_TimesInLongSig.m and TimesInLongSig.m. I did this by saving the
% times of the outputs of those two functions and going back into the audio
% file, extracting those snippets of time into their own wav files and
% saving them to one folder. 

%% Extracting the file names from your folder of audio files
myFolder = '/home/allison/Downloads/utah2analysis/'; %change this input
% Check to make sure that folder actually exists.  Warn user if it doesn't.
if ~isdir(myFolder)
  errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
  uiwait(warndlg(errorMessage));
  return;
end
% Get a list of all files in the folder with the desired file name pattern.
filePattern = fullfile(myFolder, '*.wav'); % Change to whatever pattern you need.
theFiles = dir(filePattern);
theFiles = struct2cell(theFiles);
%% Creating the Output Matrix and Setting the Destination Directory for Graphs
alloutputs =   cell(size(A,1),5);
destdirectory = '/home/allison/utahTHIRDTRYanalysisPictures/'; %change this input
mkdir(destdirectory); %comment if running more than once for one set of audio files
%% For loop which check for the individual frequencies of each signal. Works like reference_TimesInLongSig.m
for k = 1:length(theFiles)
    string = strcat(myFolder, theFiles{1,k});
    referencesignalApp(string, destdirectory)
    f = warndlg('Continue?', 'Click to Continue');
    drawnow     % Necessary to print the message
    waitfor(f);
    D = findingFrequencyOfShortSig(string, evalin('base','centerFreq'), evalin('base','convValue'), evalin('base','startloc'), evalin('base','endloc'));
    D{1,4} = centerFreq;
    D{1,5} = convValue;
    alloutputs{k,:} = D;

    end


% save('/home/allison/Oregoncleanfrequencies.mat', alloutputs);