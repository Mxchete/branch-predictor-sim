#!/bin/bash
A=$(./predictor 8 5 tests/mcf_trace.txt)
B="8 5 7.48"
ONE="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	ONE="FAIL"
fi

A=$(./predictor 8 5 tests/gobmk_trace.txt)
B="8 5 0.51"
TWO="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	TWO="FAIL"
fi

A=$(./predictor 4 2 tests/mcf_trace.txt)
B="4 2 26.86"
THREE="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	THREE="FAIL"
fi

A=$(./predictor 4 2 tests/gobmk_trace.txt)
B="4 2 0.87"
FOUR="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	FOUR="FAIL"
fi

A=$(./predictor 3 1 tests/mcf_trace.txt)
B="3 1 29.07"
FIVE="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	FIVE="FAIL"
fi

A=$(./predictor 3 1 tests/gobmk_trace.txt)
B="3 1 0.88"
SIX="PASS"
if [[ "$A" != "$B" ]]; then
	echo "Actual:"
	echo "$A"
	echo "Predicted:"
	echo "$B"
	SIX="FAIL"
fi

echo "TEST 1 RESULT=$ONE"
echo "TEST 2 RESULT=$TWO"
echo "TEST 3 RESULT=$THREE"
echo "TEST 4 RESULT=$FOUR"
echo "TEST 5 RESULT=$FIVE"
echo "TEST 6 RESULT=$SIX"
