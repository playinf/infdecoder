/*
 * feature.h
 *
 *
 * author: Playinf
 * email: playinf@stu.xmu.edu.cn
 *
 */
#ifndef __FEATURE_H__
#define __FEATURE_H__

#include <feature_function.h>

class state;
class hypothesis;

class feature {
public:
    feature(unsigned int id);
    ~feature();

    float get_score() const;
    float get_weight() const;
    state* get_state() const;
    unsigned int get_id() const;

    void add_state(state* s);
    void evaluate(const hypothesis* hypo);
private:
    float score;
    float weight;
    state* feature_state;
    unsigned int feature_id;
    feature_function function;
};

#endif /* __FEATURE_H__ */
