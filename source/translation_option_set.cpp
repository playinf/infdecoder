/* translation_option_set.cpp */
#include <limits>
#include <algorithm>
#include <rule.h>
#include <symbol.h>
#include <rule.h>
#include <rule_tree.h>
#include <hypothesis.h>
#include <partial_rule.h>
#include <translation_option.h>

translation_option_set::translation_option_set()
{
    /* do nothing */
}

translation_option_set::~translation_option_set()
{
    clear();
}

translation_option* translation_option_set::get_translation_option(size_type i)
    const
{
    return applicable_rule_set[i];
}

void translation_option_set::clear()
{
    unsigned int i;
    unsigned int size = applicable_rule_set.size();

    for (i = 0; i < size; i++)
        delete applicable_rule_set[i];
    applicable_rule_set.clear();
}

translation_option_set::size_type translation_option_set::size() const
{
    return applicable_rule_set.size();
}

void translation_option_set::add_complete_rule(partial_rule* rule)
{
    auto& rule_vec = *rule->get_rule();
    auto r = rule_vec[0];
    unsigned int terminal_num = r->get_terminal_number();
    unsigned int symbol_num = r->get_target_rule_body().size();
    unsigned int nonterminal_num = symbol_num - terminal_num;
    translation_option* option = new translation_option(nonterminal_num);
    const partial_rule* p = rule;

    while (nonterminal_num && p) {
        const symbol* sym = p->get_node()->get_symbol();

        if (sym->is_terminal()) {
            p = p->get_parent();
            continue;
        }

        nonterminal_num--;
        auto hypo_list = p->get_hypothesis_list();
        unsigned int index = r->get_nonterminal_map(nonterminal_num);

        option->add_hypothesis_list(index, hypo_list);
        p = p->get_parent();
    }

    option->set_rule_vector(&rule_vec);
    applicable_rule_set.push_back(option);
}