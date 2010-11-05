% vec_path
% vec_name
% graph_name
% mds_n_dims
% is_mds_classic = true;
% inf_dist_scale = 100000.0;


graph_pathfilename = [vec_path, '/', graph_name, '.csv'];
[flow_graph, n_nodes, n_edges, max_eweight] = func_read_graph_from_csv(graph_pathfilename);

%% load the bbox of the nodes
bbox_pathfilename = [vec_path, '/', graph_name, '.bbox.csv'];
node_bboxs = csvread(bbox_pathfilename);

%% compute the centroids of all bboxes
node_centroids_3d = node_bboxs(:, 1:3) + node_bboxs(:, 4:6)/2;
M_geometry_dist = squareform(pdist(node_centroids_3d));
clear('node_centroids_3d');

%% apply MDS and take the first 2 components as the projected vector
Y_geom = cmdscale(M_geometry_dist);
Y_geom = Y_geom(:, 1:mds_n_dims) + rand(n_nodes, mds_n_dims) - 0.5;

%% map the edge weight to probability
M = flow_graph;
M_inds = find(M);
M(M_inds) = -log(M(M_inds)/max_eweight); 

%% compute the all-pair shortest distance as the geodestic distance
M_geodestic_dist = graphallshortestpaths(M);

%% combine both Euclidean distance and Geodestic distance
inf_inds =          find(  isinf(M_geodestic_dist) );
noninf_inds =       find( ~isinf(M_geodestic_dist) );
max_noninf_geod_dist = max(M_geodestic_dist(noninf_inds)) * inf_dist_scale;

M_dist = M_geometry_dist .* M_geodestic_dist;
M_dist(inf_inds) = max_noninf_geod_dist * M_geometry_dist(inf_inds);
M_dist = min(M_dist, M_dist');

%% weighted by the Euclidean distance
clear('M_geometry_dist');
clear('M_geodestic_dist');
clear('M');

%% apply MDS and take the first 2 components as the projected vector
if( is_mds_classic )
	Y = cmdscale(M_dist);
else
	Y = mdscale(M_dist, mds_n_dims, 'Start', Y_geom);
end
Y = Y(:, 1:mds_n_dims);

edge_inds = find(flow_graph);
[edge_is, edge_js] = ind2sub(size(flow_graph), edge_inds);

%% begin to plot the graphs
figure;
for fi = 2; % 1:2
%     subplot(1, 2, fi);
    hold on;

    mesh_points = zeros(n_edges, mds_n_dims);
    mesh_lines = zeros(n_edges, mds_n_dims);
    mesh_edges = zeros(n_edges, mds_n_dims);

    switch fi
        case 1
            node_centroids = Y_geom; % 
        case 2
            node_centroids = Y; % 
    end
    
    for ei = 1:n_edges
        edge_i = edge_is(ei);
        mesh_points(ei, :) = node_centroids(edge_i, :);

        edge_j = edge_js(ei);
        edge_dir = (node_centroids(edge_j, :) - node_centroids(edge_i, :) );
        mesh_lines(ei, :) = edge_dir;

        ew = flow_graph(edge_i, edge_j)/max_eweight;
        mesh_edges(ei, :) = edge_dir * ew;
    end
    switch mds_n_dims
        case 2
            h_line = quiver(mesh_points(:, 2), mesh_points(:, 1), mesh_lines(:, 2), mesh_lines(:, 1), 0, ':b', 'Marker', 'o');
            h_edge = quiver(mesh_points(:, 2), mesh_points(:, 1), mesh_edges(:, 2), mesh_edges(:, 1), 0, '-r', 'LineWidth', 2.0);

        case 3
            h_line = quiver3(mesh_points(:, 2), mesh_points(:, 1), mesh_points(:, 3), mesh_lines(:, 2), mesh_lines(:, 1), mesh_lines(:, 3), 0, ':b', 'Marker', 'o');
            h_edge = quiver3(mesh_points(:, 2), mesh_points(:, 1), mesh_points(:, 3), mesh_edges(:, 2), mesh_edges(:, 1), mesh_edges(:, 3), 0, '-r', 'LineWidth', 2.0);
    end
    set(h_line, 'ShowArrowHead', 'off');
    set(h_edge, 'ShowArrowHead', 'off');

    view(mds_n_dims);
    title(regexprep(graph_name, '_', '\\_'));
    axis equal;
    axis tight;
end

%
% $Log: not supported by cvs2svn $
%
