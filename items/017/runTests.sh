#!/bin/bash

SCRIPT_DIR=`dirname $0`

# This shell script executes all Tests in order
${SCRIPT_DIR}/build/IteratorTests/TestA_dereference/TestA_dereference

${SCRIPT_DIR}/build/IteratorTests/TestB_increment/TestB_increment

${SCRIPT_DIR}/build/IteratorTests/TestC_decrement/TestC_decrement

${SCRIPT_DIR}/build/IteratorTests/TestD_decrement_end/TestD_decrement_end

${SCRIPT_DIR}/build/IteratorTests/TestE_arrow_operator/TestE_arrow_operator

${SCRIPT_DIR}/build/IteratorTests/TestF_explicit_loop/TestF_explicit_loop

${SCRIPT_DIR}/build/IteratorTests/TestG_range_based_loop/TestG_range_based_loop

${SCRIPT_DIR}/build/IteratorTests/TestH_constness_check/TestH_constness_check

${SCRIPT_DIR}/build/IteratorTests/TestI_STL_compatibility/TestI_STL_compatibility

