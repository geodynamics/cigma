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
    full_pressure_512.png
    full_velocity_512.png
    full_log_error_pressure_128.png
    full_log_error_pressure_256.png
    full_log_error_pressure_512.png
    full_log_error_pressure_512_128.png
    full_log_error_pressure_512_256.png
    full_log_error_velocity_128.png
    full_log_error_velocity_256.png
    full_log_error_velocity_512.png
    full_log_error_velocity_512_128.png
    full_log_error_velocity_512_256.png
    inc_pressure_512.png
    inc_velocity_512.png
    inc_log_error_pressure_128.png
    inc_log_error_pressure_256.png
    inc_log_error_pressure_512.png
    inc_log_error_pressure_512_128.png
    inc_log_error_pressure_512_256.png
    inc_log_error_velocity_512.png
    inc_log_error_velocity_512_128.png
    inc_log_error_velocity_512_256.png
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


img-caption '(a) Pressure' \
    full_pressure_512.png \
    full_pressure_512.png
img-caption '(b) Pressure near inclusion' \
    inc_pressure_512.png \
    inc_pressure_512.png

img-caption '(a) log (pressure error for 256 x 256 case)' \
    full_log_error_pressure_256.png \
    full_log_error_pressure_256.png
img-caption '(b) log (pressure error for 512 x 512 case)' \
    full_log_error_pressure_512.png \
    full_log_error_pressure_512.png

img-caption '(a) log (pressure error for 128 x 128 case)' \
    full_log_error_pressure_512_128.png \
    full_log_error_pressure_512_128.png
img-caption '(a) log (pressure error for 256 x 256 case)' \
    full_log_error_pressure_512_256.png \
    full_log_error_pressure_512_256.png

img-caption '(a) log (pressure error for 256 x 256 case)' \
    inc_log_error_pressure_256.png \
    inc_log_error_pressure_256.png
img-caption '(b) log (pressure error for 512 x 512 case)' \
    inc_log_error_pressure_512.png \
    inc_log_error_pressure_512.png

img-caption '(a) log (pressure error for 128 x 128 case)' \
    inc_log_error_pressure_512_128.png \
    inc_log_error_pressure_512_128.png
img-caption '(a) log (pressure error for 256 x 256 case)' \
    inc_log_error_pressure_512_256.png \
    inc_log_error_pressure_512_256.png


cat-pair \
    full_pressure_512.png \
    inc_pressure_512.png \
    fig_pressure_512.png

cat-pair \
    full_log_error_pressure_256.png \
    full_log_error_pressure_512.png \
    fig_full_log_error_pressure_256_512.png

cat-pair \
    full_log_error_pressure_512_128.png \
    full_log_error_pressure_512_256.png \
    fig_full_log_error_pressure_512_128_512_256.png

cat-pair \
    inc_log_error_pressure_256.png \
    inc_log_error_pressure_512.png \
    fig_inc_log_error_pressure_256_512.png \

cat-pair \
    inc_log_error_pressure_512_128.png \
    inc_log_error_pressure_512_256.png \
    fig_inc_log_error_pressure_512_128_512_256.png


# finally, resize the images that will be included in the manual
for img in fig_*; do
    resize-image ${img}
done

#
# Copy the images back
#
cd ${orig}
cp -f -v ${tmp}/fig_* ${orig}/out

