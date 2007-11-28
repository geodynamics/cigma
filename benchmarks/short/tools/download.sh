#!/bin/bash
#
# This script was used to download the benchmark files available on the website
#
#   http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/
#
# The resulting directory layout is as follows:
#
#       benchmarks
#       |-- bmssnog
#       |   |-- input
#       |   |   |-- bmssnog_tet4_1000m
#       |   |   |-- bmssnog_tet4_0500m
#       |   |   `-- bmssnog_tet4_0250m
#       |   |-- pylith
#       |   |   |-- bmssnog_hex8_1000m
#       |   |   |-- bmssnog_tet4_1000m
#       |   |   |-- bmssnog_tet4_0500m_old
#       |   |   |-- bmssnog_tet4_0500m_new_bc
#       |   |   |-- bmssnog_tet4_0500m_altered_bc
#       |   |   `-- bmssnog_tet4_0250m
#       |   `-- geofest
#       |       |-- bmssnog_tet4_1000m
#       |       |-- bmssnog_tet4_0500m
#       |       `-- bmssnog_tet4_0250m
#       `-- bmrsnog
#           |-- input
#           |   |-- bmrsnog_tet4_1000m
#           |   |-- bmrsnog_tet4_0500m
#           |   |-- bmrsnog_tet4_0250m
#           |   |-- bmrsnog_tet4_var_res_01
#           |   `-- bmrsnog_tet4_var_res_02
#           |-- pylith
#           |   |-- bmrsnog_hex8_1000m
#           |   |-- bmrsnog_tet4_1000m
#           |   |-- bmrsnog_tet4_0500m
#           |   |-- bmrsnog_tet4_0250m
#           |   `-- bmrsnog_tet4_var_res_01
#           `-- geofest
#               |-- bmrsnog_tet4_1000m
#               |-- bmrsnog_tet4_0500m
#               `-- bmrsnog_tet4_var_res_01
#

bmdir="$HOME/benchmarks"
bmssnog="$bmdir/bmssnog"
bmrsnog="$bmdir/bmrsnog"

mkdir -v $bmdir
if [ "$?" != "0" ]; then
    echo "Please remove or rename the specified directory."
    exit 1
fi


mkdir -v -p $bmssnog
mkdir -v -p $bmssnog/input
mkdir -v -p $bmssnog/pylith
mkdir -v -p $bmssnog/geofest

mkdir -v -p $bmrsnog
mkdir -v -p $bmrsnog/input
mkdir -v -p $bmrsnog/pylith
mkdir -v -p $bmrsnog/geofest

################################################################################
# Strike-slip benchmark                                                        #
################################################################################

# input meshes
cd $bmssnog/input
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/pylith-0.8-input/bmssnog_tet4_1000m.tgz
tar xvfz bmssnog_tet4_1000m.tgz -C $bmssnog/input 

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/pylith-0.8-input/bmssnog_tet4_0500m.tgz
tar xvfz bmssnog_tet4_0500m.tgz -C $bmssnog/input 

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/pylith-0.8-input/bmssnog_tet4_0250m.tgz
tar xvfz bmssnog_tet4_0250m.tgz -C $bmssnog/input 


# pylith data (v0.8)
cd $bmssnog/pylith

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bmssnog_1000m-pylith-np1-linhex.tar.bz2    --output-document=bmssnog_hex8_1000m.tar.bz2
tar xvfj bmssnog_hex8_1000m.tar.bz2
mv -v bmssnog_1000m-pylith-np1-linhex/ bmssnog_hex8_1000m

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bmssnog_1000m-pylith-np1-lintet.tar.bz2    --output-document=bmssnog_tet4_1000m.tar.bz2
tar xvfj bmssnog_tet4_1000m.tar.bz2
mv -v bmssnog_1000m-pylith-np1-lintet/ bmssnog_tet4_1000m

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bmssnog_500m-pylith-np1-lintet.tar.bz2     --output-document=bmssnog_tet4_0500m.tar.bz2
tar xvfj bmssnog_tet4_0500m.tar.bz2
mv -v bmssnog_500m-pylith-np1-lintet/ bmssnog_tet4_0500m_old

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bmssnog_250m.tar.bz2                       --output-document=bmssnog_tet4_0250m.tar.bz2
tar xvfj bmssnog_tet4_0250m.tar.bz2
mv -v bmssnog_250m/ bmssnog_tet4_0250m


# revised pylith data (v0.8)

cd $bmssnog/pylith

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/results1.tar.bz2       --output-document=bmssnog_tet4_0500m_new_bc.tar.bz2
tar xvfj bmssnog_tet4_0500m_new_bc.tar.bz2
mv -v bmssnog_500m-pylith-np1-lintet/ bmssnog_tet4_0500m_new_bc

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bmssnog_500m-pylith-np1-tet4-new.tar.bz2   --output-document=bmssnog_tet4_0500m_altered_bc.tar.bz2
tar xvfj bmssnog_tet4_0500m_altered_bc.tar.bz2
mv -v bmssnog_500m-pylith-np1-tet4-new/ bmssnog_tet4_0500m_altered_bc


# geofest data (v4.5)
cd $bmssnog/geofest

mkdir -v -p $bmssnog/geofest/bmssnog_tet4_1000m
cd $bmssnog/geofest/bmssnog_tet4_1000m
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bm4_uni1000.tar.gz
tar xvfz bm4_uni1000.tar.gz

mkdir -v -p $bmssnog/geofest/bmssnog_tet4_0500m
cd $bmssnog/geofest/bmssnog_tet4_0500m
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bm4_uni0500.out.gz
gunzip bm4_uni0500.out.gz

mkdir -v -p $bmssnog/geofest/bmssnog_tet4_0250m
cd $bmssnog/geofest/bmssnog_tet4_0250m
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-strikeslip/results/bm4_uni0250.out.gz
gunzip bm4_uni0250.out.gz



################################################################################
# Reverse-slip benchmark                                                       #
################################################################################


# input meshes
cd $bmrsnog/input

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/bmrsnog_hex8_1000m.tgz
tar xvfz bmrsnog_hex8_1000m.tgz

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/bmrsnog_tet4_1000m.tgz
tar xvfz bmrsnog_tet4_1000m.tgz

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/bmrsnog_tet4_0500m.tgz
tar xvfz bmrsnog_tet4_0500m.tgz

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/bmrsnog_tet4_0250m.tgz
tar xvfz bmrsnog_tet4_0250m.tgz

mkdir -v -p $bmrsnog/input/bmrsnog_tet4_var_res_01
cd $bmrsnog/input/bmrsnog_tet4_var_res_01
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/tet_var_res_01_pylith_input.tgz
tar xvfz tet_var_res_01_pylith_input.tgz

mkdir -v -p $bmrsnog/input/bmrsnog_tet4_var_res_02
cd $bmrsnog/input/bmrsnog_tet4_var_res_02
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/pylith-0.8-input/tet_var_res_02_pylith_input.tgz
tar xvfz tet_var_res_02_pylith_input.tgz


# pylith data (v0.8)
cd $bmrsnog/pylith

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/bmrsnog_1000m-pylith-np1-linhex.tar.bz2
tar xvfj bmrsnog_1000m-pylith-np1-linhex.tar.bz2
mv -v bmrsnog_1000m-pylith-np1-linhex/ bmrsnog_hex8_1000m

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/bmrsnog_1000m-pylith-np1-lintet.tar.bz2
tar xvfj bmrsnog_1000m-pylith-np1-lintet.tar.bz2
mv -v bmrsnog_1000m-pylith-np1-lintet/ bmrsnog_tet4_1000m

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/bmrsnog_500m-pylith-np1-lintet.tar.bz2
tar xvfj bmrsnog_500m-pylith-np1-lintet.tar.bz2
mv -v bmrsnog_500m-pylith-np1-lintet/ bmrsnog_tet4_0500m

wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/bmrsnog_250m_lintet.tar.bz2
tar xvfj bmrsnog_250m_lintet.tar.bz2
mv -v bmrsnog_250m_lintet/ bmrsnog_tet4_0250m

mkdir -v -p $bmrsnog/pylith/bmrsnog_tet4_var_res_01
cd $bmrsnog/pylith/bmrsnog_tet4_var_res_01
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/tet_var_res_01_pylith_soln.tgz
tar xvfz tet_var_res_01_pylith_soln.tgz


# geofest data (v4.5)
cd $bmrsnog/geofest

mkdir -v -p $bmrsnog/geofest/bmrsnog_tet4_1000m
cd $bmrsnog/geofest/bmrsnog_tet4_1000m
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/tet4_1000m_geofest.tar
tar xvf tet4_1000m_geofest.tar

mkdir -v -p $bmrsnog/geofest/bmrsnog_tet4_0500m
cd $bmrsnog/geofest/bmrsnog_tet4_0500m
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/tet4_500m_geofest.tar
tar xvf tet4_500m_geofest.tar

mkdir -v -p $bmrsnog/geofest/bmrsnog_tet4_var_res_01
cd $bmrsnog/geofest/bmrsnog_tet4_var_res_01
wget http://www.geodynamics.org/cig/workinggroups/short/workarea/benchmarks/benchmark-rs-nog/results/tet_var_res_01_geofest4.5_soln.tar.gz
tar xvfz tet_var_res_01_geofest4.5_soln.tar.gz


exit 0
