#!/bin/bash

##############################################################################

function cigma-compare()
{
    of=${pf}-${a}_${aa}-${b}_${bb}-step${step}.vtk
    echo $sep
    echo "# Creating $of"
    cigma compare --output=${of} \
        --first=${df}:/${a}/${bm}_${aa}/variables/${av}/${step} \
        --first-mesh=${df}:/mesh/${bm}_${aa} \
        --second=${df}:/${b}/${bm}_${bb}/variables/${bv}/${step} \
        --second-mesh=${df}:/mesh/${bm}_${aa} \
        --verbose --output-frequency=10000
}
function compare-steps()
{
    for t in $*; do
        step="step$t"
        cigma-compare
    done
}

sep="# ----------------------------------------------"

##############################################################################

bm=bmssnog
df=$bm.h5

t0=00000
t1=00010
t2=00050
t3=00100

##############################################################################

a=geofest
b=geofest

pf=ss-disp
av=displacement
bv=displacement
aa=tet4_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_1000m
bb=tet4_0250m
compare-steps $t0 $t1 $t2 $t3

pf=ss-velo
av=del_displacement
bv=del_displacement
aa=tet4_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_1000m
bb=tet4_0250m
compare-steps $t0 $t1 $t2 $t3

##############################################################################

a=pylith
b=geofest

pf=ss-disp
av=displacement
bv=displacement
aa=hex8_1000m
bb=tet4_1000m
compare-steps $t0 $t1 $t2 $t3
aa=hex8_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_0250m
bb=tet4_0500m
compare-steps $t0
aa=tet4_0250m
bb=tet4_0250m
compare-steps $t0

pf=ss-velo
av=velocity
bv=del_displacement
aa=hex8_1000m
bb=tet4_1000m
compare-steps $t0 $t1 $t2 $t3
aa=hex8_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_1000m
bb=tet4_0500m
compare-steps $t0 $t1 $t2 $t3
aa=tet4_0250m
bb=tet4_0500m
compare-steps $t0
#aa=tet4_0250m
#bb=tet4_0250m
#compare-steps $t0

##############################################################################
##############################################################################
