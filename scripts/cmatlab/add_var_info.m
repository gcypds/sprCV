function add_var_info(var, desired_type, desired_name, varargin)
% Struct definition
global vars_infos;

if isempty(vars_infos)
   vars_infos = struct('type',{},'var',{},'desired_type',{},'desired_name',{},'desired_format',{});
end

var_type = 'unknown';
var_format = '%.3g';

if(isscalar(var))
    var_type = 'scalar';    
elseif (isvector(var))
    var_type = 'vector';
elseif (ismatrix(var))
    var_type = 'matrix';
end   

if (isempty(varargin))
    if(isfloat(var))
        var_format = '%.3g';    
    elseif (isinteger(var))
        var_format = '%d';
    end   
else
    var_format = varargin{1};
end

% Save W 2d array
% Is necessary to traspose the 2d array
var_info.type = var_type;

% Is necessary to traspose var
var_info.var = var';
var_info.desired_type = desired_type;
var_info.desired_name = desired_name;
var_info.desired_format = var_format;

vars_infos(end+1) = var_info;

end

