/* partial_rule.cpp */
#include <rule.h>
#include <symbol.h>
#include <rule_tree.h>
#include <hypothesis.h>
#include <partial_rule.h>

partial_rule::partial_rule()
{
    length = 0;
    node = nullptr;
    parent = nullptr;
    nonterminal_hypothesis_list = nullptr;
}

partial_rule::partial_rule(const rt_node* node, partial_rule* parent,
    unsigned int len)
{
    length = len;
    this->node = node;
    this->parent = parent;
    nonterminal_hypothesis_list = nullptr;
}

partial_rule::~partial_rule()
{
    // do nothing
}

unsigned int partial_rule::get_length() const
{
    return length;
}

const rt_node* partial_rule::get_node() const
{
    return node;
}

const partial_rule* partial_rule::get_parent() const
{
    return parent;
}

const std::vector<const rule*>* partial_rule::get_rule() const
{
    if (node != nullptr) {
        return node->get_rules();
    }

    return nullptr;
}

std::vector<hypothesis*>* partial_rule::get_hypothesis_list() const
{
    return nonterminal_hypothesis_list;
}

bool partial_rule::is_expandable() const
{
    if (node)
        return !node->is_leaf();

    return false;
}

void partial_rule::set_hypothesis_list(std::vector<hypothesis*>* hlist)
{
    nonterminal_hypothesis_list = hlist;
}

partial_rule_set::partial_rule_set(size_type size) : partial_rules(size)
{
    // do nothing
}

partial_rule_set::~partial_rule_set()
{
    auto size = partial_rules.size();

    for (unsigned int i = 0; i < size; i++) {
        auto& rule_list = partial_rules[i];
        auto list_size = rule_list.size();

        for (unsigned int j = 0; j < list_size; j++)
            delete rule_list[j];
    }
}

const partial_rule_set::list_type*
    partial_rule_set::get_partial_rule_list(unsigned int end_pos) const
{
    return &partial_rules[end_pos];
}

void partial_rule_set::insert_partial_rule(partial_rule* r)
{
    unsigned int pos = r->get_length();
    partial_rules[pos].push_back(r);

    if (r->is_expandable())
        expandable_rule_list.push_back(r);
}

const partial_rule_set::list_type*
    partial_rule_set::get_expandable_rule_list() const
{
    return &expandable_rule_list;
}
