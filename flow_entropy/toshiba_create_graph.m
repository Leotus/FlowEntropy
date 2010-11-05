% this script can create the flow graphs for the specified dataset(s).
clear all;
clear java;

vec_path = 'D:/projects/data/flow_entropy/3D';

    'source', ...
	'sink', ...
	'circle', ...
	'saddle', ...
	'electro', ...
	'equation', ...
	'wrfout_d01_2007-10-01_18_00_00.t_1.d_1', ...
    ;

% 	'circle', ...
% 	'saddle', ...
% 	'electro', ...
% 	'equation', ...
% 	'sink', ...
vec_names = {...
	'source', ...
	'circle', ...
    };

n_vecs = length(vec_names);

%% block sizes
% block_size = [32, 32, 32];
% block_size = [16, 16, 16];
block_size = [8, 8, 8];
% block_size = [5, 5, 5];
% block_size = [90, 90, 1];
% block_size = [30, 30, 9];
% block_size = [4, 4, 4];
% block_size = [2, 2, 2];
% block_size = [1, 1, 1];

%% specify the type of the graph
% graph_type = 'face';
graph_type = 'block';

switch graph_type 
    case 'block'
        %% options for block-based graph
        
        %% specify the algorithm to decide the edge weight
        weight_computation = 'streamline';
        % weight_computation = 'boundary';
%         weight_computation = 'mean';
        % weight_computation = 'mean_gaussian';

    case 'face'
        n_faces_per_block = 6;
        
        %         are_adj_faces_the_same_vertex = false;
        are_adj_faces_the_same_vertex = true;
        
        if( true == are_adj_faces_the_same_vertex )
            adj_face_weight = 'zero';
        else
            adj_face_weight = 'sum_dot_prod';
            % adj_face_weight = 'zero';
        end
end

%% scan all vector fields
for veci = 1:n_vecs
    %% load the dataset
    vec_name = vec_names{veci};

    script_create_graph;
end

