% vec_path = 'D:/data/flow_entropy/3D';
% vec_name
% graph_name
% ns_parts = 4; % 2.^(1:5);
% default_n_rows = 2;
% mapping_algorithm = 'spectral_clustering';
% is_ploting_figures

% #eig values per iteration
max_cluster_size = 0;
max_n_eigs = 6; 

% switch mapping_algorithm
%     case 'diffusion_map'
%         %% parameters for diffusion map
%         cluster_dist = 'single';
% 
%         %% clustering methods for diffusion map
%         % clustering_method = 'kmeans';
%         clustering_method = 'recursive_hierarchical_even';
%         % clustering_method = 'hierarchical_even';
%         % clustering_method = 'hierarchical';
% end

graph_pathfilename =    [vec_path, '/',    graph_name, '.csv'];
bbox_pathfilename =     [vec_path, '/',    graph_name, '.bbox.csv'];

%% load the graph
flow_graph = func_read_graph_from_csv(graph_pathfilename);
valid_block_inds = find(sum(flow_graph + flow_graph', 2) > 0);
flow_graph = flow_graph(valid_block_inds, valid_block_inds);

%% load the bounding box of all nodes
node_bboxs = csvread(bbox_pathfilename);

%% load the vector field and generate streamlines 
[vx, vy, vz, dim] = func_read_vec([vec_path, '/', vec_name, '.vec']);
[XYZ, XYZ_neg] = func_place_random_streamlines(vx, vy, vz, 64);
clear('vx', 'vy', 'vz', 'dim');

%% decide the max. bounding box
global_bbox = [min(node_bboxs(:, 1:3)), max(node_bboxs(:, 1:3)+node_bboxs(:, 4:6))];

switch mapping_algorithm
    case 'spectral_clustering'
        %% apply spectral clustering
        [block_labels, used_eig_vectors, used_eig_vector_levels, n_classified_comps] = func_spectral_clustering(flow_graph, 1, manual_n_parts, max_cluster_size);

        unique_levels = unique(used_eig_vector_levels);
        n_levels = length(unique_levels);
        if( is_ploting_figures )
            figure;
            set(gca, 'FontSize', 20);
            legend_strs = cell(0, 0);
            for li = 1:n_levels-1
                level = unique_levels(li);
                subplot(n_levels-1, 1, li);
                plot(1:length(block_labels), used_eig_vectors(find(level == used_eig_vector_levels), :)');
                axis tight;
                n_eig_vectors = length(find(level == used_eig_vector_levels));
                for ei = 1:n_eig_vectors
                    legend_strs{ei} = sprintf('Eigen vector %d', ei);
                end
                title(sprintf('%s: Used Eigen Vectots in level %d', regexprep(graph_name, '_', '\\_'), li));
                legend(legend_strs, 'Location', 'SouthWest');
                xlabel('Block ID');
            end
        end
    case 'diffusion_map'
        [eigen_vectors, eigen_values] = func_diffusion_map(flow_graph, max_n_eigs);

        node_ignore_flags = zeros(size(eigen_vectors, 1), 1);
        for ei = 1:max_n_eigs
            %% check the number of isolated components
            pos_ratio = sum(eigen_vectors(:, ei)>0)/size(eigen_vectors, 1);
            if( pos_ratio < 0.05 || pos_ratio > 0.95 )
                if( pos_ratio < 0.05 )
                    node_ignore_flags(find(eigen_vectors(:, ei)>0)) = true;
                else
                    node_ignore_flags(find(eigen_vectors(:, ei)<0)) = true;
                end
            else
                break;
            end
        end
        min_eig_index = ei; % min((find(eigen_values < 0.95)));

        non_ignored_node_inds = find(~node_ignore_flags);
        flow_graph = flow_graph(non_ignored_node_inds, non_ignored_node_inds);
        eigen_vectors = eigen_vectors(non_ignored_node_inds, :);
        valid_block_inds = valid_block_inds(non_ignored_node_inds);

        for n_times = prod(n_blocks_per_dim(:));
        % decide the time steps
%             for n_times = min(min(n_blocks_per_dim), max_n_times) % 2:2:min(min(n_blocks_per_dim), max_n_times)
%             for n_times = 4:4:min(sum(n_blocks_per_dim), max_n_times)

%                 n_eigs = max(find(eigen_values .^ n_times > 0.5)) - min_eig_index + 1;
%                 eig_inds = min_eig_index - 1 + (1:n_eigs);\
            n_connected_components = max(find(eigen_values > 0.9999)) - min_eig_index;
            if( n_connected_components <= 0 )
                n_connected_components = 1;
            end
            max_eig_index = max(find(eigen_values .^ n_times > 0.5));
            eig_inds = min_eig_index:max_eig_index;
            if( length(eig_inds)<3 )
                eig_inds = min_eig_index+(0:3);
            end

            %% plot the points in the new space
            figure;

            subplot(2, 2, 1);
            plot(1:max_n_eigs, eigen_values);
            xlim([1, max_n_eigs]);
            ylim([0.0, 1.0]);
            title(regexprep(sprintf('Eigenvalues (%s, #T=%d,%s-based)', vec_name, n_times, graph_type), '_', '\\_'));

            %% apply hierarchical clustering
            eigen_vector_weights = eigen_values .^ n_times;
            mapped_coords = eigen_vectors * diag(eigen_vector_weights);
            mapped_coords = mapped_coords(:, eig_inds);
            node_dists = pdist(mapped_coords);
            cluster_tree = linkage(node_dists, cluster_dist);

            subplot(2, 2, 2);
            func_plot_graph_in_space(flow_graph, 1:size(flow_graph,1), mapped_coords, 0); % max(flow_graph(:))/2)

            axis equal;
            xlim([min(mapped_coords(:, 1)), max(mapped_coords(:, 1))]);
            ylim([min(mapped_coords(:, 2)), max(mapped_coords(:, 2))]);
            zlim([min(mapped_coords(:, 3)), max(mapped_coords(:, 3))]);

            view(3);    % [90, 0]);
            title('Scatter Plot');

            %% plot the dendrogram
            subplot(2, 2, 3);
            dendrogram(cluster_tree, 0);
            ylim([0.0, max(cluster_tree(:, 3))]);
            title('Dendrogram');

            %% plot the parallel coordinates of the mapped coordinates
            subplot(2, 2, 4);
            plot(eig_inds, mapped_coords');
            axis tight;
            ylim([min(mapped_coords(:)), max(mapped_coords(:))]);
            title('Parallel Coordinates');
        end % n_times

        %% apply K-mean clustering on the space spanned by the eigen
        % vectors of diffusion map
        if( manual_n_parts > 0 )
            n_components = manual_n_parts;
        elseif( 1 == n_connected_components )
            n_components = 2;
        else
            n_components = n_connected_components;
        end

        switch clustering_method 
            case 'kmeans'
                block_labels = kmeans(mapped_coords, n_components);

            case 'hierarchical'
                if( n_connected_components > 1)
                    block_labels = cluster(cluster_tree, 'maxclust', n_components);
                else
                    block_labels = func_select_even_clusters(cluster_tree, n_components);
                end

            case 'hierarchical_even'
                block_labels = func_select_even_clusters(cluster_tree, n_components);

            case 'recursive_hierarchical_even'
                block_labels = zeros(size(mapped_coords, 1), 1);
                for ci = n_components:-1:2
                    selected_inds = find(~block_labels);    % only consider the blocks that haven't been selected
                    selected_mapped_coords = mapped_coords(selected_inds, :);

                    node_dists = pdist(selected_mapped_coords);
                    cluster_tree = linkage(node_dists, cluster_dist);
                    tmp_block_labels = func_select_even_clusters(cluster_tree, ci);

                    cluster_block_inds = selected_inds(find(tmp_block_labels==1));
                    block_labels(cluster_block_inds) = ci;
                end
        end

end

%% divide the blocks into subset of the same ID
labels = sort(unique(block_labels), 'ascend');
n_labels = length(labels);

cluster_blocks = cell(1, n_labels);
large_cluster_inds = [];
for li = 1:n_labels
    l = labels(li);
    cluster_block_inds = find(block_labels == l);
    if( length(cluster_block_inds) > 6 )
        large_cluster_inds(end+1) = li;
    end
    cluster_blocks{li} = cluster_block_inds;
end
n_large_clusters = length(large_cluster_inds);

%% save the partition
part_filename = sprintf('%s.%s.part_%d', graph_name, mapping_algorithm, n_labels);
switch mapping_algorithm
    case 'diffusion_map'
        part_filename = [part_filename '.' clustering_method];
end
part_filename = [part_filename '.txt']
part_pathfilename = [vec_path, '/', part_filename];
func_write_part(part_pathfilename, graph_name, cluster_blocks);

%% plot the clusters in the new space
n_rows = min(ceil(n_large_clusters / default_n_rows), default_n_rows);
switch mapping_algorithm
    case 'diffusion_map'
        %% plot the scatter plots of the mapped coordinates
        figure;
        for li = 1:n_labels
            blocks_w_label = cluster_blocks{li}; % find(block_labels == labels(li));
            subplot(n_rows, ceil(n_labels/n_rows), li);
%                     plot3(mapped_coords(blocks_w_label, 1), mapped_coords(blocks_w_label, 2), mapped_coords(blocks_w_label, 3), '*');

            func_plot_graph_in_space(flow_graph, blocks_w_label, mapped_coords, max(flow_graph(:))/2)

            axis equal;
            xlim([min(mapped_coords(:, 1)), max(mapped_coords(:, 1))]);
            ylim([min(mapped_coords(:, 2)), max(mapped_coords(:, 2))]);
            zlim([min(mapped_coords(:, 3)), max(mapped_coords(:, 3))]);
            view(3); % [90, 0]);
            title('Scatter Plot');
        end

        %% plot the mapped coordinates in parallel coordinates plot
        figure;
        for li = 1:n_labels
            blocks_w_label = cluster_blocks{li}; % find(block_labels == labels(li));
            subplot(n_rows, ceil(n_labels/n_rows), li);
            plot(mapped_coords(blocks_w_label, :)', '-b');
            axis tight;
            ylim([min(mapped_coords(:)), max(mapped_coords(:))]);
            title('Parallel Coordinates');
        end
end

%% plot the clusters in the spatail domain
if( is_ploting_figures )
    figure;
    for lli = 1:n_large_clusters
        subplot(n_rows, ceil(n_large_clusters/n_rows), lli);
        hold on;

        li = large_cluster_inds(lli);

        blocks_w_label = cluster_blocks{li}; 
        blocks_w_label = valid_block_inds(blocks_w_label);

        if( isempty(blocks_w_label) )
            continue;
        end

        %% plot the nodes with the current label
        func_plot_nodes(node_bboxs, blocks_w_label);

        %% plot the streamlines
        streamline(XYZ);
        streamline(XYZ_neg);

        axis equal
        xlim([global_bbox(2) global_bbox(5)]);
        ylim([global_bbox(1) global_bbox(4)]);
        zlim([global_bbox(3) global_bbox(6)]);
        view(3);
        title(sprintf('|B| = %d', length(blocks_w_label)));
    end
end

%
% $Log: not supported by cvs2svn $
%