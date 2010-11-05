function func_plot_nodes(node_bboxs, nodes)
    n_nodes = length(nodes);

    unit_faces_matrix = [...
        2, 1, 5, 6; % -y
        3, 4, 8, 7; % +y
        3, 7, 5, 1; % -x
        2, 6, 8, 4; % +x
        1, 2, 4, 3; % -z
        6, 5, 7 ,8; % +z
        ];
    n_faces_per_node = size(unit_faces_matrix, 1);
    n_vertex_per_node = 8;
    faces_matrix =  zeros(n_nodes * n_faces_per_node,     4);
    vertex_matrix = zeros(n_nodes * n_vertex_per_node,    3);
    
    for ni = 1:n_nodes
        bbox = node_bboxs(nodes(ni), :);
        bbox_low = bbox(1:3);
        bbox_top = bbox(1:3) + bbox(4:6);
        vi = 0;
        for nz = [bbox_low(3), bbox_top(3)]
            for nx = [bbox_low(2), bbox_top(2)]
                for ny = [bbox_low(1), bbox_top(1)]
                    vi = vi + 1;
                    vertex_matrix((ni - 1) * n_vertex_per_node + vi, :) = [nx, ny, nz];
                end
            end
        end

        faces_matrix(6*(ni-1)+(1:6), :) = unit_faces_matrix + (ni - 1) * n_vertex_per_node;
    end

    patch('Vertices', vertex_matrix, 'Faces', faces_matrix, 'FaceColor','none');
end

%
% $Log: not supported by cvs2svn $
%
