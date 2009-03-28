#!/bin/bash

function download-images
{
    local url="http://stress.geodynamics.org/~luis/bm3/inclusion/"
    wget -q -nc -k -O foo.html ${url}
    wget -q -nc -F -B ${url} -i foo.html -nd -r -A'*.png'
    rm -f foo.html
}

function concatenate-pair
{
    montage -tile 2x1 -mode Concatenate $1 $2 $3
}

function trim-image
{
    convert -trim -resize '80%' $1 $1
}

download-images
concatenate-pair pressure_512.png velocity_512.png fields_512.png
trim-image fields_512.png

