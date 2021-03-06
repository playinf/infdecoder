/* symbol.cpp */
#include <string>
#include <functional>
#include <unordered_set>
#include <symbol.h>

// static symbol table instance
symbol_table symbol_table::instance;

symbol::symbol()
{
    name = nullptr;
    type = TERMINAL;
}

symbol::symbol(const std::string& name, bool terminal)
{
    this->name = &name;

    if (terminal)
        this->type = TERMINAL;
    else
        this->type = NONTERMINAL;
}

symbol::symbol(const std::string& name, symbol_type type)
{
    this->name = &name;
    this->type = type;
}

symbol::~symbol()
{
    // do nothing
}

const std::string* symbol::get_name() const
{
    return name;
}

symbol_type symbol::get_type() const
{
    return type;
}

bool symbol::is_terminal() const
{
    return type == TERMINAL;
}

bool symbol::is_nonterminal() const
{
    return type != TERMINAL;
}

bool symbol::operator==(const symbol& sym) const
{
    if (type == sym.type) {
        return name == sym.name;
    }

    return false;
}

symbol_table::symbol_table()
{
    // do nothing
}

symbol_table::~symbol_table()
{
    // do nothing
}

unsigned int symbol_table::size() const
{
    return symbol_set.size();
}

const symbol* symbol_table::search_symbol(const std::string& s)
{
    return search_symbol(s, true);
}

const symbol* symbol_table::search_symbol(const std::string& s, bool terminal)
{
    symbol sym(s, terminal);
    mutex.lock_shared();
    auto iter = symbol_set.find(sym);
    mutex.unlock_shared();

    if (iter != symbol_set.end())
        return &(*iter);

    // not in the symbol set
    mutex.lock();
    auto r1 = string_set.insert(s);
    symbol tmp(*r1.first, terminal);
    auto r2 = symbol_set.insert(tmp);
    mutex.unlock();

    return &(*r2.first);
}

symbol_table* symbol_table::get_instance()
{
    return &instance;
}
