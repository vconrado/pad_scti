#!/bin/bash


for i in 1 2; do
	for j in 1 2 3; do
		./app ../imgs/$i/original_${i}.png ../imgs/$i/template_${i}_${j}.png >> results/saida_${i}_${j}.csv 2>results/err_${i}_${j}.csv
	done 
done
 
