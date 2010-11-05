% Pre-requisite: vec_path 
% Pre-requisite: vec_name
% Pre-requisite: block_size
% Pre-requisite: graph_type 

switch graph_type 
    case 'block'
        graph_fileprefix = sprintf('%s.block_graph.block_%d_%d_%d.weight_%s', vec_name, block_size(1), block_size(2), block_size(3), weight_computation);

    case 'face'
        graph_fileprefix = sprintf('%s.face_graph.block_%d_%d_%d', vec_name, block_size(1), block_size(2), block_size(3));
end
graph_filename = [graph_fileprefix, '.csv'];
bbox_filename = [graph_fileprefix, '.bbox.csv'];

vec_filename = [vec_name, '.vec'];
vec_pathfilename = [vec_path, '/', vec_filename];

[vx, vy, vz, dim] = func_read_vec(vec_pathfilename);

%% normalize the vector field
vec_mag = sqrt(vx .* vx + vy .* vy + vz .* vz);
vx = vx ./ vec_mag;
vy = vy ./ vec_mag;
vz = vz ./ vec_mag;

%% compute the #blocks per dimension
n_blocks_per_dim = ceil(dim ./ block_size);
n_blocks = prod(n_blocks_per_dim);

%% initialize the gragh
[bx, by, bz] = meshgrid(1:n_blocks_per_dim(2), 1:n_blocks_per_dim(1), 1:n_blocks_per_dim(3));

%% build the graph
tic
switch graph_type 
    case 'block';
        flow_graph = func_build_block_based_graph(vx, vy, vz, block_size, weight_computation);
        node_bboxs = func_comp_block_based_node_bboxs(dim, block_size);

    case 'face'
        % build the flow graph
        flow_graph = func_build_face_based_graph(vx, vy, vz, block_size, adj_face_weight);
        node_bboxs = func_comp_face_based_node_bboxs(dim, block_size);

        if( true == are_adj_faces_the_same_vertex )
            % find the mapping from (block, face) to vertices in the new
            % flow graph
            [face_mapping, n_used_faces] = func_build_face_mapping(n_blocks, n_blocks_per_dim);

            %% map the edges in the original flow graph to form the new graph
            new_flow_graph = sparse(n_used_faces, n_used_faces);
            edge_inds = find(flow_graph);
            [edge_us, edge_vs] = ind2sub(size(flow_graph), edge_inds);
            n_edges = length(edge_inds); 
            for ei = 1:n_edges
                new_u = face_mapping(edge_us(ei));
                new_v = face_mapping(edge_vs(ei));
                new_flow_graph(new_u, new_v) = new_flow_graph(new_u, new_v) + flow_graph(edge_us(ei), edge_vs(ei));
            end
            flow_graph = new_flow_graph;

            %% merge the bboxs of the adjact faces
            new_node_bboxs = zeros(n_used_faces, n_faces_per_block);
            new_node_bboxs(:, 1:3) = inf;
            n_faces = size(node_bboxs, 1);
            for fi = 1:n_faces
                [block_face, bi] = ind2sub([n_faces_per_block, n_blocks], fi);
                new_fi = face_mapping(bi, block_face);
                switch block_face
                    case {1, 2}
                        new_node_bboxs(new_fi, 1) = min(new_node_bboxs(new_fi, 1), node_bboxs(fi, 1));
                        new_node_bboxs(new_fi, [2 3]) = node_bboxs(fi, [2 3]);
                        new_node_bboxs(new_fi, 4) = new_node_bboxs(new_fi, 4) + node_bboxs(fi, 4);
                        new_node_bboxs(new_fi, [5 6]) = node_bboxs(fi, [5 6]);
                    case {3, 4}
                        new_node_bboxs(new_fi, 2) = min(new_node_bboxs(new_fi, 2), node_bboxs(fi, 2));
                        new_node_bboxs(new_fi, [1 3]) = node_bboxs(fi, [1 3]);
                        new_node_bboxs(new_fi, 5) = new_node_bboxs(new_fi, 5) + node_bboxs(fi, 5);
                        new_node_bboxs(new_fi, [4 6]) = node_bboxs(fi, [4 6]);
                    case {5, 6}
                        new_node_bboxs(new_fi, 3) = min(new_node_bboxs(new_fi, 3), node_bboxs(fi, 3));
                        new_node_bboxs(new_fi, [1 2]) = node_bboxs(fi, [1 2]);
                        new_node_bboxs(new_fi, 6) = new_node_bboxs(new_fi, 6) + node_bboxs(fi, 6);
                        new_node_bboxs(new_fi, [4 5]) = node_bboxs(fi, [4 5]);
                end
            end
            node_bboxs = new_node_bboxs;

%                 figure;
%                 func_plot_nodes(node_bboxs, 1:10);
        end
end
toc

%% save the graph
fid = fopen([vec_path, '/', graph_filename], 'wt');
n_graph_nodes = size(flow_graph, 1);
fprintf(fid, '%d,%d,%d,\n', n_graph_nodes, length(find(flow_graph)), full(max(flow_graph(:))));
edge_inds = find(flow_graph);
n_edge = length(edge_inds);
[edge_ys, edge_xs] = ind2sub(size(flow_graph), edge_inds);
for ei = 1:n_edge
    edge_weight = full(flow_graph(edge_ys(ei), edge_xs(ei)));
    fprintf(fid, '%d,%d,%f,\n', edge_ys(ei), edge_xs(ei), edge_weight);
end
fclose(fid);

%% save the bbox of all nodes
csvwrite([vec_path, '/', bbox_filename], node_bboxs);

block_sinked_weight = prod(block_size) - sum(flow_graph, 2);

[internal_block_xs, internal_block_ys, internal_block_zs] = meshgrid(2:n_blocks_per_dim(2)-1, 2:n_blocks_per_dim(1)-1, 2:n_blocks_per_dim(3)-1);
internal_block_inds = sub2ind(n_blocks_per_dim, internal_block_ys(:), internal_block_xs(:), internal_block_zs(:));
[sorted_sinked_weight, sorted_block_inds] = sort(block_sinked_weight(internal_block_inds));
sorted_block_inds = sorted_block_inds(end:-1:1);
figure;
hist(block_sinked_weight(internal_block_inds));
title(regexprep(graph_filename, '_', '\\_'));

%
% $Log: not supported by cvs2svn $
%
