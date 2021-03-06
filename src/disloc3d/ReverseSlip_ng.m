function ReverseSlip_ng(varargin)
%
% Matlab script to generate the analytic boundary conditions for 
% the Reverse-Slip (no gravity) bench mark
%
% See okadasoln_ng for more info.

%% Define number of faults to taper with
taperd = 10.0;
n = 12000.0:taperd:(16000.0-taperd);

%% Get a number of faults with tapered slip
fi = [];
for i = 1 : 1 : length(n)
  fx1 =  4000.0;
  fy1 = -n(i);
  
  fx2 =  4000.0;
  fy2 =  n(i);	
  
  dip = 45.0;
  D   = n(i);
  Pr  = 0.25;
  bd  = 0;
  ss = 0;
  ds  = 1/length(n);
  ts  = 0;
  
  fi  = [fi; fx1 fy1 fx2 fy2 dip D Pr bd ss ds ts];
end

if length(varargin)==0
  okadasoln_ng(fi)
else
  okadasoln_ng(fi, varargin{1})
end
