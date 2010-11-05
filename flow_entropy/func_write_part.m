%% vec_name: name of the vector field
function func_write_part(part_pathfilename, input_name, part_blocks)
    fp = fopen(part_pathfilename, 'wt');
    
    fprintf(fp, '%s\n', input_name);
    
    n_parts = length(part_blocks);
	fprintf(fp, '%d\n', n_parts);
    for parti = 1:n_parts
        fprintf(fp, '%d,%d\n', parti, length(part_blocks{parti}));
        fprintf(fp, '%d,', part_blocks{parti});
        fprintf(fp, '\n');
    end
    fclose(fp);
end

%
% $Log: not supported by cvs2svn $
%
