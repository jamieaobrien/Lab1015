function [datamono, Fs] = converttomono(filename, opt1)
%This function takes an audio file and returns the sampling frequency and
%converts the data into one channel. Has an optionl parameter of converting
%the data into native
    if nargin == 2
        [y,Fs] = audioread(filename, 'native'); %imports file
    else
        [y, Fs] = audioread(filename);
    end
    %converts data to monochannel
    if size(y,2) > 1
        [~,n] = size(abs(y));
        datamono = sum(y, n) / size(y, n); 
    else
        datamono = y;
    end
end