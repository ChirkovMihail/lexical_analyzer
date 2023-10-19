#include "Libraries.h"
#include "Hash_table.h"
#include "Token.h"

HashTable::HashTable(int table_size)
{
	this->table.resize(table_size, Token());
}

int HashTable::hash_func(Token tok)
{
	long long res = 0;	
	for (int i = 0; i < tok.s.size(); ++i)
		res = (res * M + tok.s[i]) % N;
	res %= table.size();
	return res;
}

void HashTable::insert_token(Token tok)
{
	int i;
	int hashh = hash_func(tok);

	for (i = hashh; i < table.size(); ++i) {
		if (table[i].empty) {
			table[i] = tok;
			return;
		}
	}

	table.push_back(tok);
}

int HashTable::get_index(Token tok)
{
	int i;
	int hashh = hash_func(tok);

	for (i = hashh; i < table.size(); ++i) 
		if (table[i].s == tok.s && table[i].type == tok.type) 		
			return i;

	return -1;
}

Token HashTable::get_tok(int index)
{
	if (index >= 0 && index < table.size())
		return table[index];
	else
		return Token();
}

void HashTable::print_table(ostream& out)
{
	for (int i = 0; i < table.size(); ++i) {
		out << i + 1 << ")------\n";
		table[i].print(out);
	}
}