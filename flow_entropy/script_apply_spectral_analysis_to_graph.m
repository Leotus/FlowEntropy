% vec_path
% graph_name
% max_n_eigs

%% scan all vector fields
graph_pathfilename =    [vec_path, '/',    graph_name, '.csv'];

%% load the graph
flow_graph = func_read_graph_from_csv(graph_pathfilename);
valid_block_inds = find(sum(flow_graph + flow_graph', 2) > 0);
flow_graph = flow_graph(valid_block_inds, valid_block_inds);

[eig_vectors, eig_values] = func_comp_laplacian_eigen(flow_graph, max_n_eigs);

figure;
plot(eig_values);
axis tight;
title(regexprep(graph_name, '_', '\\_'));

%
% $Log: not supported by cvs2svn $
%