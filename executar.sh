#!/bin/bash
echo "Compilando..."
make clean > /dev/null 2> /dev/null; make > /dev/null 2> /dev/null;
echo "Criando diretorios de saida..."
rm -rf output/*
echo "Executando 100 ..."
./bin/Release/AdaptiveMesh input/configuration_100.cfg 2> output/erros_100.txt&
echo "Executando 15..."
./bin/Release/AdaptiveMesh input/configuration_15.cfg 2> output/erros_15.txt&
echo "Executando 30..."
./bin/Release/AdaptiveMesh input/configuration_30.cfg 2> output/erros_30.txt&
echo "Executando 60..."
./bin/Release/AdaptiveMesh input/configuration_60.cfg 2> output/erros_60.txt
