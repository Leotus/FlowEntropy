function [vx, vy, vz, dim] = func_read_vec(filename)
    fid = fopen(filename,'rb');

    dim=fread(fid,3,'int32');
    dim = reshape(dim, 1, length(dim)); % convert dim to row vector
    vec= fread(fid, 3 * dim(1)*dim(2)*dim(3), 'single');
    fclose(fid);

    vec = reshape(vec, 3, dim(1), dim(2), dim(3));
%     vx = squeeze(vec(1, :, :, :));
%     vy = squeeze(vec(2, :, :, :));
%     vz = squeeze(vec(3, :, :, :));
    dim(1:2) = dim(2:-1:1); % flip the xyz domain
    vx = zeros(dim);
    vy = zeros(dim);
    vz = zeros(dim);
	for z = 1:dim(3)
        vx(:, :, z) = squeeze(vec(1, :, :, z))';
        vy(:, :, z) = squeeze(vec(2, :, :, z))';
        vz(:, :, z) = squeeze(vec(3, :, :, z))';
	end
end

%
% $Log: not supported by cvs2svn $
%
