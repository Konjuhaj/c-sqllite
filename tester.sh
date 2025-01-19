#!/bin/bash

# if [ $# -lt 1 ]; then
#     echo "Usage: ./test.sh operations"
#     exit 1
# fi

DATABASE="db"

META_TEST_FILE="meta_test.txt"
META_TEST_RESULT="test_files/meta_test_result.txt"

INSERT_TEST_FILE="insert_test.txt"
INSERT_TEST_RESULT="test_files/insert_test_result.txt"

SELECT_TEST_FILE="select_test.txt"
SELECT_TEST_RESULT="test_files/select_test_result.txt"

PERSISTANCE_TEST_FILE="persistance_test.txt"
PERSISTANCE_TEST_RESULT="test_files/persistance_test_result.txt"

compile_program() {
    echo "...Compiling database"
    make re
    echo "...Compilation complete"
}

# ADD TEST FOR MAX R0WS PER PAGE
# ADD TEST FOR MAX PAGES PER TABLE
# ADD TEST TO EXCEED FIELD LENGTHS
# ADD TEST LONG EMAIL AND NAME


meta_command_test() {
    echo ".exit" | ./$DATABASE > $META_TEST_FILE
    test_name="META COMMAND TEST"

 if diff "$META_TEST_FILE" $META_TEST_RESULT >/dev/null; then
        echo "✓ Test passed: $test_name"
        return 0
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        cat "$META_TEST_RESULT"
        echo "Actual output:"
        cat "$META_TEST_FILE"
        return 1
    fi
}


insert_command_test() {
    echo "INSERT  1 besnik test
.exit" | ./$DATABASE > $INSERT_TEST_FILE
    test_name="INSERT COMMAND TEST"

 if diff "$INSERT_TEST_FILE" $INSERT_TEST_RESULT >/dev/null; then
        echo "✓ Test passed: $test_name"
        return 0
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        cat "$INSERT_TEST_RESULT"
        echo "Actual output:"
        cat "$INSERT_TEST_FILE"
        return 1
    fi
}

insert_max_rows_test() {
    test_name="MAX INSERT TEST"
    test_file_input=test_files/insert_max_rows_input.txt
    test_file_output=test_files/insert_max_rows_output.txt
    test_file_result=test_files/insert_max_rows_result.txt

    cat "$test_file_input" | ./$DATABASE > $test_file_output
    if diff "$test_file_output" "$test_file_result"> /dev/null; then
        echo "✓ Test passed: $test_name"
        return 0
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        echo "~$test_file_result"
        echo "Actual output:"
        echo "~$test_file_output"
        return 1
    fi


}

select_command_test() {
    echo "INSERT 1 besnik besnik.konjuhaj@gmail.com
SELECT
.exit" | ./$DATABASE > $SELECT_TEST_FILE
    test_name="SELECT COMMAND TEST"

 if diff "$SELECT_TEST_FILE" $SELECT_TEST_RESULT >/dev/null; then
        echo "✓ Test passed: $test_name"
        return 0
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        cat "$SELECT_TEST_RESULT"
        echo "Actual output:"
        cat "$SELECT_TEST_FILE"
        return 1
    fi
}

persistance_test() {
    test_name="PERSISTANCE TEST"
    echo "INSERT 1 besnik besnik.konjuhaj@gmail.com
SELECT
.exit" | ./$DATABASE


    echo "SELECT
.exit" | ./$DATABASE > 

    if diff "$PERSISTANCE_TEST_FILE" $PERSISTANCE_TEST_RESULT > /dev/null; then
        echo "✓ Test passed: $test_name"
    else
        echo "✗ Test failed: $test_name"
        echo "Expected output:"
        cat "$PERSISTANCE_TEST_FILE"
        echo "Actual output:"
        cat "$PERSISTANCE_TEST_RESULT"
        return 1
}

cleanup() {
    make clean
    rm -rf $SELECT_TEST_FILE $META_TEST_FILE $INSERT_TEST_FILE
}


main () {
    compile_program
    meta_command_test
    insert_command_test
    select_command_test
    insert_max_rows_test
    
    case $TEST_TYPE in
        "clean")
            make clean
            ;;
    esac
    
    cleanup
}

main