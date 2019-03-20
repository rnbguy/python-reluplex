#ifndef __NeuralReluplex_h__
#define __NeuralReluplex_h__

#include <Reluplex.h>

#include <vector>
#include <map>

class NeuralReluplex {
public:
Reluplex *reluplex;

void load_weights(std::vector<std::vector<std::vector<double> > > weights_) {
        weights = weights_;
        auto numVariables = 1ul;
        for(auto i = 0ul; i < weights.size(); i+=2) {
                if (i == 0) {
                        numVariables += weights[i].size();
                } else {
                        numVariables += weights[i].size() * 3;
                }
        }
        numVariables += weights[weights.size()-1][0].size() * 2;

        reluplex = new Reluplex(numVariables);

        reluplex->setLogging(true);
        reluplex->setLogging(false);

        // constant 1
        setLowerBound(get_one_constant(), 1.0);
        setUpperBound(get_one_constant(), 1.0);

        for(auto layer_i = 0ul; layer_i < weights.size(); layer_i += 2) {
                if (layer_i != 0) {
                        add_relu_pair(layer_i >> 1);
                }
                for(auto neuron_i = 0ul; neuron_i < weights[layer_i][0].size(); ++neuron_i) {
                        add_linear_relation(layer_i >> 1, neuron_i);
                }
        }
}

unsigned get_num_layer() {
        return (weights.size() >> 1) + 1;
}

unsigned get_num_input_variable() {
        return weights[0].size();
}

unsigned get_num_output_variable() {
        return weights[weights.size()-1][0].size();
}

unsigned get_input_i_variable(unsigned i) {
        return get_f_variable(0, i);
}

unsigned get_output_i_variable(unsigned i) {
        return get_b_variable(get_num_layer()-1, i);
}



unsigned layer_size(unsigned layer_i) {
        return weights[layer_i << 1].size();
}

unsigned get_one_constant() {
        return 0;
}

unsigned get_new_variable() {
        return f_variable.size() + b_variable.size() + a_variable.size() + 1;
}

unsigned get_f_variable(unsigned i, unsigned j) {
        auto it = f_variable.find(std::make_pair(i, j));

        if (it == f_variable.end()) {
                unsigned v = get_new_variable();
                f_variable.insert(std::make_pair(std::make_pair(i, j), v));
                return v;
        } else {
                return it->second;
        }
}

unsigned get_b_variable(unsigned i, unsigned j) {
        auto it = b_variable.find(std::make_pair(i, j));

        if (it == b_variable.end()) {
                unsigned v = get_new_variable();
                b_variable.insert(std::make_pair(std::make_pair(i, j), v));
                return v;
        } else {
                return it->second;
        }
}

unsigned get_a_variable(unsigned i, unsigned j) {
        auto it = a_variable.find(std::make_pair(i, j));

        if (it == a_variable.end()) {
                unsigned v = get_new_variable();
                a_variable.insert(std::make_pair(std::make_pair(i, j), v));
                markBasic(v);
                setLowerBound(v, 0.0);
                setUpperBound(v, 0.0);
                return v;
        } else {
                return it->second;
        }
}

void markBasic(unsigned v) {
        reluplex->markBasic(v);
}

void initializeCell(unsigned v1, unsigned v2, double val) {
        reluplex->initializeCell(v1, v2, val);
}

void setReluPair(unsigned v1, unsigned v2) {
        reluplex->setReluPair(v1, v2);
        setLowerBound(v2, 0.0);
}

void setUpperBound(unsigned v, double val) {
        reluplex->setUpperBound(v, val);
}

void setLowerBound(unsigned v, double val) {
        reluplex->setLowerBound(v, val);
}

void add_linear_relation(unsigned layer_i, unsigned neuron_i) {
        auto y_a = get_a_variable(layer_i + 1, neuron_i);
        auto y = get_b_variable(layer_i + 1, neuron_i);
        auto c = get_one_constant();

        initializeCell(y_a, y_a, -1.0);
        initializeCell(y_a, y, -1.0);

        for(auto i = 0ul; i < weights[layer_i << 1].size(); ++i) {
                auto x_i = get_f_variable(layer_i, i);
                initializeCell(y_a, x_i, weights[layer_i << 1][i][neuron_i]);
        }
        initializeCell(y_a, c, weights[(layer_i << 1) + 1][0][neuron_i]);
}

void add_relu_pair(unsigned layer_i) {
        for(auto i = 0u; i < layer_size(layer_i); ++i) {
                auto b = get_b_variable(layer_i, i);
                auto f = get_f_variable(layer_i, i);
                setReluPair(b, f);
        }
}

std::vector<double> getInputAssignment() {
        std::vector<double> assignment;

        for(auto i = 0ul; i < get_num_input_variable(); i++) {
                assignment.push_back(reluplex->getAssignment(get_input_i_variable(i)));
        }

        return assignment;
}

std::vector<double> getOutputAssignment() {
        std::vector<double> assignment;

        for(auto i = 0ul; i < get_num_output_variable(); i++) {
                assignment.push_back(reluplex->getAssignment(get_output_i_variable(i)));
        }

        return assignment;
}

~NeuralReluplex() {
        delete reluplex;
}

private:
std::vector<std::vector<std::vector<double> > > weights;
std::map<std::pair<unsigned, unsigned>, unsigned> f_variable;
std::map<std::pair<unsigned, unsigned>, unsigned> b_variable;
std::map<std::pair<unsigned, unsigned>, unsigned> a_variable;
};



#endif // __NeuralReluplex_h__
