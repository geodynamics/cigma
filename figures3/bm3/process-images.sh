#!/bin/bash
#

# Include some common image processing functions
source ../common.sh

# Enable bash debugging
set -x

#
# List of the original image names
#
imglist="
    pressure_512.png
    velocity_512.png
    error_pressure_128.png
    error_pressure_256.png
    error_pressure_512.png
    error_velocity_128.png
    error_velocity_256.png
    error_velocity_512.png
    error_pressure_512_128.png
    error_pressure_512_256.png
    error_velocity_512_128.png
    error_velocity_512_256.png
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
# Start processing the images
#

for img in ${imglist}; do
    img-trim ${img}
    img-add-border ${img} 100
done


cat-pair \
    pressure_512.png \
    velocity_512.png \
    fig_fields_512.png


cat-pair \
    error_pressure_128.png \
    error_velocity_128.png \
    fig_error_fields_128.png
cat-pair \
    error_pressure_256.png \
    error_velocity_256.png \
    fig_error_fields_256.png
cat-pair \
    error_pressure_512.png \
    error_velocity_512.png \
    fig_error_fields_512.png


cat-pair \
    log_error_pressure_128.png \
    log_error_velocity_128.png \
    fig_log_error_fields_128.png
cat-pair \
    log_error_pressure_256.png \
    log_error_velocity_256.png \
    fig_log_error_fields_256.png
cat-pair \
    log_error_pressure_512.png \
    log_error_velocity_512.png \
    fig_log_error_fields_512.png


img-cat 3x1 \
    error_pressure_128.png \
    error_pressure_256.png \
    error_pressure_512.png \
    fig_error_pressure.png
img-cat 3x1 \
    log_error_pressure_128.png \
    log_error_pressure_256.png \
    log_error_pressure_512.png \
    fig_log_error_pressure.png

img-cat 2x1 \
    error_pressure_256.png \
    error_pressure_512.png \
    fig_error_pressure_256_512.png
img-cat 2x1 \
    log_error_pressure_256.png \
    log_error_pressure_512.png \
    fig_log_error_pressure_256_512.png


img-cat 2x1 \
    error_pressure_512_128.png \
    error_pressure_512_256.png \
    fig_error_pressure_512_128_512_256.png
img-cat 2x1 \
    log_error_pressure_512_128.png \
    log_error_pressure_512_256.png \
    fig_log_error_pressure_512_128_512_256.png


img-cat 2x1 \
    error_velocity_512_128.png \
    error_velocity_512_256.png \
    fig_error_velocity_512_128_512_256.png
img-cat 2x1 \
    log_error_velocity_512_128.png \
    log_error_velocity_512_256.png \
    fig_log_error_velocity_512_128_512_256.png


# finally, resize the images that will be included in the manual
for img in fig_*; do
    resize-image ${img}
done

#
# Copy the images back
#
cd ${orig}
cp -f -v ${tmp}/fig_* ${orig}/out

