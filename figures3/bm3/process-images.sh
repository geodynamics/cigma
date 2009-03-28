#!/bin/bash

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

function concatenate-pair
{
    concatenate-images 2x1 $1 $2 $3
}

function trim-image
{
    convert -trim -resize '80%' $1 $1
}

download-images
concatenate-pair pressure_512.png velocity_512.png fields_512.png
concatenate-pair log_error_pressure_128.png log_error_velocity_128.png log_error_128.png
concatenate-pair log_error_pressure_256.png log_error_velocity_256.png log_error_256.png
concatenate-pair log_error_pressure_512.png log_error_velocity_512.png log_error_512.png

trimlist="
    fields_512.png
    log_error_128.png
    log_error_256.png
    log_error_512.png
"

for img in ${trimlist}; do
    trim-image ${img}
done

