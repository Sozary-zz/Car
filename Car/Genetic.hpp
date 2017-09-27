#pragma once
#include <algorithm>
#include "NeuralNetwork.hpp"
//https://www.youtube.com/watch?v=9tCySY6TLk8

#define ELITIST .15f // 10% of the first chromosoms from the population 
#define MID_ELITIST .60f
#define TRASH .25f
#define MUTATION 1.f
#define POPULATION_SIZE 10
#define MAX_RANK int(POPULATION_SIZE * ELITIST)
#define MID_RANK int(POPULATION_SIZE * MID_ELITIST)
#define DRAW_NEURAL_NETWORK false


struct Chromosom {
	NeuralNetwork * network;
	float fitness;
};

class Genetic
{
public:
	Genetic(int population_size);
	void evolve();
	~Genetic();

private:
	std::vector<Chromosom*> getCombinaisons(std::vector<Chromosom*>& a,  std::vector<Chromosom*>& b) const;
	void cleanPopulation();
	void mutate();

protected:
	std::vector<Chromosom*> m_population;
	int m_pop_size;
};

