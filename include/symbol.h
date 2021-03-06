/*
 * symbol.h
 *
 *
 * author: Playinf
 * email: playinf@stu.xmu.edu.cn
 *
 */
#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <string>
#include <unordered_set>
#include <functional.h>
#include <shared_mutex.h>

enum symbol_type {
    TERMINAL, NONTERMINAL
};

class symbol {
public:
    typedef unsigned int size_type;

    symbol();
    symbol(const std::string& name, bool terminal = true);
    symbol(const std::string& name, symbol_type type);
    ~symbol();

    symbol& operator=(const symbol&) = delete;
    bool operator==(const symbol&) const;

    const std::string* get_name() const;
    symbol_type get_type() const;
    bool is_terminal() const;
    bool is_nonterminal() const;
private:
    const std::string* name;
    symbol_type type;
};

class symbol_table {
public:
    ~symbol_table();

    unsigned int size() const;
    const symbol* search_symbol(const std::string& s);
    const symbol* search_symbol(const std::string& s, bool terminal);
    static symbol_table* get_instance();
private:
    symbol_table();

    shared_mutex mutex;
    std::unordered_set<std::string> string_set;
    std::unordered_set<symbol, symbol_hash, symbol_equal> symbol_set;
    static symbol_table instance;
};

#endif /* __SYMBOL_H__ */
