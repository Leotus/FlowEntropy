% is_ploting_weighted_edges = false;
% vec_path = 'D:\data\nek';
% vec_name = 'isabel.t_20.d_1.z_2';
% block_size = [8 8 1];
% is_converting_C_to_MATLAB

%% load the vector field
vec_pathfilename = [vec_path, '/', vec_name, '.vec'];
[vx, vy, vz, dim] = func_read_vec(vec_pathfilename);
[X,Y,Z] = meshgrid(1:dim(2), 1:dim(1), 1:dim(3));

%% load the graph
graph_name = [vec_name, '.', graph_name_postfix]; % sprintf(graph_nameformat, vec_name, block_size(1), block_size(2), block_size(3));
graph_pathfilename = [vec_path, '/', graph_name, '.csv'];
[flow_graph, n_nodes, n_edges, max_eweight] = func_read_graph_from_csv(graph_pathfilename);

max_eweight = prod(block_size);

if( true == is_converting_C_to_MATLAB )
    %% [TEST] convert the array from 0-base to 1-base 
    M(:, 1:2) = M(:, 1:2) + 1;
    M_temp = M;

    %% TEST: convert the file from XYZ order to YXZ order
    [M_j, M_i, M_k] = ind2sub(n_blocks_per_dim([2, 1, 3]), M(:, 1));
    M_temp(:, 1) = sub2ind(n_blocks_per_dim, M_i, M_j, M_k );
    [M_j, M_i, M_k] = ind2sub(n_blocks_per_dim([2, 1, 3]), M(:, 2));
    M_temp(:, 2) = sub2ind(n_blocks_per_dim, M_i, M_j, M_k );
    
    M = M_temp;
end

%% load the bbox of the nodes
bbox_pathfilename = [vec_path, '/', graph_name, '.bbox.csv'];
node_bboxs = csvread(bbox_pathfilename);

%% compute the centroids of all bboxes
node_centroids = node_bboxs(:, 1:3) + node_bboxs(:, 4:6)/2;

%% generate streamlines
[XYZ, XYZ_neg] = func_place_random_streamlines(vx, vy, vz, 64);
clear('vx', 'vy', 'vz', 'dim');

figure;
hold on;

%% plot the streamlines
streamline(XYZ);
streamline(XYZ_neg);

edge_inds = find(flow_graph);
[edge_is, edge_js] = ind2sub(size(flow_graph), edge_inds);

%% plot the graph nodes whose edge weight is larger than a threshold
eweight_threshold = 1.0;
node_flags = zeros(n_nodes, 1);
for ei = 1:n_edges
    edge_i = edge_is(ei);
    edge_j = edge_js(ei);
    ew = flow_graph(edge_i, edge_j);
    if( ew > max_eweight * eweight_threshold )
        node_flags(edge_i) = 1;
        node_flags(edge_j) = 1;
    end
end
nodes_to_be_plotted = find(node_flags);
if( ~isempty(nodes_to_be_plotted) )
    func_plot_nodes(node_bboxs, nodes_to_be_plotted);
end

if( ~is_ploting_weighted_edges )
    QCoord = zeros(n_edges, 3);
    QVec = zeros(n_edges, 3);
    for ei = 1:n_edges
        edge_i = edge_is(ei);
        edge_j = edge_js(ei);
        ew = flow_graph(edge_i, edge_j) / max_eweight;
        QCoord(ei, :) = node_centroids(edge_i, 1:3);
        QVec(ei, :) = (node_centroids(edge_j, 1:3) - node_centroids(edge_i, 1:3) ) * ew;
    end
    QInd = 1:n_edges; % find(QCoord(:, 3) <= max(SZ(:)));
    quiver3(QCoord(QInd, 2), QCoord(QInd, 1), QCoord(QInd, 3), QVec(QInd, 2), QVec(QInd, 1), QVec(QInd, 3),  0, '-r', 'filled');
else
    %% compute the weighded average vector on each block
    QVec = zeros(n_nodes, 3);
    for ei = 1:n_edges
        ni = edge_is(ei);
        nj = edge_js(ei);
        ew = flow_graph(ni, nj);
        edge_vec = node_centroids(nj, :) - node_centroids(ni, :);
        QVec(ni, :) = QVec(ni, :) + edge_vec * ew;
    end
    QInd = 1:n_nodes; % find(node_centroids(:, 3) <= max(SZ(:)));
    quiver3(node_centroids(QInd, 2), node_centroids(QInd, 1), node_centroids(QInd, 3), QVec(QInd, 2), QVec(QInd, 1), QVec(QInd, 3),  '-r', 'filled');
end

view(2);

axis equal;
axis tight;

%
% $Log: not supported by cvs2svn $
%
