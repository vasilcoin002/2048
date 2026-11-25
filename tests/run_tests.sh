#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"

echo "=========================================="
echo "    Running B6B36PCC 2048 Test Suite      "
echo "=========================================="

# 1. Run C++ Unit Tests
echo -e "\n[1/2] Running Unit Tests..."
"${BUILD_DIR}/unit_tests"

# 2. Run Sample Input Scenarios
echo -e "\n[2/2] Running Sample Input Scenarios..."

echo -n "  Testing test_win.in (Win condition)... "
"${BUILD_DIR}/2048" -s 3 -t 8 --seed 100 < "${SCRIPT_DIR}/inputs/test_win.in" > /dev/null
echo -e "\033[92mOK\033[0m"

echo -n "  Testing test_undo.in (Undo feature)... "
"${BUILD_DIR}/2048" < "${SCRIPT_DIR}/inputs/test_undo.in" > /dev/null
echo -e "\033[92mOK\033[0m"

echo -n "  Testing test_restart.in (Restart feature)... "
"${BUILD_DIR}/2048" < "${SCRIPT_DIR}/inputs/test_restart.in" > /dev/null
echo -e "\033[92mOK\033[0m"

echo -n "  Testing test_invalid_keys.in (Edge case: invalid input)... "
"${BUILD_DIR}/2048" < "${SCRIPT_DIR}/inputs/test_invalid_keys.in" > /dev/null
echo -e "\033[92mOK\033[0m"

echo -n "  Testing test_moves.in (Movement in all 4 directions)... "
"${BUILD_DIR}/2048" < "${SCRIPT_DIR}/inputs/test_moves.in" > /dev/null
echo -e "\033[92mOK\033[0m"

echo "=========================================="
echo -e "\033[92mAll tests completed successfully!\033[0m"
echo "=========================================="
