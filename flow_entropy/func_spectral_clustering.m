function [vert_comp_inds, used_eig_vectors, used_eig_vector_levels, n_classified_comps] = func_spectral_clustering(graph_matrix, begin_comp_ind, n_comps, max_cluster_size)
	n_verts = size(graph_matrix, 1);
	vert_comp_inds = zeros(n_verts, 1);

    if( 1 >= n_comps || n_verts < max_cluster_size )
        vert_comp_inds(:) = begin_comp_ind;
        used_eig_vectors = zeros(1, n_verts);
        used_eig_vectors(:) = NaN;
        used_eig_vector_levels = 1;
        n_classified_comps = n_comps;
        return;
    end

    max_n_eigs = 6;
    %% find the smallest eigenvalues/eigenvectors of the Laplacian
    %% matrix
    [V, eigen_values] = func_comp_laplacian_eigen(graph_matrix, max_n_eigs);
    sorted_inds = 1:length(eigen_values);

    if( false ) % TEST-MOD-FROM:
        n_eigen_zeros = max(find(eigen_values<0.0001));
    else        % TEST-MOD-TO: 
        n_eigen_zeros = max(find(eigen_values<0.02));
    end         % TEST-MOD-END
    vert_labels = zeros(n_verts, 1);
    if(1==n_eigen_zeros)
        vert_labels(:) = V(:,n_eigen_zeros+1);
        vert_label_median = median(vert_labels(:));
        vert_labels = sign(vert_labels - vert_label_median);

        used_eig_vectors = V(:, n_eigen_zeros+1)';
        used_eig_vector_levels = 1;
    else
        for ei = 1:n_eigen_zeros
            eigen_vector = V(:, ei);
            vert_labels = vert_labels * 3 + sign(eigen_vector) + 1;

            [b, m, n] = unique(vert_labels);
            n_hist = hist(n, length(b));
            if(length(find(n_hist > max_cluster_size)) > n_comps)
                break;
            end
        end
        used_eig_vectors = V(:, 1:ei)';
        used_eig_vector_levels = ones(ei, 1);
    end

    %% ignore the components w/ too small sizes
    unique_vert_labels = unique(vert_labels);
    n_labels = length(unique_vert_labels);
    n_classified_comps = 0;
    comp_ind = begin_comp_ind;
    for li = 1:n_labels
        verts_w_label = find(vert_labels == unique_vert_labels(li));
        n_vert_w_labels = length(verts_w_label);
        
        if( li < n_labels )
            if( n_vert_w_labels < max_cluster_size )
                expected_n_comps  = 0;
            else
                if( true ) % TEST-MOD-FROM:
                    expected_n_comps = round(n_comps * n_vert_w_labels / n_verts);
                else        % TEST-MOD-TO:
                    expected_n_comps = 1;
                end         % TEST-MOD-END
            end
        else
            expected_n_comps = n_comps - n_classified_comps;
        end
        
        [classified_vert_comp_ind, sub_used_eig_vectors, sub_used_eig_vector_levels, n_new_classified_comps] = func_spectral_clustering(graph_matrix(verts_w_label, verts_w_label), comp_ind, expected_n_comps, max_cluster_size);
        vert_comp_inds(verts_w_label) = classified_vert_comp_ind;
        comp_ind = max(classified_vert_comp_ind)+1;

        if( expected_n_comps > 0 )
            n_classified_comps = n_classified_comps + n_new_classified_comps; % length(unique(classified_vert_comp_ind));
            
            tmp_used_eig_vectors = zeros(size(sub_used_eig_vectors, 1), n_verts);
            tmp_used_eig_vectors(:) = NaN;
            tmp_used_eig_vectors(:, verts_w_label) = sub_used_eig_vectors;
            used_eig_vectors = [used_eig_vectors; tmp_used_eig_vectors];
            used_eig_vector_levels = [used_eig_vector_levels; sub_used_eig_vector_levels+1];
        end
    end
end

%
% $Log: not supported by cvs2svn $
%
