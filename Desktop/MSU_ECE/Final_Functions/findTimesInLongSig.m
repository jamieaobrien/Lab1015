function [candidates] = findTimesInLongSig(audiofile, centerFreq, convValue, frequencyRef, variability)
% This is the first final function to check for the times at which
% Red-Winged Blackbird songs were found in long audio files. It takes in
% the parameters that you set, instead of a reference signal. 
% 
% 
% Variability set to 1,2,3. This means that either :
% 1: the frequency range will be +/- 3 of the reference signal 
% 2: the frequency range will be +/- 5 of the reference signal 
% 3:  the frequency range will be +/- 10 of the reference signal 
%% Preparing File
    [datamono, Fs] = converttomono(audiofile);
    [medSmootheddata, ~, newFs] = preparingwindow(datamono, Fs, centerFreq, convValue);
%% Reshaping Data
v = medSmootheddata;
cs = 20000;
sh = 18000;

A = v(bsxfun(@plus,(1:cs),(0:sh:length(v)-cs)'));

%% Finding Time Intervals that have the same frequency and number of bumps

    candidates = cell(size(A,1),5);
%     numbersamples= 1:1:length(v);
%     times = (1:1/newFs:length(numbersamples)/newFs).';

    for j= 1:size(A,1)
    
        [reshapeddata, M, Mother] = reshapingdata(A, j, 2000);
        % minimizing errors 
%         if any(isoutlier(Mother)) ==1 || any(isoutlier(M)) ==2
%             continue
%         end
        [OUTPUTlist, allbumps] = hysteresisloop(reshapeddata, M, Mother);
        if isempty(OUTPUTlist)
            continue
        end

    frequencyNew = calculatefrequency(OUTPUTlist, Fs);

    if variability == 1
        delta = 3;
    elseif variability == 2
        delta = 5;
    elseif variability == 3
        delta = 10;
    elseif ~exist('variability', 'var')
        warning('variability not chosen. Defaulting to variability == 1, +/- 3 from ref signal');
        delta = 3;
    else
        warning('the number you chose is not applicable. Defaulting to variability == 1, +/- 3 from ref signal');
    end
        
        if frequencyNew >= frequencyRef - delta && frequencyNew <=frequencyRef + delta  && allbumps >= 10 %numberBumps - 10 && allbumps <= numberBumps + 10
            D = calculatetraits(A, j, v, allbumps, newFs,  frequencyNew);

            candidates(j,:)  = D;
        end

    
candidates= candidates(~any(cellfun('isempty', candidates), 2), :);
for k=2:size(candidates,1)
    if candidates{k,4}(1) < candidates{k-1,4}(2)
        candidates{k-1,1} = [candidates{k-1,1} candidates{k,1}];
        candidates{k-1,2} = mean( [candidates{k-1,2}  candidates{k,2}]);
         candidates{k-1,3} =  candidates{k-1,3} +  candidates{k,3};
          candidates{k-1,4} = [ candidates{k-1,4}(1) candidates{k,4}(2)];
          candidates{k-1,5} = candidates{k-1,5} +  candidates{k,5};
          candidates(k,:) = [];
    end
end

    end