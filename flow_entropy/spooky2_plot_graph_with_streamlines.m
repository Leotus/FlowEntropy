clear all;
clear java;

is_ploting_weighted_edges = true;
is_converting_C_to_MATLAB = false;

vec_graphs = {...
    {'D:\data\flow_entropy\3D', 'source', 'block_graph.block_8_8_8.weight_streamline', [8, 8, 8]},...
    };

n_graphs = length(vec_graphs);

for gi = 1:n_graphs
    vec_path = vec_graphs{gi}{1};
    vec_name = vec_graphs{gi}{2};
    graph_name_postfix = vec_graphs{gi}{3};
	block_size = vec_graphs{gi}{4};

    script_plot_graph_with_streamlines;
end

