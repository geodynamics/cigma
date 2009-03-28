#!/bin/bash
#
# Some useful links:
#   http://www.imagemagick.org/Usage/crop/index.html
#   http://personal.cscs.ch/~mvalle/postprocessing/ImageTools.html
#

function download-images
{
    local url="http://stress.geodynamics.org/~luis/bm3/inclusion/"
    wget -q -nc -k -O foo.html ${url}
    wget -q -nc -F -B ${url} -i foo.html -nd -r -A'*.png'
    rm -f foo.html
}

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
    echo Concatenating $1 and $2
    concatenate-images 2x1 $1 $2 $3
}



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

imglist2="
    fields_512.png
    log_error_128.png
    log_error_256.png
    log_error_512.png
"

#download-images

for img in ${imglist}; do
    trim-image ${img}
    add-border ${img} 100
done

concatenate-pair pressure_512.png velocity_512.png fields_512.png
concatenate-pair log_error_pressure_128.png log_error_velocity_128.png log_error_128.png
concatenate-pair log_error_pressure_256.png log_error_velocity_256.png log_error_256.png
concatenate-pair log_error_pressure_512.png log_error_velocity_512.png log_error_512.png

for img in ${imglist2}; do
    resize-image ${img}
done

