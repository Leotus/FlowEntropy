% return the geometry of the graph nodes as a N x 6 matrix
% for each row, the first 3 entries represent coordinates of the the
% lower-bottom-left corner, and the last 3 entries represent the size of
% the corresponding block.
function block_node_bboxs = func_comp_block_based_node_bboxs(vec_size, block_size)
    cell_size = vec_size - 1; % assume the vector are defined over the corners
    n_blocks_per_dim = ceil(cell_size ./ block_size);
    n_blocks = prod(n_blocks_per_dim);
    n_blocks = prod(n_blocks_per_dim);
    
    block_node_bboxs = zeros(n_blocks, 6);
    b_ind = 0;
    for bk = 1:n_blocks_per_dim(3)
        bz = 1 + (bk-1)*block_size(3);
        for bj = 1:n_blocks_per_dim(2)
            bx = 1 + (bj-1)*block_size(2);
            for bi = 1:n_blocks_per_dim(1)
                by = 1 + (bi-1)*block_size(1);
                b_ind = b_ind + 1;
                bh = min(by + block_size(1), vec_size(1)) - by;
                bw = min(bx + block_size(2), vec_size(2)) - bx;
                bd = min(bz + block_size(3), vec_size(3)) - bz;
                block_node_bboxs(b_ind, :) = [by, bx, bz, bh, bw, bd];
            end
        end
    end
 end

%
% $Log: not supported by cvs2svn $
%
