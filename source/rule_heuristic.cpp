/* rule_heuristic.cpp */
#include <rule.h>
#include <model.h>
#include <config.h>
#include <symbol.h>
#include <rule_tree.h>
#include <language_model.h>
#include <translation_model.h>
#include <ngram_language_model.h>

float hpb_rule_heuristic_function(const rule* r)
{
    configuration* config = configuration::get_instance();
    model* system_model = config->get_model();
    auto& target_rule_body = r->get_target_rule_body();
    unsigned int size = target_rule_body.size();
    const float penalty = -1.0f;
    unsigned int tm_id = r->get_id();
    language_model* lm = system_model->get_language_model(0);
    ngram_language_model* nlm = static_cast<ngram_language_model*>(lm);
    translation_model* tm = system_model->get_translation_model(tm_id);
    float lm_weight = system_model->get_weight(lm->get_feature_id());
    unsigned int id = system_model->get_feature_id("word penalty");
    float terminal_num_weight = system_model->get_weight(id);
    unsigned target_terminal_number = r->get_terminal_number();
    float score = 0.0f;

    for (unsigned int i = 0; i < size; i++) {
        const symbol* sym = target_rule_body[i];

        if (sym->is_nonterminal())
            continue;

        score += nlm->probability(*sym->get_name());
    }

    size = tm->get_feature_number();
    score *= lm_weight;

    for (unsigned int i = 0; i < size; i++) {
        unsigned int id = tm->get_feature_id(i);
        float weight = system_model->get_weight(id);

        score += weight * r->get_score(i);
    }

    score += terminal_num_weight * penalty * target_terminal_number;

    return score;
}
