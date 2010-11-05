function [XYZ, XYZ_neg] = func_place_random_streamlines(vx, vy, vz, n_seeds)
    dim = size(vx);
    n_voxels = prod(dim);
    randperm_inds = randperm(n_voxels);
    seed_inds = randperm_inds(1:n_seeds);
    seed_inds = seed_inds(:);
    [seed_ys, seed_xs, seed_zs] = ind2sub(dim, seed_inds);
    seed_ys = seed_ys + rand(n_seeds, 1) - 0.5;
    seed_xs = seed_xs + rand(n_seeds, 1) - 0.5;
    seed_zs = seed_zs + rand(n_seeds, 1) - 0.5;
    
    XYZ =       stream3( vx,  vy,  vz, seed_xs, seed_ys, seed_zs);
    XYZ_neg =   stream3(-vx, -vy, -vz, seed_xs, seed_ys, seed_zs);
end

%
% $Log: not supported by cvs2svn $
%
