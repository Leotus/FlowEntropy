clear all;
clear java;

%     {'wrfout_d01_2007-10-01_18_00_00.t_1.d_2', 'block_graph.block_30_30_1.weight_streamline'}, ...
%     {'nek.d_4', 'block_graph.block_8_8_8.weight_streamline'}, ...
%     {'D:/data/flow_entropy/3D', 'circle', 'block_graph.block_4_4_4.weight_streamline'}, ...
%     {'D:/data/flow_entropy/3D', 'source', 'block_graph.block_4_4_4.weight_streamline'}, ...
%     {'D:\data\nek', 'nek.d_4', 'block_graph.block_8_8_8.weight_streamline'}, ...
%     {'D:/data/flow_entropy/3D', 'sink', 'block_graph.block_4_4_4.weight_streamline'}, ...
%     {'D:/data/flow_entropy/3D', 'equation', 'block_graph.block_4_4_4.weight_streamline'}, ...
%     {'D:/data/flow_entropy/3D', 'electro', 'block_graph.block_4_4_4.weight_streamline'}, ...
%     {'D:\data\pnnl\smhagos', 'wrfout_d01_2007-10-01_18_00_00.t_1.d_1', 'block_graph.block_90_90_1.weight_streamline'}, ...

vec_graphs = ...
{
    {'D:/data/flow_entropy/3D', 'source', 'block_graph.block_8_8_8.weight_streamline'}, ...
};

n_vec_graphs = length(vec_graphs);

ns_parts = 2.^(2:3);

is_ploting_figures = true;

%% specify the clustering algorithm
mapping_algorithm = 'spectral_clustering';
% mapping_algorithm = 'diffusion_map';

switch mapping_algorithm
    case 'diffusion_map'
        %% parameters for diffusion map
        cluster_dist = 'single';

        %% clustering methods for diffusion map
        % clustering_method = 'kmeans';
        clustering_method = 'recursive_hierarchical_even';
        % clustering_method = 'hierarchical_even';
        % clustering_method = 'hierarchical';
end

%% scan all vector fields
for vgi = 1:n_vec_graphs
    vec_path = vec_graphs{vgi}{1};
    vec_name = vec_graphs{vgi}{2};
    graph_name = [vec_name '.' vec_graphs{vgi}{3}];
    
    for manual_n_parts = ns_parts
        %% decide the default #rows to show the clusters
        default_n_rows = ceil(sqrt(manual_n_parts));
    
        script_partition_graph;
    end
end

