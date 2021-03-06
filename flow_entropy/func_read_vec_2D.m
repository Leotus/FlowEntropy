function [vx, vy, dim] = func_read_vec_2D(filename)
    fid = fopen(filename,'rb');

    dim=fread(fid,3,'int32');
    vec= fread(fid, 3 * dim(1)*dim(2), 'single');
    fclose(fid);

    vec = reshape(vec, 3, dim(1), dim(2));
    vx = squeeze(vec(1, :, :));
    vy = squeeze(vec(2, :, :));
    vx = vx';
    vy = vy';
    dim = size(vx);
end

%
% $Log: not supported by cvs2svn $
%
