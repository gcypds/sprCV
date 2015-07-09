function save_c_matrix(arrays_info, libname)

    array, desired_type, desired_name, desired_format
	filename = strcat(upper(desired_name), '_data');
	h_filename = sprintf('%s.h', filename);
	cpp_filename = sprintf('%s.cpp', filename);
	
	fd=fopen(h_filename,'wt');
		
    number_of_dimensions = ndims(array);
    
    dimensions_name = sprintf('%s_DIMENSIONS_COUNT', upper(desired_name));
    dimensions_array_name = sprintf('const int %s_dimensions[%s]', upper(desired_name), dimensions_name);
    
    fprintf(fd, '#define %s %d\n\n', dimensions_name, number_of_dimensions);
    fprintf(fd, 'extern %s;\n', dimensions_array_name);
    
	array_name = sprintf('const %s %s', desired_type, desired_name);
	
    array_size = size(array);
    
    for i=1:number_of_dimensions
        array_name = strcat(array_name, sprintf('[%d]', array_size(i)));
    end
    
	fprintf(fd, sprintf('extern %s;', array_name));		
	fclose(fd);

	fd=fopen(cpp_filename,'wt');
	fprintf(fd,'#include "%s"\n', h_filename);
        	
    % Array de dimensiones
    fprintf(fd, '%s = {\n %d', dimensions_array_name , array_size(1));
    
    fprintf(fd, ',\n %d', array_size(2:end));
    
	fprintf(fd,'\n};\n\n');
    
    % Array de datos
    fprintf(fd, '%s ={\n', array_name);
    
    for i=1:array_size(1),
        fprintf(fd, strcat('\n{\n', desired_format), array(i,1));
        
        fprintf(fd, strcat(',', desired_format), array(i,2:end));

        fprintf(fd,'\n}\n');
        
        if(i < array_size(1))
            fprintf(fd,'\n,\n');
        end        
    end
    fprintf(fd,'\n};\n');
    
	fclose(fd);
end