/*
 * parser.h
 * implementation of CYK parser
 *
 * author: Playinf
 * playinf@stu.xmu.edu.cn
 *
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <string>
#include <rule.h>
#include <chart.h>
#include <rule_set.h>
#include <rule_tree.h>
#include <rule_finder.h>
#include <partial_rule.h>

class parser {
public:
    typedef unsigned int size_type;
    typedef std::vector<const symbol*> input_type;
    
    parser(std::vector<rule_tree*>* tree_vec);
    ~parser();

    void parse(input_type& input);
    hypothesis* get_hypothesis(size_type index);
    void clear();
private:
    void process_unknow_word(const symbol* sym);
    void parser_initialize();
    chart* table;
    input_type* input;
    rule_set* applicable_rule_set;
    std::vector<rule_tree*>* tree_vector;
    std::vector<rule_finder*> rule_lookup_manager;
};

#endif /* __PARSER_H__ */