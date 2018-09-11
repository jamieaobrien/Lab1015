function[D] = calculatetraits(A, i, v, allbumps, newFs,frequency, opt1, opt2)
% This function is run after frequency is in the right range.
% It returns the important information about the candidate signals and
% returns them in a cell array that you can then save to a matrix
    if nargin == 8
        starttime = opt1;
        endtime = opt2;
 
    else
        starttimeidx = find(v == A(i,1),1);
        starttime = starttimeidx/ newFs;
        endtimeidx = find(v==A(i,end));
        endtimeidx = endtimeidx(end);
        endtime  = endtimeidx/ newFs;

    end
    
        timeofcall = [starttime endtime];
        deltatime = endtime - starttime;
        D = {i, frequency, allbumps,timeofcall, deltatime };

end