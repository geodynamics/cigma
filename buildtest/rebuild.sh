#!/bin/bash -v
cd .. && make dist && cd - && \
rm -rf cigma-0.9.1 && \
tar xvzf ../cigma-0.9.1.tar.gz && \
cd cigma-0.9.1 && \
./configure --prefix=$HOME/cigma/buildtest/cigma-0.9.1/opt \
            --with-hdf5=$HOME/opt/hdf5/1.6.5 && \
make && make install

