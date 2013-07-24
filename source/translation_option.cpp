/* translation_option.cpp */
#include <vector>
#include <rule.h>
#include <symbol.h>
#include <hypothesis.h>
#include <translation_option.h>

translation_option::translation_option(size_type size)
{
    if (size != 0) {
        hypothesis_vector = new std::vector<hypothesis_list*>();
        hypothesis_vector->resize(size);
    } else
        hypothesis_vector = nullptr;
    rule_vector = nullptr;
}

translation_option::~translation_option()
{
    if (hypothesis_vector != nullptr)
        delete hypothesis_vector;
}

const symbol* translation_option::get_start_symbol() const
{
    /* target nonterminal */
    const unsigned int index = 1;
    const rule* r = rule_vector->at(0);

    return r->get_start_symbol(index);
}

unsigned int translation_option::get_rule_nonterminal_number() const
{
    auto r = rule_vector->at(0);
    unsigned int terminal_num = r->get_terminal_number();
    unsigned int symbol_num = r->get_target_rule_body().size();
    unsigned int nonterminal_num = symbol_num - terminal_num;

    return nonterminal_num;
}

std::vector<const rule*>* translation_option::get_rule_vector() const
{
    return rule_vector;
}

std::vector<hypothesis*>* translation_option::get_hypothesis_vector(size_type i)
{
    return hypothesis_vector->at(i);
}

translation_option::size_type translation_option::size() const
{
    return rule_vector->size();
}

void translation_option::set_rule_vector(const vector_type* vec)
{
    rule_vector = const_cast<vector_type*>(vec);
}

void translation_option::add_hypothesis_list(unsigned int index,
    hypothesis_list* list)
{
    hypothesis_list** data = hypothesis_vector->data();

    data[index] = list;
}