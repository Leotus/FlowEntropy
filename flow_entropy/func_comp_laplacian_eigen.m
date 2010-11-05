function [eig_vectors, eig_values] = func_comp_laplacian_eigen(graph_matrix, max_n_eigs)
	n_verts = size(graph_matrix, 1);
	node_labels = zeros(n_verts, 1);

    %% compute the Laplacian matrix and the eigenvalues
    adjacency_matrix = graph_matrix;

    symetric_matrix = adjacency_matrix + adjacency_matrix';

    node_degree = sum(symetric_matrix, 2);
    laplacian_matrix = diag(node_degree) - symetric_matrix;

    n_eigs = min(n_verts, max_n_eigs);
    options.disp = 0;
    flag = true;
    n_estimating_eigs = n_eigs;
    while( flag )
        [V, D, flag] = eigs(laplacian_matrix, n_estimating_eigs, 'sa', options);
        if( 0 ~= flag)
%             n_estimating_eigs = n_estimating_eigs + n_eigs;
            n_estimating_eigs = n_estimating_eigs + n_estimating_eigs;
            warning('Unstable eigenvalues. The #eigs is increased to %d', n_estimating_eigs);
        end
    end
    [eig_values, sorted_inds] = sort(diag(D), 'ascend');
    eig_vectors = V;
    
    eig_values = eig_values(1:n_eigs);
    eig_vectors = eig_vectors(:, 1:n_eigs);
end

%
% $Log: not supported by cvs2svn $
%
