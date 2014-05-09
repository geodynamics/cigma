#!/bin/bash
set -x
source ../common.sh

img-caption '(a) 2D Poisson Problem' \
    alpha_2d.png \
    rm_alpha_2d.png

img-caption '(b) 3D Poisson Problem' \
    alpha_3d.png \
    rm_alpha_3d.png

cat-pair \
    rm_alpha_2d.png \
    rm_alpha_3d.png \
    alpha.png

rm -f rm_*.png

