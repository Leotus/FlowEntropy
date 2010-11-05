clear all;
clear java;

%% #dim for MDS to plot
mds_n_dims = 2;

%% a flag to decide whether the classic mds will be used
is_mds_classic = true;

%% a parameter to control the distance between non-connected point pairs
inf_dist_scale = 1.0;

%% specify the dataset
%     {'D:/data/nek', 'nek.d_4', [16, 16, 16]}, ...
vec_path_graphs = {...
    {'D:\data\flow_entropy\3D', 'source', [8, 8, 8]}, ...
    };

n_graphs = length(vec_path_graphs);

for gi = 1:n_graphs
    %% specify the path of the dataset
    % vec_path = 'D:/data/flow_entropy/3D';
    vec_path = vec_path_graphs{gi}{1};

    % vec_name = 'wrfout_d01_2007-10-01_18_00_00.t_1.d_1';
    vec_name = vec_path_graphs{gi}{2};

    %% load the graph
    % block_size = [30 30 9];
    block_size = vec_path_graphs{gi}{3};

    graph_name = sprintf('%s.block_graph.block_%d_%d_%d.weight_streamline', vec_name, block_size(1), block_size(2), block_size(3));

    script_plot_graph_by_mds;
end

