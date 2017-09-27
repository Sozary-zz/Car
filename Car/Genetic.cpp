#include "Genetic.hpp"
#include<iostream>
using namespace std;

Genetic::Genetic(int population_size) :m_pop_size(population_size)
{
}

void Genetic::evolve()
{
	sort(m_population.begin(), m_population.end(), [](Chromosom* a, Chromosom* b) {
		return a->fitness < b->fitness;
	});
	vector<Chromosom*> elite, mid_elite, combinaison;
	for (int i = 0; i < MAX_RANK*POPULATION_SIZE; ++i)
		elite.push_back(new Chromosom{ m_population[i]->network,m_population[i]->fitness });

	for (int i = MAX_RANK; i < MID_RANK; ++i)
		mid_elite.push_back(new Chromosom{ m_population[i]->network,m_population[i]->fitness });
	cleanPopulation();

	combinaison = getCombinaisons(elite, mid_elite);


	for (int i = 0; i < combinaison.size(); ++i)
		m_population.push_back(combinaison[i]);
	for (int i = combinaison.size(); i < POPULATION_SIZE; ++i)
	{
		m_population.push_back(new Chromosom{ new NeuralNetwork({ 0,0,0,0,0 }, 2, 4, 2, 800, 600,DRAW_NEURAL_NETWORK),0 });
		m_population.back()->network->move({ 350,50 });
	}

	mutate();

}

Genetic::~Genetic()
{
	for (auto&x : m_population)
	{
		delete x->network;
		delete x;
	}
}

vector<Chromosom*> Genetic::getCombinaisons(vector<Chromosom*>& a, vector<Chromosom*>& b) const
{
	vector<Chromosom*> res;

	const int elite_size = a.size();
	vector<int> avaible_places;
	int alea;

	for (int i = 0; i < a.size(); ++i)
		avaible_places.push_back(i);

	for (int i = 0; i < elite_size; ++i) {
		alea = NeuralNetwork::randInt(0, avaible_places.size());
		b[i]->network->combineNetwork(*a[alea]->network);
		delete a[alea]->network;
		avaible_places.erase(avaible_places.begin() + alea);
	}


	for (int i = 0; i < b.size(); ++i)
	{
		res.push_back(new Chromosom{ b[i]->network, 0 });
		delete b[i];
	}
	a.clear();
	b.clear();

	return res;
}

void Genetic::cleanPopulation()
{
	for (int i = m_population.size() - 1; i >= 0; --i) {
		if (i > (MAX_RANK + MID_RANK)) {
			delete m_population[i]->network;
		
		}
		delete m_population[i];
	}
	m_population.clear();
}

void Genetic::mutate()
{
	for (auto&x : m_population)
		if (NeuralNetwork::randInt(0, 100) <= MUTATION)
			x->network->mutate();
}
