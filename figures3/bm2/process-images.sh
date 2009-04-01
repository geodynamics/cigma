#!/bin/bash
#
# See ../bm3/process-images.sh for comments
#

source ../common.sh

set -x

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


# 
# Start processing the images
#

for img in ${imglist}; do
    img-trim ${img}
    img-add-border ${img} 100
done

img-caption '(a) Temperature                           ' \
    temperature_64_20000_y099.png \
    temperature_64_20000_y099.png

img-caption '(b) Velocity                     ' \
    velocity_64_20000_y099.png \
    velocity_64_20000_y099.png

img-caption '(a) log (temperature error for 16 x 16 x 16 case)             ' \
    log_error_temperature_64_16_20000_4700_y099.png \
    log_error_temperature_64_16_20000_4700_y099.png

img-caption '(b) log (temperature error for 32 x 32 x 32 case)         ' \
    log_error_temperature_64_32_20000_9900_y099.png \
    log_error_temperature_64_32_20000_9900_y099.png

img-caption '(a) log (velocity error for 16 x 16 x 16 case)                ' \
    log_error_velocity_64_16_20000_4700_y099.png \
    log_error_velocity_64_16_20000_4700_y099.png

img-caption '(b) log (velocity error for 32 x 32 x 32 case)            ' \
    log_error_velocity_64_32_20000_9900_y099.png \
    log_error_velocity_64_32_20000_9900_y099.png

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

