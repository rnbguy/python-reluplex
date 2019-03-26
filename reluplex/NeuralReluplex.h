#ifndef __NeuralReluplex_h__
#define __NeuralReluplex_h__

#include <ReluplexGate.h>
#include <Reluplex.h>

#include <vector>
#include <map>

class NeuralReluplex {
public:
ReluplexGate *reluplexgate;

void load_weights(std::vector<std::vector<std::vector<double> > > weights_) {
        weights = weights_;

        reluplexgate = new ReluplexGate();

        input_variables = new std::vector<unsigned>();
        for(auto i = 0ul; i < weights[0].size(); ++i) {
                input_variables->push_back(reluplexgate->get_new_variable());
        }

        std::vector<unsigned> curr(input_variables->begin(), input_variables->end());

        for(auto layer_i = 0ul; layer_i < weights.size(); layer_i += 2) {
                if (layer_i != 0) {
                        std::vector<unsigned> curr_next;
                        for(auto a: curr) {
                                curr_next.push_back(reluplexgate->relu(a));
                        }
                        curr.swap(curr_next);
                }
                std::vector<unsigned> curr_next;
                for(auto neuron_i = 0ul; neuron_i < weights[layer_i][0].size(); ++neuron_i) {
                        std::vector<double> w;
                        for(auto w_l: weights[layer_i]) {
                                w.push_back(w_l[neuron_i]);
                        }
                        curr_next.push_back(reluplexgate->apply_linear_op(curr, w, weights[layer_i+1][0][neuron_i]));
                }
                curr.swap(curr_next);
        }

        output_variables = new std::vector<unsigned>(curr.begin(), curr.end());
}

void input_interval_box(std::vector<double> l_bound, std::vector<double> u_bound) {
        reluplexgate->from_interval_box(*input_variables, l_bound, u_bound);
}

void input_neg_interval_box(std::vector<double> l_bound, std::vector<double> u_bound) {
        reluplexgate->not_from_interval_box(*input_variables, l_bound, u_bound);
}

void i_output_does_not_win(unsigned i) {
        std::vector<unsigned> others;
        for(auto a: *output_variables) {
                if(a != output_variables->at(i)) {
                        others.push_back(a);
                }
        }
        auto max_others = reluplexgate->max(others);
        reluplexgate->greater_than(max_others, output_variables->at(i));
}

inline void greater_than_equal(unsigned a, unsigned b) {
        reluplexgate->greater_than_equal(a, b);
}

inline void greater_than(unsigned a, unsigned b) {
        reluplexgate->greater_than(a, b);
}

void build_reluplex() {
        reluplex = reluplexgate->get_reluplex();
}

Reluplex::FinalStatus solve() {
        return reluplex->solve();
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
        return input_variables->at(i);
}

unsigned get_output_i_variable(unsigned i) {
        return output_variables->at(i);
}

unsigned layer_size(unsigned layer_i) {
        return weights[layer_i << 1].size();
}

void setName(unsigned v, const char * s) {
        // printf("setName(%u, %s)\n", v, s);
        reluplex->setName(v, s);
}

std::vector<double> getInputAssignment() {
        std::vector<double> assignment;

        for(auto v: *input_variables) {
                assignment.push_back(reluplex->getAssignment(v));
        }

        return assignment;
}

std::vector<double> getOutputAssignment() {
        std::vector<double> assignment;

        for(auto v: *output_variables) {
                assignment.push_back(reluplex->getAssignment(v));
        }

        return assignment;
}

~NeuralReluplex() {
        delete input_variables;
        delete output_variables;
        delete reluplexgate;
        delete reluplex;
}

private:
Reluplex *reluplex;
std::vector<std::vector<std::vector<double> > > weights;
std::vector<unsigned> * input_variables;
std::vector<unsigned> * output_variables;
};



#endif // __NeuralReluplex_h__
