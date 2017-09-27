#include "NeuralNetwork.hpp"
#include <iostream>

using namespace std;
using namespace sf;

NeuralNetwork::NeuralNetwork(vector<float> inputs, int layers, int neurons_by_layer, int outputs, float win_w, float win_h, bool draw, bool draw_text)
	:m_layers_number(layers), m_neurons_by_layer_number(neurons_by_layer), m_outputs_number(outputs), m_screen_size({ win_w,win_h }), m_draw(draw), m_draw_text(draw_text) {
	if (m_draw)
		this->m_main_font.loadFromFile("font\\arial.ttf");

	m_gen = new mt19937(m_rd());

	this->initStruct(inputs);
	this->makeBounds();
	this->clean();
	this->propagation();
	if (m_draw)
		this->build2D();
}

vector<float> NeuralNetwork::setInputs(const vector<float>& inputs)
{
	if (inputs.size() != m_inputs_number - 1)
		throw string("Different inputs from the original network");

	vector<float> res;
	for (int i = 0; i < m_inputs_number - 1; ++i)
		m_inputs[i]->value = inputs[i];

	this->clean();
	this->propagation();
	if (m_draw)
		this->build2D();

	for (int i = 0; i < m_outputs_number; ++i)
		res.push_back(m_outputs[i]->value);

	return res;
}

string NeuralNetwork::getChromosom() const
{
	return this->m_chromosom;
}

void NeuralNetwork::combineNetwork(const NeuralNetwork & other)
{
	string a = this->m_chromosom, b = other.m_chromosom;

	if (a.size() < b.size())
		for (int i = a.size(); i < b.size(); ++i)
			a.append(0);
	else if (a.size() > b.size())
		for (int i = b.size(); i < a.size(); ++i)
			b.append(0);
	int quart = .25f*a.size();
	int alea = this->randInt(quart, a.size() - quart);

	for (int i = alea; i < a.size(); ++i)
		a[i] = b[i];

	this->applyThisChromosom(a);
}

void NeuralNetwork::mutate()
{
	/*m_chromosom[this->randInt(0, m_chromosom.size() - 1)] = randInt(0, 1);
	this->applyThisChromosom(m_chromosom);*/
}

void NeuralNetwork::initStruct(vector<float> inputs)
{
	m_inputs_number = inputs.size() + 1; // bias

	this->findTheHeight();

	this->m_inputs = new Neuron*[m_inputs_number];
	for (int i = 0; i < m_inputs_number - 1; ++i)
		this->m_inputs[i] = new Neuron{ inputs[i] };
	this->m_inputs[m_inputs_number - 1] = new Neuron{ 1 };

	this->m_layers = new Neuron**[m_layers_number];

	for (int i = 0; i < m_layers_number; ++i)
	{
		this->m_layers[i] = new Neuron*[m_neurons_by_layer_number];
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			this->m_layers[i][j] = new Neuron;
	}

	this->m_outputs = new Neuron*[m_outputs_number];
	for (int i = 0; i < m_outputs_number; ++i)
		this->m_outputs[i] = new Neuron;
}

void NeuralNetwork::findTheHeight()
{
	m_max_neural_network_height = m_inputs_number;
	if (m_max_neural_network_height < m_neurons_by_layer_number)
		m_max_neural_network_height = m_neurons_by_layer_number;
	if (m_max_neural_network_height < m_outputs_number)
		m_max_neural_network_height = m_outputs_number;
}

void NeuralNetwork::makeBounds()
{
	for (int j = 0; j < m_inputs_number; ++j)
	{
		this->m_inputs[j]->targets = new Axon*[m_neurons_by_layer_number];
		for (int i = 0; i < m_neurons_by_layer_number; ++i)
			this->m_inputs[j]->targets[i] = new Axon{ m_inputs[j],m_layers[0][i],randFloat(-RANGE,RANGE) };
	}

	for (int layer = 0; layer < m_layers_number - 1; ++layer)
		for (int neuron = 0; neuron < m_neurons_by_layer_number; ++neuron) {
			this->m_layers[layer][neuron]->targets = new Axon*[m_neurons_by_layer_number];
			for (int i = 0; i < m_neurons_by_layer_number; ++i)
				this->m_layers[layer][neuron]->targets[i] = new Axon{ this->m_layers[layer][neuron],this->m_layers[layer + 1][i],randFloat(-RANGE,RANGE) };
		}

	for (int neuron = 0; neuron < m_neurons_by_layer_number; ++neuron) {
		m_layers[m_layers_number - 1][neuron]->targets = new Axon*[m_outputs_number];
		for (int i = 0; i < m_outputs_number; ++i)
			m_layers[m_layers_number - 1][neuron]->targets[i] = new Axon{ m_layers[m_layers_number - 1][neuron] ,m_outputs[i],randFloat(-RANGE,RANGE) };
	}
}

float NeuralNetwork::randFloat(float min, float max) const
{
	uniform_real_distribution<> dis(min, max);

	return dis(*m_gen);
}

void NeuralNetwork::build2D()
{
	build2DCells();
	build2DLinks();
	if (m_draw_text)
		build2DTexts();
}

void NeuralNetwork::build2DCells()
{
	const Vector2f separation = { m_screen_size.x / float(m_layers_number + 2),m_screen_size.y / float(m_max_neural_network_height) };
	float neuron_size = separation.x > separation.y ? separation.y / 4.f : separation.x / 4.f;

	for (int i = 0; i < m_inputs_number; ++i) {
		this->m_inputs[i]->drawing = new CircleShape;
		this->m_inputs[i]->drawing->setFillColor(Color::Blue);
		this->m_inputs[i]->drawing->setRadius(neuron_size);
		this->m_inputs[i]->drawing->setPosition({ separation.x - 2.f * neuron_size ,(m_screen_size.y / float(m_inputs_number))*i + neuron_size });
		this->m_inputs[i]->drawing->setOrigin({ neuron_size ,neuron_size });
	}

	for (int i = 0; i < m_layers_number; ++i) {
		for (int j = 0; j < m_neurons_by_layer_number; ++j) {
			this->m_layers[i][j]->drawing = new CircleShape;
			this->m_layers[i][j]->drawing->setFillColor(Color::Green);
			this->m_layers[i][j]->drawing->setRadius(neuron_size);
			this->m_layers[i][j]->drawing->setPosition({ (i + 2)*separation.x - 2.f * neuron_size ,(m_screen_size.y / float(m_neurons_by_layer_number))*j + neuron_size });
			this->m_layers[i][j]->drawing->setOrigin({ neuron_size ,neuron_size });
		}
	}
	for (int i = 0; i < m_outputs_number; ++i) {
		this->m_outputs[i]->drawing = new CircleShape;
		this->m_outputs[i]->drawing->setFillColor(Color::Red);
		this->m_outputs[i]->drawing->setRadius(neuron_size);
		this->m_outputs[i]->drawing->setPosition({ (m_layers_number + 2)*separation.x - 2.f * neuron_size ,(m_screen_size.y / float(m_outputs_number))*i + neuron_size });
		this->m_outputs[i]->drawing->setOrigin({ neuron_size ,neuron_size });

	}
}

void NeuralNetwork::build2DLinks()
{
	float w;
	for (int j = 0; j < m_inputs_number; ++j)
	{
		for (int i = 0; i < m_neurons_by_layer_number; ++i)
		{
			RectangleShape r;
			Vector2f a = m_inputs[j]->drawing->getPosition(), b = m_inputs[j]->targets[i]->target->drawing->getPosition();
			w = m_inputs[j]->targets[i]->weight;

			r.setFillColor(w > 0 ? Color::Green : Color::Red);
			w += RANGE;
			r.setSize({ static_cast<float>(this->getDistance(a,b)),CONVERSION_LINK*w });
			r.setPosition(a);
			r.setRotation(RAD_TO_DEG(atan2(b.y - a.y, b.x - a.x)));
			m_links.push_back(r);
		}
	}

	for (int layer = 0; layer < m_layers_number - 1; ++layer)
		for (int neuron = 0; neuron < m_neurons_by_layer_number; ++neuron) {
			for (int i = 0; i < m_neurons_by_layer_number; ++i) {
				RectangleShape r;
				Vector2f a = m_layers[layer][neuron]->drawing->getPosition(), b = m_layers[layer][neuron]->targets[i]->target->drawing->getPosition();
				w = m_layers[layer][neuron]->targets[i]->weight;

				r.setFillColor(w > 0 ? Color::Green : Color::Red);
				w += RANGE;
				r.setSize({ static_cast<float>(this->getDistance(a,b)),CONVERSION_LINK*w });
				r.setPosition(a);
				r.setRotation(RAD_TO_DEG(atan2(b.y - a.y, b.x - a.x)));
				m_links.push_back(r);
			}
		}

	for (int neuron = 0; neuron < m_neurons_by_layer_number; ++neuron)
		for (int i = 0; i < m_outputs_number; ++i) {
			RectangleShape r;
			Vector2f a = m_layers[m_layers_number - 1][neuron]->drawing->getPosition(), b = m_layers[m_layers_number - 1][neuron]->targets[i]->target->drawing->getPosition();
			w = m_layers[m_layers_number - 1][neuron]->targets[i]->weight;

			r.setFillColor(w > 0 ? Color::Green : Color::Red);
			w += RANGE;
			r.setSize({ static_cast<float>(this->getDistance(a,b)),CONVERSION_LINK*w });
			r.setPosition(a);
			r.setRotation(RAD_TO_DEG(atan2(b.y - a.y, b.x - a.x)));
			m_links.push_back(r);
		}

}

void NeuralNetwork::build2DTexts()
{
	for (int i = 0; i < m_inputs_number; ++i) {
		Text txt;
		txt.setFont(this->m_main_font);
		auto data = to_string(m_inputs[i]->value);
		data.resize(5);
		txt.setString(data);
		txt.setColor(Color::White);
		txt.setPosition(m_inputs[i]->drawing->getPosition() - Vector2f(m_inputs[i]->drawing->getRadius() / 2.f, m_inputs[i]->drawing->getRadius() / 4.f));
		txt.setCharacterSize(m_inputs[i]->drawing->getRadius() / 2.f);

		this->m_text.push_back(txt);
	}
	for (int layer = 0; layer < m_layers_number; ++layer)
		for (int neuron = 0; neuron < m_neurons_by_layer_number; ++neuron) {
			Text txt;
			txt.setFont(this->m_main_font);
			auto data = to_string(m_layers[layer][neuron]->value);
			data.resize(5);
			txt.setString(data);
			txt.setColor(Color::Red);
			txt.setPosition(m_layers[layer][neuron]->drawing->getPosition() - Vector2f(m_layers[layer][neuron]->drawing->getRadius() / 2.f, m_layers[layer][neuron]->drawing->getRadius() / 4.f));
			txt.setCharacterSize(m_layers[layer][neuron]->drawing->getRadius() / 2.f);

			this->m_text.push_back(txt);
		}

	for (int i = 0; i < m_outputs_number; ++i) {
		Text txt;
		txt.setFont(this->m_main_font);
		auto data = to_string(m_outputs[i]->value);
		data.resize(5);
		txt.setString(data);
		txt.setColor(Color::White);
		txt.setPosition(m_outputs[i]->drawing->getPosition() - Vector2f(m_outputs[i]->drawing->getRadius() / 2.f, m_outputs[i]->drawing->getRadius() / 4.f));
		txt.setCharacterSize(m_outputs[i]->drawing->getRadius() / 2.f);

		this->m_text.push_back(txt);
	}

}

double NeuralNetwork::sigmoid(float x) const
{
	return 1.f / (1.f + exp(-x));
}

void NeuralNetwork::propagation()
{
	for (int i = 0; i < m_inputs_number; ++i) // sum of all the weights
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			m_inputs[i]->targets[j]->target->value += m_inputs[i]->targets[j]->weight;

	for (int i = 0; i < m_neurons_by_layer_number; ++i) // apply logistic function on them
		m_layers[0][i]->value = this->sigmoid(m_layers[0][i]->value);

	for (int i = 0; i < m_layers_number - 1; ++i) {
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			for (int k = 0; k < m_neurons_by_layer_number; ++k)
				m_layers[i][j]->targets[k]->target->value += m_layers[i][j]->targets[k]->weight; // the value of each next layer's neuron is changed 

		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			m_layers[i + 1][j]->value = this->sigmoid(m_layers[i + 1][j]->value); // apply logistic function on them
	}

	for (int i = 0; i < m_neurons_by_layer_number; ++i)
		for (int j = 0; j < m_outputs_number; ++j)
			m_layers[m_layers_number - 1][i]->targets[j]->target->value += m_layers[m_layers_number - 1][i]->targets[j]->weight;

	for (int j = 0; j < m_outputs_number; ++j)
		m_outputs[j]->value = this->sigmoid(m_outputs[j]->value); // apply logistic function on them

	this->computeChromosom();
}

void NeuralNetwork::clean()
{
	for (int i = 0; i < m_layers_number; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			this->m_layers[i][j]->value = 0;


	for (int i = 0; i < m_outputs_number; ++i)
		this->m_outputs[i]->value = 0;
}

void NeuralNetwork::computeChromosom()
{
	m_chromosom = "";

	for (int i = 0; i < m_inputs_number; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			m_chromosom.append(this->floatToBinary(m_inputs[i]->targets[j]->weight + RANGE)); // + RANGE cause I don t want negative res

	for (int i = 0; i < m_layers_number - 1; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			for (int k = 0; k < m_neurons_by_layer_number; ++k)
				m_chromosom.append(this->floatToBinary(m_layers[i][j]->targets[k]->weight + RANGE));

	for (int i = 0; i < m_neurons_by_layer_number; ++i)
		for (int j = 0; j < m_outputs_number; ++j)
			m_chromosom.append(this->floatToBinary(m_layers[m_layers_number - 1][i]->targets[j]->weight + RANGE));

}

void NeuralNetwork::applyThisChromosom(const std::string & chr)
{
	int cursor = 0;
	auto data = this->extractDataFromChromosom(chr);

	// I hope they have a 32 as size...
	for (int i = 0; i < m_inputs_number; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			m_inputs[i]->targets[j]->weight = data[cursor++];

	for (int i = 0; i < m_layers_number - 1; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			for (int k = 0; k < m_neurons_by_layer_number; ++k)
				m_layers[i][j]->targets[k]->weight = data[cursor++];

	for (int i = 0; i < m_neurons_by_layer_number; ++i)
		for (int j = 0; j < m_outputs_number; ++j)
			m_layers[m_layers_number - 1][i]->targets[j]->weight = data[cursor++];

}

std::vector<float> NeuralNetwork::extractDataFromChromosom(const std::string & chr) const
{

	string buffer = "";
	int cursor = 0;
	vector<float> data;

	for (int i = 0; i < chr.size(); ++i) {
		if (cursor == 32) {
			data.push_back(binaryToFloat(buffer));
			buffer.clear();
			cursor = 0;
		}
		buffer += chr[i];
		cursor++;
	}
	data.push_back(binaryToFloat(buffer)); // necessary?

	return data;
}

double NeuralNetwork::getDistance(const sf::Vector2f & a, const sf::Vector2f & b)
{
	return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

int NeuralNetwork::randInt(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

string NeuralNetwork::floatToBinary(float f)
{
	union
	{
		float input;   // assumes sizeof(float) == sizeof(int)
		int   output;
	}    data;

	data.input = f;

	std::bitset<sizeof(float) * CHAR_BIT>   bits(data.output);

	std::string mystring = bits.to_string<char,
		std::char_traits<char>,
		std::allocator<char> >();

	return mystring;
}

int NeuralNetwork::binary2Hex(const std::string & str)
{

	std::bitset<32> set(str);
	int hex = set.to_ulong();

	return hex;
}

float NeuralNetwork::binaryToFloat(const std::string & str)
{
	int HexNumber = binary2Hex(str);

	bool negative = !!(HexNumber & 0x80000000);
	int  exponent = (HexNumber & 0x7f800000) >> 23;
	int sign = negative ? -1 : 1;

	// Subtract 127 from the exponent
	exponent -= 127;

	// Convert the mantissa into decimal using the
	// last 23 bits
	int power = -1;
	float total = 0.0;
	for (int i = 0; i < 23; i++)
	{
		int c = str[i + 9] - '0';
		total += (float)c * (float)pow(2.0, power);
		power--;
	}
	total += 1.0;

	float value = sign * (float)pow(2.0, exponent) * total;

	return value;
}

void NeuralNetwork::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (auto&x : m_links)
		target.draw(x, states);

	for (int i = 0; i < m_inputs_number; ++i)
		target.draw(*this->m_inputs[i]->drawing, states);

	for (int i = 0; i < m_layers_number; ++i)
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
			target.draw(*this->m_layers[i][j]->drawing, states);

	for (int i = 0; i < m_outputs_number; ++i)
		target.draw(*this->m_outputs[i]->drawing, states);

	for (auto&x : m_text)
		target.draw(x, states);
}

NeuralNetwork::~NeuralNetwork()
{
	delete m_gen;

	for (int i = 0; i < m_inputs_number; ++i)
	{
		if (m_draw)
			delete this->m_inputs[i]->drawing;
		delete this->m_inputs[i];
	}

	delete this->m_inputs;

	for (int i = 0; i < m_layers_number; ++i)
	{
		for (int j = 0; j < m_neurons_by_layer_number; ++j)
		{
			if (m_draw)

				delete this->m_layers[i][j]->drawing;
			delete this->m_layers[i][j];
		}
		delete this->m_layers[i];
	}
	delete this->m_layers;

	for (int i = 0; i < m_outputs_number; ++i) {
		if (m_draw)

			delete this->m_outputs[i]->drawing;
		delete this->m_outputs[i];
	}
	delete this->m_outputs;
	m_links.clear();
	m_text.clear();
}
