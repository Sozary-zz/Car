
//https://www.youtube.com/watch?v=0jh-jlWfKwo
#pragma once
#include <vector>
#include <random>
#include <SFML\Graphics.hpp>
#include <bitset>

#define PI 3.141592654f
#define DEG_TO_RAD(x) (x*PI)/180.f
#define RAD_TO_DEG(x) (x*180.f)/PI
#define MAX_WIDTH_LINKS 1.f
#define RANGE 2.f
#define CONVERSION_LINK 5.f/RANGE


struct Axon;
struct Neuron {
	float value;
	Axon** targets;
	sf::CircleShape * drawing;
};

struct Axon {
	Neuron* father;
	Neuron* target;
	float weight;
};

class NeuralNetwork : public sf::Drawable, public sf::Transformable // Backpropagation not handled here. This neural network shall only be used with
															//  a genetic algorithm.
{
public:
	NeuralNetwork(std::vector<float> inputs, int layers, int neurons_by_layer, int outputs, float win_w, float win_h, bool draw = true, bool draw_text = false);
	std::vector<float> setInputs(const std::vector<float>& inputs); // and compute the result and gives outputs
	std::string getChromosom() const;
	void combineNetwork(const NeuralNetwork& other);
	void mutate();

	static int binary2Hex(const std::string & str);
	static std::string floatToBinary(float f);
	static float binaryToFloat(const std::string& str);
	static double getDistance(const sf::Vector2f& a, const sf::Vector2f&b);
	static int randInt(int min, int max);
	~NeuralNetwork();

private:
	void initStruct(std::vector<float> inputs); // creates the neural network structure (allocations for arrays)
	void findTheHeight(); // [for drawing] find the height of the neural network
	void makeBounds(); // associates the neurons from the last layer to the next layer
	float randFloat(float min, float max) const; // ...
	void build2D(); // handles the 2D creation of the neural network
	void build2DCells(); // draws the circles which represent the neurons
	void build2DLinks(); // draws the bounds
	void build2DTexts(); // draws the values of each neurons
	double sigmoid(float x)const; // 1/(1+exp(x)) it s the logistic function
	void propagation(); // propagation of the information through the neural network
	void clean(); // clean all the neurons except ones from the input layer
	void computeChromosom(); // convert into a binary string
	void applyThisChromosom(const std::string& chr);
	std::vector<float> extractDataFromChromosom(const std::string& chr) const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	int m_inputs_number;
	int m_layers_number;
	int m_neurons_by_layer_number;
	int m_outputs_number;
	int m_max_neural_network_height;

	Neuron ** m_inputs;
	Neuron *** m_layers;
	Neuron ** m_outputs;
	std::random_device m_rd;
	std::mt19937* m_gen;
	sf::Vector2f m_screen_size;

	std::vector<sf::RectangleShape> m_links;
	sf::Font m_main_font;
	std::vector<sf::Text> m_text;
	std::string m_chromosom;

	bool m_draw;
	bool m_draw_text;
};


