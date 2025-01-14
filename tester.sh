#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: ./test.sh operations"
    exit 1
fi

make re

