#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: ./test.sh operations"
    exit 1
fi

DATABASE="db"


compile_program() {
    echo "...Compiling database"
    make re
    echo "...Compilation complete"
}


meta_command_test() {
    output=$(echo ".exit" | ./$DATABASE)
    test_name="META COMMAND TEST"

 if diff <(echo "$output") "" >/dev/null; then
        echo "✓ Test passed: $test_name"
        return 0
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        echo "Actual output:"
        echo "$output"
        return 1
    fi
}

meta_command_test()