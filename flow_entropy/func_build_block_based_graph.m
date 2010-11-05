function block_graph = func_comp_block_based_graph(vx, vy, vz, block_size, weight_computation)
    is_marking_all_occupied_voxels = false;
    is_finding_neighbor_by_lookup_table = false;
    is_backward_advention = false;
    debug_flag = false;
    
    %% 
    vec_size = size(vx);
    cell_size = vec_size - 1; % assume the vector are defined over the corners
    n_blocks_per_dim = ceil(cell_size ./ block_size);
    n_blocks = prod(n_blocks_per_dim);

    n_neighbors = 6;    

    %% build the directions to the 26 neighbors
    switch(n_neighbors)
        case 26
            neighbor_dirs = [];
            for nw = -1:1
                for nv = -1:1
                    for nu = -1:1
                        if( 0 == nw && 0 == nv && 0 == nu )
                            continue;
                        end
                        vec = [nu, nv, nw];
                        neighbor_dirs(end+1, :) = vec / norm(vec);
                    end
                end
            end
            
        case 6
            neighbor_dirs = [...
                -1, 0, 0;
                +1, 0, 0;
                 0,-1, 0;
                 0,+1, 0;
                 0, 0,-1;
                 0, 0,+1;
                 ];
    end

    %%
    n_faces_per_block = 6;
    adj_block_offsets = [...
        -1, 0, 0;
        +1, 0, 0;
         0,-1, 0;
         0,+1, 0;
         0, 0,-1;
         0, 0,+1;
        ];

    if( is_finding_neighbor_by_lookup_table )
        voxel_faces = zeros(block_size);
        for fi = 1:n_faces_per_block
            switch fi
                case 1
                    src_inds = voxel_inds(1, 2:end-1, 2:end-1);
                case 2
                    src_inds = voxel_inds(end, 2:end-1, 2:end-1);
                case 3
                    src_inds = voxel_inds(2:end-1, 1, 2:end-1);
                case 4
                    src_inds = voxel_inds(2:end-1, end, 2:end-1);
                case 5
                    src_inds = voxel_inds(2:end-1, 2:end-1, 1);
                case 6
                    src_inds = voxel_inds(2:end-1, 2:end-1, end);
            end
            voxel_faces(src_inds) = fi;
        end
    end
    
    %% build the graph
    block_graph = sparse(n_blocks, n_blocks);
    for bw = 1:n_blocks_per_dim(3)
        block_ws = (1 + (bw-1)*block_size(3)):min(1 + bw*block_size(3), vec_size(3));
        for bv = 1:n_blocks_per_dim(2)
            block_vs = (1 + (bv-1)*block_size(2)):min(1 + bv*block_size(2), vec_size(2));
            for bu = 1:n_blocks_per_dim(1)
                block_us = (1 + (bu-1)*block_size(1)):min(1 + bu*block_size(1), vec_size(1));

                %% show the progress
                n_bytes = fprintf(1, '%d/%d, %d/%d, %d/%d\n', bu, n_blocks_per_dim(1), bv, n_blocks_per_dim(2), bw, n_blocks_per_dim(3));
                for bi = 1:n_bytes
                    fprintf(1, '\b');
                end
                
                %% fetch the vector fiel in the block
                block_vx = vx(block_us, block_vs, block_ws);
                block_vy = vy(block_us, block_vs, block_ws);
                block_vz = vz(block_us, block_vs, block_ws);
                curr_block_size = size(block_vx);

                block_sub = [bu, bv, bw];
                block_id = sub2ind(n_blocks_per_dim, bu, bv, bw);

                %% consider all voxels
                switch weight_computation 
                    case 'streamline'
                        face_weights = zeros(1, n_faces_per_block);
                        voxel_occupied = zeros(curr_block_size);
                        voxel_occupied(:) = -1;
                        
                        is_reaching_critical_points = false;
                        for bzi = 1:curr_block_size(3)-1
                            for bxi = 1:curr_block_size(2)-1
                                for byi  = 1:curr_block_size(1)-1
                                    if( voxel_occupied(byi, bxi, bzi) >= 0 )
                                        continue;
                                    end
                                    XYZ =       stream3( block_vx, block_vy, block_vz,  bxi + 0.5, byi + 0.5, bzi + 0.5);
                                    if( is_backward_advention )
                                        XYZ_neg =   stream3(-block_vx,-block_vy,-block_vz,  bxi + 0.5, byi + 0.5, bzi + 0.5);
                                        seed_trace = XYZ{1};
                                        seed_trace_rev = XYZ_neg{1};
                                        stream = [seed_trace_rev(end:-1:1, :);seed_trace];
                                    else
                                        stream = XYZ{1};
                                    end

                                    stream(:,[2,1]) = stream(:, 1:2);
                                    is_stream_ending_with_nan = false;
                                    if( sum(isnan(stream(:))) > 0 )
                                        if( ~is_reaching_critical_points )
                                            warning('The seed (%d, %d, %d) in block (%d, %d, %d) reaches a critical point\n', bxi, byi, bzi, bu, bv, bw);
                                        end
                                        is_reaching_critical_points = true;
                                        is_stream_ending_with_nan = true;
                                        stream = stream(find(~isnan(stream(:, 1))), :);
                                    end
                                    
                                    if( is_marking_all_occupied_voxels )
                                        occupied_voxel_subs = floor(stream);
                                        occupied_voxel_fracts = stream - (occupied_voxel_subs + 0.5);
                                        occupied_voxel_offsets = sum(occupied_voxel_fracts .* occupied_voxel_fracts, 2).^0.5;
                                        occupied_voxel_inds = find(occupied_voxel_offsets < 0.4);
                                        occupied_ys = occupied_voxel_subs(occupied_voxel_inds, 1);
                                        occupied_xs = occupied_voxel_subs(occupied_voxel_inds, 2);
                                        occupied_zs = occupied_voxel_subs(occupied_voxel_inds, 3);
                                    end

                                    %% find the face that this streamline ends
                                    if( is_finding_neighbor_by_lookup_table )
                                        face_ind = voxel_faces(occupied_ys(end), occupied_xs(end), occupied_zs(end));
                                    else
                                        if( is_stream_ending_with_nan )
                                            warning('The seed (%d, %d, %d) in block (%d, %d, %d) reaches a critical point\n', bxi, byi, bzi, bu, bv, bw);
                                            face_ind = 0;
                                        else
                                            end_coord = stream(end, :);
                                            end_coord_to_min = end_coord - 1;
                                            end_coord_to_max = curr_block_size - end_coord;
                                            [end_coord_value, end_coord_comp] = min(min([end_coord_to_min; end_coord_to_max]));
                                            face_ind = (end_coord_comp - 1) * 2;
                                            if( end_coord_to_min(end_coord_comp) < end_coord_to_max(end_coord_comp) )
                                                face_ind = face_ind + 1;
                                            else
                                                face_ind = face_ind + 2;
                                            end
                                        end
                                    end

                                    %% 
                                    voxel_occupied(byi, bxi, bzi) = face_ind;
                                    if( is_marking_all_occupied_voxels )
                                        voxel_occupied(occupied_ys(:), occupied_xs(:), occupied_zs(:)) = face_ind;
                                    end
                                end
                            end
                        end
                        
                        for fi = 1:n_faces_per_block
                            weight = length(find(voxel_occupied==fi));
                            if( ~weight )
                                continue;
                            end
                            face_weights(fi) = face_weights(fi) + weight;
                            
                            next_block_sub = block_sub + adj_block_offsets(fi, :);
                            if( next_block_sub(1) < 1 || next_block_sub(1) > n_blocks_per_dim(1) || ...
                                next_block_sub(2) < 1 || next_block_sub(2) > n_blocks_per_dim(2) || ...
                                next_block_sub(3) < 1 || next_block_sub(3) > n_blocks_per_dim(3) )
                                continue;
                            end
                            next_block_id = sub2ind(n_blocks_per_dim, next_block_sub(1), next_block_sub(2), next_block_sub(3));
                            block_graph(block_id, next_block_id) = block_graph(block_id, next_block_id) + weight;
                        end
                                
                            
                        %% DEBUG: plot the streamlines and the graph to debug
                        if( debug_flag )
                            figure;
                            hold on;
                            subplot(1, 2, 1);
                            streamline(XYZ(4:4:end));
                            axis equal;
                            xlim([1, block_size(2)]);
                            ylim([1, block_size(1)]);
                            zlim([1, block_size(3)]);
                            xlabel('X');
                            ylabel('Y');
                            zlabel('Z');
                            view(3);
                            
                            subplot(1, 2, 2);
                            plot(1:n_faces_per_block, face_weights);
                        end
                        
                    case 'boundary'
                        for bbw = 1:length(block_ws)
                            for bbv = 1:length(block_vs)
                                for bbu = 1:length(block_us)
                                    neighbor_offset = [mod(bbu, length(block_us)), mod(bbv, length(block_vs)), mod(bbw, length(block_ws))];

                                    for ci = 1:3
                                        if( neighbor_offset(ci) > 1 )
                                            continue;
                                        end
                                        neighbor_dir = zeros(1, 3);
                                        switch neighbor_offset(ci)
                                            case 0
                                                neighbor_dir(ci) = +1;
                                            case 1
                                                neighbor_dir(ci) = -1;
                                        end

                                        block_vec = [block_vy(bbu, bbv, bbw), block_vx(bbu, bbv, bbw), block_vz(bbu, bbv, bbw)];
                                        block_vec = block_vec / norm(block_vec);

                                        weight = dot(block_vec, neighbor_dir);

                                        next_block_sub = block_sub + neighbor_dir;
                                        if( next_block_sub(1) < 1 || next_block_sub(1) > n_blocks_per_dim(1) || ...
                                            next_block_sub(2) < 1 || next_block_sub(2) > n_blocks_per_dim(2) || ...
                                            next_block_sub(3) < 1 || next_block_sub(3) > n_blocks_per_dim(3) )
                                            continue;
                                        end
                                        next_block_id = sub2ind(n_blocks_per_dim, next_block_sub(1), next_block_sub(2), next_block_sub(3));

                                        weight = max(weight, 0);
                                        block_graph(block_id, next_block_id) = block_graph(block_id, next_block_id) + abs(weight);
                                    end
                                end
                            end
                        end
                        
                    case 'max'
                        %% only consider the mean vector
                        mean_vx = mean(block_vx(:));
                        mean_vy = mean(block_vy(:));
                        mean_vz = mean(block_vz(:));
                        block_vec = [mean_vy, mean_vx, mean_vz];
                        if( norm(block_vec) == 0.0 )
                            continue;
                        end
                        block_vec = block_vec / norm(block_vec);

                        [max_dir, max_comp] = max(abs(block_vec));
                        neighbor_dir = zeros(1, 3);
                        neighbor_dir(max_comp) = sign(block_vec(max_comp));
                        next_block_sub = block_sub + sign(neighbor_dir);
                        block_weight = max_dir;
                        
                        if( next_block_sub(1) < 1 || next_block_sub(1) > n_blocks_per_dim(1) || ...
                            next_block_sub(2) < 1 || next_block_sub(2) > n_blocks_per_dim(2) || ...
                            next_block_sub(3) < 1 || next_block_sub(3) > n_blocks_per_dim(3) )
                            continue;
                        end

                        next_block_id = sub2ind(n_blocks_per_dim, next_block_sub(1), next_block_sub(2), next_block_sub(3));

                        weight = block_weight;
                        if( true == strcmp(weight_computation, 'mean_gaussian') )
                            weight = normpdf(weight, 1, 0.1);
                        end
                        weight = max(weight, 0.0);
                        block_graph(block_id, next_block_id) = block_graph(block_id, next_block_id) + abs(weight);
                        
                    case {'mean', 'mean_gaussian'}
                        %% only consider the mean vector
                        mean_vx = mean(block_vx(:));
                        mean_vy = mean(block_vy(:));
                        mean_vz = mean(block_vz(:));
                        block_vec = [mean_vy, mean_vx, mean_vz];
                        if( norm(block_vec) == 0.0 )
                            continue;
                        end
                        block_vec = block_vec / norm(block_vec);

                        block_weights = neighbor_dirs * block_vec';
                        directed_dir_inds = find(block_weights >= 0.0);  %cos(45)

                        for directed_dir_ind = directed_dir_inds'
                            next_block_sub = block_sub + sign(neighbor_dirs(directed_dir_ind, :));
                            if( next_block_sub(1) < 1 || next_block_sub(1) > n_blocks_per_dim(1) || ...
                                next_block_sub(2) < 1 || next_block_sub(2) > n_blocks_per_dim(2) || ...
                                next_block_sub(3) < 1 || next_block_sub(3) > n_blocks_per_dim(3) )
                                continue;
                            end

                            next_block_id = sub2ind(n_blocks_per_dim, next_block_sub(1), next_block_sub(2), next_block_sub(3));

                            weight = block_weights(directed_dir_ind);
                            if( true == strcmp(weight_computation, 'mean_gaussian') )
                                weight = normpdf(weight, 1, 0.1);
                            end
                            weight = max(weight, 0.0);
                            block_graph(block_id, next_block_id) = block_graph(block_id, next_block_id) + abs(weight);
                        end
                end
            end
        end
    end
end

%
% $Log: not supported by cvs2svn $
%
