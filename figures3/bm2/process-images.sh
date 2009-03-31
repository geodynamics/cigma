#!/bin/bash
#
# See ../bm3/process-images.sh for comments
#

imglist="
    temperature_64_20000_y099.png
    velocity_64_20000_y099.png
    log_error_temperature_64_08_20000_2100_y099.png
    log_error_temperature_64_16_20000_4700_y099.png
    log_error_temperature_64_32_20000_9900_y099.png
    log_error_velocity_64_08_20000_2100_y099.png
    log_error_velocity_64_16_20000_4700_y099.png
    log_error_velocity_64_32_20000_9900_y099.png
"

orig=${PWD}
tmp=/tmp/bm2
mkdir -p $tmp
for img in ${imglist}; do
    cp -f -v ${img} ${tmp}
done
cd ${tmp}

function img-cat
{
    local tile=$1
    shift
    montage -tile ${tile} -mode Concatenate $*
}

function img-trim
{
    echo $1 ...trimming
    convert -trim $1 $1
}

function img-add-border
{
    echo $1 ...adding border
    convert -border $2 -bordercolor white $1 $1
}

function img-resize
{
    echo $1 ...resizing
    convert -resize '80%' $1 $1
}

function cat-pair
{
    echo Concatenating $1 $2
    img-cat 2x1 $1 $2 $3
}


# 
# Start processing the images
#

for img in ${imglist}; do
    img-trim ${img}
    img-add-border ${img} 100
done

cat-pair \
    temperature_64_20000_y099.png \
    velocity_64_20000_y099.png \
    fig_fields_64_20000_y099.png

cat-pair \
    log_error_temperature_64_16_20000_4700_y099.png \
    log_error_temperature_64_32_20000_9900_y099.png \
    fig_log_error_temperature_64_16_64_32_y099.png
    
cat-pair \
    log_error_velocity_64_16_20000_4700_y099.png \
    log_error_velocity_64_32_20000_9900_y099.png \
    fig_log_error_velocity_64_16_64_32_y099.png


for img in fig_*; do
    img-resize ${img}
done

cd ${orig}
cp -f -v ${tmp}/fig_* ${orig}/out

