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
        auto numVariables = 2ul;
        for(auto i = 0ul; i < weights.size(); i+=2) {
                if (i == 0) {
                        numVariables += weights[i].size();
                } else {
                        numVariables += weights[i].size() * 3;
                }
        }
        numVariables += weights[weights.size()-1][0].size() * 2;

        reluplex = new Reluplex(numVariables);

        printf("[PYTHON] Total number of variables in Reluplex %lu\n", numVariables);

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

unsigned get_contest_variable() {
        return 1;
}

unsigned get_new_variable() {
        return f_variable.size() + b_variable.size() + a_variable.size() + 2;
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
                unsigned v = f_variable.size() + b_variable.size() + a_variable.size() + 2;
                b_variable.insert(std::make_pair(std::make_pair(i, j), v));
                return v;
        } else {
                return it->second;
        }
}

unsigned get_a_variable(unsigned i, unsigned j) {
        auto it = a_variable.find(std::make_pair(i, j));

        if (it == a_variable.end()) {
                unsigned v = f_variable.size() + b_variable.size() + a_variable.size() + 2;
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
        printf("markBasic(%lu)\n", v);
        reluplex->markBasic(v);
}

void initializeCell(unsigned v1, unsigned v2, double val) {
        printf("initializeCell(%lu, %lu, %lf)\n", v1, v2, val);
        reluplex->initializeCell(v1, v2, val);
}

void setReluPair(unsigned v1, unsigned v2) {
        printf("setReluPair(%lu, %lu)\n", v1, v2);
        reluplex->setReluPair(v1, v2);
}

void setUpperBound(unsigned v, double val) {
        printf("setUpperBound(%lu, %lf)\n", v, val);
        reluplex->setUpperBound(v, val);
}

void setLowerBound(unsigned v, double val) {
        printf("setLowerBound(%lu, %lf)\n", v, val);
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

void a_wins_b(unsigned a, unsigned b) {
        auto output_a_variable = get_output_i_variable(a);
        auto output_b_variable = get_output_i_variable(b);

        auto contest_variable = get_contest_variable();

        // contest = output_a - output_b
        initializeCell(contest_variable, contest_variable, -1.0);
        initializeCell(contest_variable, output_a_variable, 1.0);
        initializeCell(contest_variable, output_b_variable, -1.0);

        // contest variable, if i beats j, then content = o[i] - o[j] > 0
        setLowerBound(1, 0.0);
        markBasic(contest_variable);
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
