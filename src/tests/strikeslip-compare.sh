#!/bin/bash


################
# 0: tet4_1000m
# 1: tet4_0500m
# 2: hex8_1000m
# 3: hex8_0500m


################
# r='00'
# a='tet4_1000m'
# b='tet4_1000m'

# r='01'
# a='tet4_1000m'
# b='tet4_0500m'

# r='02'
# a='tet4_1000m'
# b='hex8_1000m'

# r='03'
# a='tet4_1000m'
# b='hex8_0500m'


################
# r='10'
# a='tet4_0500m'
# b='tet4_1000m'

# r='11'
# a='tet4_0500m'
# b='tet4_0500m'

# r='12'
# a='tet4_0500m'
# b='hex8_1000m'

# r='13'
# a='tet4_0500m'
# b='hex8_0500m'


################
# r='20'
# a='hex8_1000m'
# b='tet4_1000m'

# r='21'
# a='hex8_1000m'
# b='tet4_0500m'

# r='22'
# a='hex8_1000m'
# b='hex8_1000m'

# r='23'
# a='tet4_1000m'
# b='hex8_0500m'


################
# r='30'
# a='hex8_0500m'
# b='tet4_1000m'

# r='31'
# a='hex8_0500m'
# b='tet4_0500m'

# r='32'
# a='hex8_0500m'
# b='hex8_1000m'

# r='33'
# a='hex8_0500m'
# b='hex8_0500m'




#######################################################

path1="./strikeslip_${a}_t0.vtk:displacements_t0"
path2="./strikeslip_${b}_t0.vtk:displacements_t0"
outputfile="./residuals${r}-${a}-${b}.vtk"

echo "Case ${r}"
echo "Comparing ${a} with ${b}"
echo "../cigma compare --output=${outputfile} --first=${path1} --second=${path2} ${*}"

../cigma compare \
    --output=${outputfile}  \
    --first=${path1}        \
    --second=${path2}       \
    ${*}


#EOF
