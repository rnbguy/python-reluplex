#ifndef __ReluplexGate_h__
#define __ReluplexGate_h__

#include <Reluplex.h>

#include <vector>
#include <map>

class ReluplexGate {
public:
ReluplexGate() : n_variable(0),
        cell_constraints(new std::vector<std::pair<std::pair<unsigned, unsigned>, double> > ()),
        relu_constraints(new std::vector<std::pair<unsigned, unsigned> >()),
        lb_constraints(new std::vector<std::pair<unsigned, double> >()),
        ub_constraints(new std::vector<std::pair<unsigned, double> >()),
        basic_constraints(new std::vector<unsigned>())
{
        auto one = get_new_variable();
        set_bound(one, 1, 1);

}
inline unsigned get_new_variable() {
        return n_variable++;
}
inline unsigned get_basic_variable() {
        auto v = get_new_variable();
        mark_basic(v);
        return v;
}
inline void set_cell_constraints(unsigned u, unsigned v, double w) {
        cell_constraints->push_back(std::make_pair(std::make_pair(u, v), w));
}
unsigned relu(unsigned v) {
        auto rv = get_new_variable();
        set_lower_bound(rv, 0);
        relu_constraints->push_back(std::make_pair(v, rv));
        return rv;
}

inline void set_lower_bound(unsigned u, double b) {
        lb_constraints->push_back(std::make_pair(u, b));
}

inline void set_upper_bound(unsigned u, double b) {
        ub_constraints->push_back(std::make_pair(u, b));
}

inline void mark_basic(unsigned u) {
        basic_constraints->push_back(u);
}

inline void set_bound(unsigned a, double l, double u) {
        set_lower_bound(a, l);
        set_upper_bound(a, u);
}

unsigned get_constant(double c) {
        auto v = get_new_variable();
        set_upper_bound(v, c);
        set_lower_bound(v, c);
        return v;
}

unsigned apply_linear_op(std::vector<unsigned> v, std::vector<double> w, double b) {
        auto a = get_new_variable();
        auto a_b = get_basic_variable();
        set_bound(a_b, 0, 0);
        set_cell_constraints(a_b, a_b, -1);
        set_cell_constraints(a_b, a, -1);
        for(auto i = 0u; i < v.size(); ++i) {
                if(w[i] != 0) {
                        set_cell_constraints(a_b, v[i], w[i]);
                }
        }
        if(b != 0) {
                set_cell_constraints(a_b, get_one(), b);
        }
        return a;
}

inline unsigned neg(unsigned v) {
        return apply_linear_op({v}, {-1}, 0);
}

inline unsigned get_one() {
        return 0;
}

unsigned mod(unsigned a) {
        auto a_p = relu(a);
        auto a_n = relu(neg(a));
        return apply_linear_op({a_p, a_n}, {1, 1}, 0);
}

unsigned max_pair(unsigned a, unsigned b) {
        auto diff_a_b = apply_linear_op({a, b}, {1, -1}, 0);
        auto diff_b_a = apply_linear_op({b, a}, {1, -1}, 0);
        auto relu_diff_a_b = relu(diff_a_b);
        auto relu_diff_b_a = relu(diff_b_a);
        return apply_linear_op({a, b, relu_diff_a_b, relu_diff_b_a}, {.5, .5, .5, .5}, 0);
}

unsigned min_pair(unsigned a, unsigned b) {
        auto diff_a_b = apply_linear_op({a, b}, {1, -1}, 0);
        auto diff_b_a = apply_linear_op({b, a}, {1, -1}, 0);
        auto relu_diff_a_b = relu(diff_a_b);
        auto relu_diff_b_a = relu(diff_b_a);
        return apply_linear_op({a, b, relu_diff_a_b, relu_diff_b_a}, {.5, .5, -.5, -.5}, 0);
}

unsigned max(std::vector<unsigned> v) {
        auto r = *v.begin();
        for(auto it = std::next(v.begin()); it != v.end(); ++it) {
                r = max_pair(r, *it);
        }
        return r;
}

unsigned min(std::vector<unsigned> v) {
        auto r = *v.begin();
        for(auto it = std::next(v.begin()); it != v.end(); ++it) {
                r = min_pair(r, *it);
        }
        return r;
}

void greater_than_equal(unsigned a, unsigned b) {
        auto diff_a_b = apply_linear_op({a, b}, {1, -1}, 0);
        set_lower_bound(diff_a_b, 0);
}

void greater_than(unsigned a, unsigned b) {
        auto diff_a_b = apply_linear_op({a, b}, {1, -1}, 0);
        // bad hack
        set_lower_bound(diff_a_b, 0.01);
}

void from_interval_box(std::vector<unsigned> x, std::vector<double> l, std::vector<double> u) {
        std::vector<unsigned> y;
        for(auto i = 0u; i < x.size(); ++i) {
                set_lower_bound(x[i], l[i]);
                set_upper_bound(x[i], u[i]);
        }
}

void not_from_interval_box(std::vector<unsigned> x, std::vector<double> l, std::vector<double> u) {
        std::vector<unsigned> y;
        for(auto i = 0u; i < x.size(); ++i) {
                // x[i] - l[i]
                y.push_back(apply_linear_op({x[i]}, {1}, -l[i]));
                // u[i] - x[i]
                y.push_back(apply_linear_op({x[i]}, {-1}, u[i]));
        }
        // min(x[i] - l[i], u[i] - x[i])
        auto y_min = min(y);
        // x[i] - l[i] <= 0 or u[i] - x[i] <= 0
        set_upper_bound(y_min, 0);
}

Reluplex* get_reluplex() {
        Reluplex* r = new Reluplex(n_variable);
        for(auto v: *cell_constraints) {
                // printf("initializeCell(%u, %u, %lf)\n", v.first.first, v.first.second, v.second);
                r->initializeCell(v.first.first, v.first.second, v.second);
        }
        for(auto v: *relu_constraints) {
                // printf("setReluPair(%u, %u)\n", v.first, v.second);
                r->setReluPair(v.first, v.second);
        }
        for(auto v: *lb_constraints) {
                // printf("setLowerBound(%u, %lf)\n", v.first, v.second);
                r->setLowerBound(v.first, v.second);
        }
        for(auto v: *ub_constraints) {
                // printf("setUpperBound(%u, %lf)\n", v.first, v.second);
                r->setUpperBound(v.first, v.second);
        }
        for(auto v: *basic_constraints) {
                // printf("markBasic(%u)\n", v);
                r->markBasic(v);
        }

        return r;
}

~ReluplexGate() {
        delete cell_constraints;
        delete relu_constraints;
        delete lb_constraints;
        delete ub_constraints;
        delete basic_constraints;
}

private:
unsigned n_variable;
std::vector<std::pair<std::pair<unsigned, unsigned>, double> > *cell_constraints;
std::vector<std::pair<unsigned, unsigned> > *relu_constraints;
std::vector<std::pair<unsigned, double> > *lb_constraints;
std::vector<std::pair<unsigned, double> > *ub_constraints;
std::vector<unsigned> *basic_constraints;
};

#endif // __ReluplexGate_h__
