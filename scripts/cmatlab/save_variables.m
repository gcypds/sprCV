% Define library name
library_name = 'bgs';

% Visual Studio 2010 Path
vs2010_path = 'C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC';

% bin2coff Path
bin2coff_path = 'G:/data/sebastianvilla139/work/survmantics/cmatlab/bin2coff';

% Select 64 bit support. 1 for true, 0 for false.
x64 = 0;

% Create library folder
if(~exist(library_name, 'dir'))
    mkdir(library_name);
end

% Init var_info list.
global vars_infos;

% Clean var_info list.
vars_infos = {};

% W
add_var_info(W, 'float', 'Mat_W');

% SupportVectors
add_var_info(svms{1,1}.SupportVectors, 'float', 'SupportVectors');

% Alpha
add_var_info(svms{1,1}.Alpha, 'float', 'alpha');

% Bias
add_var_info(svms{1,1}.Bias, 'float', 'bias');

% Sigma
add_var_info(sXrn, 'float', 'sigma');

disp(vars_infos);

% Set VS env vars
set_vcvars_command = sprintf('"%s/vcvarsall.bat"', vs2010_path);

if(x64 == 1)
    set_vcvars_command = sprintf('%s %s', set_vcvars_command, 'x86_amd64');
else
    set_vcvars_command = sprintf('%s %s', set_vcvars_command, 'x86');
end

disp(set_vcvars_command);

% Create create_libs.bat
create_libs_filename = sprintf('create_%s_lib.bat', library_name);
fileID_create_libs = fopen(create_libs_filename,'wt');
fprintf(fileID_create_libs, 'call %s\n', set_vcvars_command);

% lib_command = sprintf('"%s/bin/lib.exe" /out:%s/%s.lib',vs2010_path, library_name, library_name);
lib_command = sprintf('lib.exe /out:%s/%s.lib', library_name, library_name);

% Creacion de objetos
for k=1:length(vars_infos)
    var_filename = strcat(library_name, '/', vars_infos(k).desired_name);
    var_filename_bin = strcat(var_filename, '.bin');
    var_filename_obj = strcat(var_filename, '.obj');
%     disp(var_filename);    
    fileID = fopen(var_filename_bin,'w+');
    fwrite(fileID, vars_infos(k).var, vars_infos(k).desired_type);
    fclose(fileID);
    
    bin2coff_command = sprintf('%s/bin2coff.exe %s %s %s\n', bin2coff_path, var_filename_bin, var_filename_obj, vars_infos(k).desired_name);
    
    if(x64 == 1) 
        bin2coff_command = strcat(bin2coff_command, ' 64bit');
    end
    
    disp(bin2coff_command);    
    fprintf(fileID_create_libs, '\n%s\n', bin2coff_command);
    lib_command = sprintf('%s %s', lib_command, var_filename_obj);
    
    dumpbin_command = sprintf('dumpbin.exe /symbols %s\n', var_filename_obj);
    fprintf(fileID_create_libs, '%s', dumpbin_command);
end

fprintf(fileID_create_libs, '\n\n%s\n', lib_command);

fprintf(fileID_create_libs, '\npause');
fclose(fileID_create_libs);

disp(lib_command);
dos(create_libs_filename);