#!/bin/bash

mkdir -p test/result

cat test/in/learner.in | bin/learner > test/result/chain.dat
bin/generator test/in/start_words test/result/chain.dat  1000 > test/result/out

cat test/in/learner_rus.in | bin/learner > test/result/chain_rus.dat
bin/generator test/in/start_words_rus test/result/chain_rus.dat  1000 > test/result/out_rus