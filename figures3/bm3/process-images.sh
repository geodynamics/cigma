#!/bin/bash
#
# Some useful links:
#   http://www.imagemagick.org/Usage/crop/index.html
#   http://personal.cscs.ch/~mvalle/postprocessing/ImageTools.html
#

#
# List of the original image names
#
imglist="
    pressure_512.png
    velocity_512.png
    log_error_pressure_128.png
    log_error_pressure_256.png
    log_error_pressure_512.png
    log_error_velocity_128.png
    log_error_velocity_256.png
    log_error_velocity_512.png
    log_error_pressure_512_128.png
    log_error_pressure_512_256.png
    log_error_velocity_512_128.png
    log_error_velocity_512_256.png
"

# 
# Operate on copies of the original images
#
orig=${PWD}
tmp=/tmp/bm3
mkdir -p $tmp
for img in ${imglist}; do
    cp -f -v ${img} ${tmp}
done
cd ${tmp}


#
# Some useful functions
#
function concatenate-images
{
    local tile=$1
    shift
    montage -tile ${tile} -mode Concatenate $*
}

function trim-image
{
    echo $1 ...trimming
    convert -trim $1 $1
}

function add-border
{
    echo $1 ...adding border
    convert -border $2 -bordercolor white $1 $1
}

function resize-image
{
    echo $1 ...resizing
    convert -resize '80%' $1 $1
}

function concatenate-pair
{
    echo Concatenating $1 $2
    concatenate-images 2x1 $1 $2 $3
}

#
# Start processing the images
#

for img in ${imglist}; do
    trim-image ${img}
    add-border ${img} 100
done

concatenate-pair pressure_512.png velocity_512.png fig_fields_512.png
concatenate-pair log_error_pressure_128.png log_error_velocity_128.png fig_log_error_fields_128.png
concatenate-pair log_error_pressure_256.png log_error_velocity_256.png fig_log_error_fields_256.png
concatenate-pair log_error_pressure_512.png log_error_velocity_512.png fig_log_error_fields_512.png

concatenate-images 3x1 log_error_pressure_128.png log_error_pressure_256.png log_error_pressure_512.png fig_log_error_pressure.png
concatenate-images 3x1 log_error_velocity_128.png log_error_velocity_256.png log_error_velocity_512.png fig_log_error_velocity.png

concatenate-images 2x1 log_error_pressure_256.png log_error_pressure_512.png fig_log_error_pressure_256_512.png
concatenate-images 2x1 log_error_velocity_256.png log_error_velocity_512.png fig_log_error_velocity_256_512.png

concatenate-images 2x1 log_error_pressure_512_128.png log_error_pressure_512_256.png fig_log_error_pressure_512_128_512_256.png
concatenate-images 2x1 log_error_velocity_512_128.png log_error_velocity_512_256.png fig_log_error_velocity_512_128_512_256.png


#
# finally, resize
for img in fig_*; do
    resize-image ${img}
done

#
# Copy the images back
#
cd ${orig}
cp -f -v ${tmp}/fig_* ${orig}/out

