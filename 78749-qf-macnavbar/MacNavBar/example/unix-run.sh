#!/bin/bash

cd ../
QFMACNAVBAR_PATH="`pwd`/bin"
cd -

export LD_LIBRARY_PATH="${QFMACNAVBAR_PATH}:${LD_LIBRARY_PATH}"
exec ./example "$@"
