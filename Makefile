clean:
	@rm ./predictor

build-opt:
	@gcc -o predictor -O3 -lm branch_sim.c

build-gen:
	@gcc -o predictor -lm branch_sim.c
