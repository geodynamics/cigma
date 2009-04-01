#!/bin/bash

#
# In this file we define a few useful functions for
# processing multiple images, based on the ImageMagick
# manipulation programs
#
# Some useful links:
#
#   http://www.imagemagick.org/Usage/crop/
#   http://www.imagemagick.org/Usage/annotating/
#   http://personal.cscs.ch/~mvalle/postprocessing/ImageTools.html
#

function img-caption0
{
    #echo $1 ...adding caption
    convert \
        -gravity south  \
        -stroke '#000C' \
        -strokewidth 2  \
        -font Arial-Regular \
        -pointsize 30 \
        -annotate 0 "$1" \
        $2 $3
}

function img-caption
{
    #echo $1 ...adding caption
    montage \
        -geometry +0+0 \
        -background White \
        -font Arial-Regular \
        -pointsize 35 \
        -label "$1" \
        $2 $3
}

function img-cat
{
    local tile=$1
    shift
    montage -tile ${tile} -mode Concatenate $*
}

function cat-pair
{
    #echo Concatenating $1 $2
    img-cat 2x1 $1 $2 $3
}


function img-trim
{
    #echo $1 ...trimming
    convert -trim $1 $1
}

function img-add-border
{
    #echo $1 ...adding border
    convert -border $2 -bordercolor white $1 $1
}

function img-resize
{
    #echo $1 ...resizing
    convert -resize '80%' $1 $1
}

