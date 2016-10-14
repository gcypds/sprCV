path = 'D:\People_Detection\Arvis3\Arvis3\cbook\';

Dl = load(strcat(path,'Dlast.txt'));
DmS = load(strcat(path,'DminS.txt'));
DmL = load(strcat(path,'DminL.txt'));
R = load(strcat(path,'R.txt'));
T = load(strcat(path,'T.txt'));
V = load(strcat(path,'V.txt'));

%Truncate
Dl = Dl*30000;
DmS = DmS*30000;
DmL = DmL*30000;
R = R*10000;
T = T*100;
V = V*1000;

%Save
imwrite(uint16(Dl),strcat(path,'Dlast.png'),'png');
imwrite(uint16(DmS),strcat(path,'DminS.png'),'png');
imwrite(uint16(DmL),strcat(path,'DminL.png'),'png');
imwrite(uint16(R),strcat(path,'R.png'),'png');
imwrite(uint16(T),strcat(path,'T.png'),'png');
imwrite(uint16(V),strcat(path,'V.png'),'png');