/* rule_finder.cpp */
#include <chart.h>
#include <rule_tree.h>
#include <chart_cell.h>
#include <rule_finder.h>
#include <partial_rule.h>
#include <translation_option.h>

rule_finder::rule_finder(rule_tree* tree)
{
    this->tree = tree;
    this->table = nullptr;
    this->input = nullptr;
    this->partial_rule_table = nullptr;
}

rule_finder::~rule_finder()
{
    clear();
}

void rule_finder::set_span_limit(size_type limit)
{
    span_limit = limit;
}

rule_finder::size_type rule_finder::get_span_limit() const
{
    return span_limit;
}

void rule_finder::initialize(input_type* input, chart* table)
{
    const rt_node* root = tree->get_root();
    unsigned int size = input->size();
    this->input = input;
    this->table = table;
    partial_rule_table = new std::vector<partial_rule_set*>(size);

    for (unsigned int i = 0; i < size; i++) {
        partial_rule* initial_rule = new partial_rule(root, nullptr, 0);
        partial_rule_set* rules = new partial_rule_set(size - i + 1);

        rules->insert_partial_rule(initial_rule);
        (*partial_rule_table)[i] = rules;
    }
}

void rule_finder::clear()
{
    if (partial_rule_table) {
        unsigned int size = partial_rule_table->size();
        std::vector<partial_rule_set*>& rule_table = *partial_rule_table;

        for (unsigned int i = 0; i < size; i++)
            delete rule_table[i];
        delete partial_rule_table;
        partial_rule_table = nullptr;
    }
}

void rule_finder::find(size_type start, size_type end,
    translation_option_set& set)
{
    partial_rule_set* rule_set = partial_rule_table->at(start);
    auto& expandable_rule_list = *rule_set->get_expandable_rule_list();

    for (unsigned int i = 0; i < expandable_rule_list.size(); i++) {
        partial_rule* r = expandable_rule_list[i];
        unsigned int rule_start = start + r->get_length();
        unsigned int rule_end;
        unsigned int len = end - start + 1;
        const rt_node* node = r->get_node();

        // already covered
        if (rule_start > end)
            continue;

        // expand rules

        // match a terminal symbol
        if (rule_start == end) {
            const symbol* sym = input->at(end);
            rt_node* child = tree->find_child(node, sym);

            if (child != nullptr) {
                partial_rule* new_rule;
                new_rule = new partial_rule(child, r, len);
                rule_set->insert_partial_rule(new_rule);
            }
        }

        // we do not allow match unary non-lexical rules here
        if (rule_start == start && end > start)
            rule_end = end - 1;
        else
            rule_end = end;

        len = rule_end - start + 1;

        // match a nonterminal symbol
        chart_cell* cell = table->get_cell(rule_start, rule_end);
        auto& src_set = cell->get_source_start_symbol_set();
        auto& tgt_set = cell->get_target_start_symbol_set();

        for (const symbol* src_sym : src_set) {
            for (const symbol* tgt_sym : tgt_set) {
                rt_node* child = tree->find_child(node, src_sym, tgt_sym);

                if (child != nullptr) {
                    auto hypothesis_list = cell->get_hypothesis_list(tgt_sym);
                    partial_rule* new_rule = new partial_rule(child, r, len);
                    new_rule->set_hypothesis_list(hypothesis_list);
                    rule_set->insert_partial_rule(new_rule);
                }
            }
        }
    }

    // insert all applicable rules into output parameter
    unsigned int position = end - start + 1;
    auto& applicable_rule_list = *rule_set->get_partial_rule_list(position);
    size_type list_size = applicable_rule_list.size();

    for (size_type i = 0; i < list_size; i++) {
        partial_rule* r = applicable_rule_list[i];
        auto applicable_rules = r->get_rule();

        if (applicable_rules == nullptr)
            continue;

        set.add_complete_rule(r);
    }
}
