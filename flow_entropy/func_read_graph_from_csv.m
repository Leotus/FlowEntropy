function [flow_graph, n_nodes, n_edges, max_eweight] = func_read_graph_from_csv(graph_pathfilename)
    M = csvread(graph_pathfilename);
    n_nodes = M(1, 1);
    n_edges = M(1, 2);
    max_eweight = M(1, 3);
    M = M(2:end, :);
    flow_graph = sparse(n_nodes, n_nodes);
    for ei = 1:n_edges
        eu = M(ei, 1);
        ev = M(ei, 2);
        ew = M(ei, 3);
        flow_graph(eu, ev) = ew;
    end
end

%
% $Log: not supported by cvs2svn $
%
