function okadasoln_ng(varargin)
%
% Matlab script to generate the analytic boundary conditions for 
% the Reverse-Slip and Strike-Slip (no gravity) benchmarks.
%
% This script is essentially bm5.m by Noah Fay (the version for the
% old benchmark 5), all comments below this are his (slip taper was
% modified so that zero slip at the edge of the fault plane in the
% FEMs is also zero slip in this code)...
%
% ARGUMENTS:
%  #1 - Subfaults information [REQUIRED]
%  #2 - Name of input file [OPTIONAL]

%% Inputs: 
%%        points.in   file which must exist in the local directory
%%                    or somewhere in path.  this file contains
%%                    all the xyz triples at which displacement
%%                    are calculated.  No headerlines.
%%
%% Outputs:		
%%        disp.out    file with 6 columns:
%%                    xcoord ycoord zcoord ux uy uz
%%
%%	Notes:			--See bm5_analytic_README.txt for further discussion
%%				--All units (.in and .out files) are SI (use meters)
%%				--Some things are hardcoded:
%%					-Taper depth from 12 to 16km
%%					-Taper step, taperd = 10m
%%					-Dip angle, dip = 45
%%					-xcoord of fault, fx1 = fx2 = 4km
%%					-Poisson's ratio for elastic solid, Pr = 0.25
%%					-Elastic constants lamda = mu = 30e9 Pa
%% Credits:		--The mex function (disloc3d_mod2) is a modified (and
%%					corrected) version of P.Cervelli's disloc3d.f
%%			--function fault_params_to_okada_form.m is courtesy of
%%					Brendan Meade, MIT 
%%  
%%  
%% Noah Fay, 2003
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Get the stations (anywhwere) to calculate displacement,
%% strain, strain energy, and total strain energy (sum for those pts)

if length(varargin) < 1
  error('usage: okadasoln(subfaults [, filenameIn])')
  return
else
  subfaults = varargin{1};
  if length(varargin)==2
    filein = varargin{2};
    fileout = sprintf('%s.disp.out',filein);
  else
    filein = 'points.in';
    fileout = 'disp.out';
  end
end


[sx,sy,sz] = textread(filein,'%f %f %f');


%% Read in node coords for calculating displacement at
%% the sides of the box for the analytic bc
%%	[node, sx, sy, sz] = textread('node_coords.txt','%f %f %f %f');
%% Read in the face of interest
%%	face = 'back';
%%	fid = fopen([face,'.nodes']);
%%		i = fscanf(fid, '%f');
%% Get the stations that belong to that face
%%	sx = sx(i);
%%	sy = sy(i);
%%	sz = sz(i);


%% Calculate displacements, derivatives
[dispx,dispy,dispz,D,S] = calc_disp_cervelli(sx,sy,sz,subfaults);

% $$$ %% Calculate strains strij:
% $$$ strxx = D(1,:);							
% $$$ strxy = 0.5 * (D(2,:) + D(4,:));		
% $$$ strxz = 0.5 * (D(3,:) + D(7,:));		
% $$$ 
% $$$ stryx = strxy;
% $$$ stryy = D(5,:);								
% $$$ stryz = 0.5 * (D(6,:) + D(8,:));			
% $$$ 
% $$$ strzx = strxz;
% $$$ strzy = stryz;
% $$$ strzz = D(9,:);							
% $$$ 
% $$$ %% Calculate stress tauij
% $$$ lamda = 30e9;
% $$$ mu    = lamda;
% $$$ strkk = sum([strxx; stryy; strzz]);
% $$$ 
% $$$ tauxx = lamda*strkk + 2*mu*strxx;
% $$$ tauxy =               2*mu*strxy;
% $$$ tauxz =               2*mu*strxz;
% $$$ 
% $$$ tauyx =               2*mu*stryx;
% $$$ tauyy = lamda*strkk + 2*mu*stryy;
% $$$ tauyz =               2*mu*stryz;
% $$$ 
% $$$ tauzx =               2*mu*strzx;
% $$$ tauzy = 	            2*mu*strzy;
% $$$ tauzz = lamda*strkk + 2*mu*strzz;
% $$$ 
% $$$ 
% $$$ %% Calculate strain energy
% $$$ Exx = strxx.*tauxx;
% $$$ Exy = strxy.*tauxy;
% $$$ Exz = strxz.*tauxz;
% $$$ 
% $$$ Eyx = stryx.*tauyx;
% $$$ Eyy = stryy.*tauyy;
% $$$ Eyz = stryz.*tauyz;
% $$$ 
% $$$ Ezx = strzx.*tauzx;
% $$$ Ezy = strzy.*tauzy;
% $$$ Ezz = strzz.*tauzz;
% $$$ 
% $$$ Etot = 0.5 * sum([Exx; Exy; Exz; Eyx; Eyy; Eyz; Ezx; Ezy; Ezz]);


%% Print a file with displacements
fid = fopen(fileout,'w');
for i = 1:1:length(sx)
  fprintf(fid,'%0.12e %0.12e %0.12e %0.12e %0.12e %0.12e\n',...
	  sx(i),sy(i),sz(i),dispx(i),dispy(i),dispz(i));
% $$$   fprintf(fid, '%e\t', sx(i));
% $$$   fprintf(fid, '%e\t', sy(i));
% $$$   fprintf(fid, '%e\t', sz(i));
% $$$   
% $$$   fprintf(fid, '%0.12e\t', dispx(i));
% $$$   fprintf(fid, '%0.12e\t', dispy(i));
% $$$   fprintf(fid, '%0.12e\n',   dispz(i));
end
fclose(fid);

% $$$ %% Print a file with strains
% $$$ fid = fopen('strain.out','w');
% $$$ for i = 1:1:length(sx)
% $$$   fprintf(fid, '%e\t', sx(i));
% $$$   fprintf(fid, '%e\t', sy(i));
% $$$   fprintf(fid, '%e\t', sz(i));
% $$$   
% $$$   fprintf(fid, '%e\t', strxx(i));
% $$$   fprintf(fid, '%e\t', strxy(i));
% $$$   fprintf(fid, '%e\t', strxz(i));
% $$$   fprintf(fid, '%e\t', stryx(i));
% $$$   fprintf(fid, '%e\t', stryy(i));
% $$$   fprintf(fid, '%e\t', stryz(i));
% $$$   fprintf(fid, '%e\t', strzx(i));
% $$$   fprintf(fid, '%e\t', strzy(i));
% $$$   fprintf(fid, '%e\n', strzz(i));
% $$$ end
% $$$ fclose(fid);
% $$$ 
% $$$ %% Print a file with strain energy (9) and total
% $$$ fid = fopen('E.out','w');
% $$$ for i = 1:1:length(sx)
% $$$   fprintf(fid, '%e\t', sx(i));
% $$$   fprintf(fid, '%e\t', sy(i));
% $$$   fprintf(fid, '%e\t', sz(i));
% $$$   
% $$$   %%fprintf(fid, '%e\t', Exx(i));
% $$$   %%fprintf(fid, '%e\t', Exy(i));
% $$$   %%fprintf(fid, '%e\t', Exz(i));
% $$$   %%fprintf(fid, '%e\t', Eyx(i));
% $$$   %%fprintf(fid, '%e\t', Eyy(i));
% $$$   %%fprintf(fid, '%e\t', Eyz(i));
% $$$   %%fprintf(fid, '%e\t', Ezx(i));
% $$$   %%fprintf(fid, '%e\t', Ezy(i));
% $$$   %%fprintf(fid, '%e\t', Ezz(i));
% $$$   fprintf(fid, '%e\n', Etot(i));
% $$$ end
% $$$ fclose(fid);
% $$$ 
% $$$ %% Print a file with sum of all Etot (total strain energy for all pts)
% $$$ fid = fopen('Esum.out','w');
% $$$ fprintf(fid, '%e', sum(Etot));
% $$$ fclose(fid);

%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
function [dispx, dispy, dispz,D,S] = ...
	calc_disp_cervelli(sx, sy, sz, fault_info)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% function calc_disp_cervelli
%% Calculate dipslacements via Cervelli's Okada (1992) code
%%
%%	Inputs:	sx					station x coord
%%				sy					station y coord
%%				fault_info			matrix with fault stuff
%%
%%	Outputs:	dispx				disp in x direction
%%				dispy				disp in y direction
%%				dispz				disp in z direciton
%%				D					matrix with derivatives
%%				S					Stresses (do not use)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Introduce a dummy station at the beginning 
%% of the list so the cervelli fortran code   
%% doesn't freak out...remove it later        
	sx = [-rand; sx];
	sy = [-rand; sy];
	sz = [-rand; sz];
	
%% Set station depth and elastic parameters  
	mu = 0.25;			%% poissons ratio
	nu = 0.25;	

%% get the "model matrix" with fault patch  
%% info in the form for cervelli calcs      
	M = getM(fault_info,mu);

%% Preallocate vectors for better memory management
nstations = length(sx);
dispx = zeros(1, nstations);
dispy = zeros(1, nstations);
dispz = zeros(1, nstations);
D = zeros(9, nstations);
S = zeros(6, nstations);

%% Loop over a subset of stations b/c Cervelli   
%% code doesn't seem to like it if given more    
%% than ~500 stations                            
for subset = 1:1:ceil(nstations/500)
  a = (subset-1)*500 + 1;
  b = subset*500;
  if b > nstations
    b = nstations;
  end
  X = [sx(a:b)'; sy(a:b)'; sz(a:b)'];
  
  [Usub,Dsub,Ssub,flag,flag2] = disloc3d_mod2(M,X,mu,nu);
  dispx(a:b) = Usub(1,:)';
  dispy(a:b) = Usub(2,:)';
  dispz(a:b) = Usub(3,:)';
  D(:,a:b) = Dsub;
  S(:,a:b) = Ssub;
end %%subset
clear X Usub Dsub Ssub flag flag2

%% Remove the dummy displacements at the beginning 
%% of the displacement lists                       
dispx(1) = [];
dispy(1) = [];
dispz(1) = [];
D(:,1) = [];

%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
function M = getM(fi,mu)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% function getM
%% Convert fault parameters into form needed for
%% Cervelli calculations
%%
%%	Inputs:	fi					fault stuff
%%				mu					Poissons ratio
%%
%%	Outputs:	M					matrix with fault stuff
%%									(see below for what is on
%%									the rows, 1 column per 
%%									fault patch)
%%
%%	Notes:		This could be redone to be vectorized and faster
%%            (later maybe)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

M=[];

%% Loop over all the fault patches to build M   
s = size(fi);
for j = 1:1:s(1)   
  fx1 = fi(j,1);
  fy1 = fi(j,2);
  fx2 = fi(j,3);
  fy2 = fi(j,4);
  dip = fi(j,5);%%in degs here
  D   = fi(j,6);
  Pr  = fi(j,7);
  bd  = fi(j,8);
  ss  = fi(j,9);
  ds  = fi(j,10);
  ts  = fi(j,11);
  [strike, L, W, ofx, ofy, ofxe, ofye, tfx, tfy, tfxe, tfye] =...
      fault_params_to_okada_form(fx1,fy1,fx2,fy2,dip*pi/180.0,D,bd);
  
  length = L;
  width  = W;
  
  if (dip) < 0
    east  = (L/2)*cos(strike) + tfx;		%% east component of top center
    north = (L/2)*sin(strike) + tfy;		%% north   "       "  "     "
    depth = bd;				
    width = abs(width);
    ss = -1*ss;
    ds = -1*ds;
    ts = -1*ts;
  else
    east  = (L/2)*cos(strike) + ofx;		%% east component of bottom center
    north = (L/2)*sin(strike) + ofy;
    depth = D;
  end
  
  str = 180.0/pi*(pi/2-strike);  %%Cervelli takes strike in degs
			       %%(strike comes out of
			       %% fault_params_to_okada_form in rads)
			       
			       %%nextMcol = [length;width;depth;dip;str;east;north;ss;ds;ts];
			       M(:,j) = [length;width;depth;dip;str;east;north;ss;ds;ts];
			       
end
%%
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
function [strike, L, W, ofx, ofy, ofxe, ofye, tfx, tfy, tfxe, tfye] = fault_params_to_okada_form(sx1, sy1, sx2, sy2, dip, D, bd)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%                                                 %%
%%  fault_params_to_okada_form.m                   %%
%%                                                 %%
%%  MATLAB script                                  %%
%%                                                 %%
%%  This function takes fault trace, dip, and      %%
%%  locking depth information and calculates the   %%
%%  anchor coordinates, length, width and strike   %%
%%  of the fault plane as per Okada (1985).        %%
%%                                                 %%
%%  It may be beneficial in the future to compute  %%
%%  the midpoint base anchor as well for using     %%
%%  Okada (1992) Stanford binary.                  %%
%%                                                 %%
%%  We should also allow for buried faults in the  %%
%%  future.  This can be achieved by passing a     %%
%%  locking depth and a burial depth.  The only    %%
%%  output changed would be the width of the       %%
%%  fault plane.                                   %%
%%                                                 %%
%%  We may need to be more formal about say        %%
%%  endpoint1 < endpoint2 ... maybe                %%
%%                                                 %%
%%  Arguments:                                     %%
%%    sx1:  x coord of fault trace endpoint1       %%
%%    sy1:  y coord of fault trace endpoint1       %%
%%    sx2:  x coord of fault trace endpoint2       %%
%%    sy2:  y coord of fault trace endpoint2       %%
%%    dip:  dip of fault plane [radians]           %%
%%    D  :  fault locking depth                    %%
%%    bd :  burial depth (top "locking depth")     %%
%%                                                 %%
%%  Returned variables:                            %%
%%    strike:  stike of fault plane                %%
%%    L     :  fault length                        %%
%%    W     :  fault width                         %%
%%    ofx   :  x coord of fault anchor             %%
%%    ofy   :  y coord of fault anchor             %%
%%    ofxe  :  x coord of other buried corner      %%
%%    ofye  :  y coord of other buried corner      %%
%%    tfx   :  x coord of fault anchor             %%
%%             (top relative)                      %%
%%    tfy   :  y coord of fault anchor             %%
%%             (top relative)                      %%
%%    tfxe  :  x coord of other buried corner      %%
%%             (top relative)                      %%
%%    tfye  :  y coord of other buried corner      %%
%%             (top relative)                      %%
%%                                                 %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Do some basic checks  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if (D < bd)
   disp(sprintf('Burial depth is greater than locking depth!'))
   disp(sprintf('Fault width will be negative!'))
end

if (D == bd)
   disp(sprintf('Burial depth is equal to locking depth!'))
   disp(sprintf('Fault width will zero!'))
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Calculate need parameters for Okada input  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
strike                        = atan2(sy1 - sy2, sx1 - sx2) + pi;
L                             = sqrt((sx2 - sx1)^2+(sy2 - sy1)^2);
W                             = (D - bd) / sin(dip);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  For older versions without a burial depth option  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  W                             = D / sin(dip);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Calculate fault segment anchor and other buried point  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ofx                           = sx1 + D / tan(dip) * sin(strike);
ofy                           = sy1 - D / tan(dip) * cos(strike);
ofxe                          = sx2 + D / tan(dip) * sin(strike);
ofye                          = sy2 - D / tan(dip) * cos(strike);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Calculate fault segment anchor and other buried point (top relative)  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tfx                           = sx1 + bd / tan(dip) * sin(strike);
tfy                           = sy1 - bd / tan(dip) * cos(strike);
tfxe                          = sx2 + bd / tan(dip) * sin(strike);
tfye                          = sy2 - bd / tan(dip) * cos(strike);

