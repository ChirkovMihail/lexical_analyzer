#pragma once
#include "Token.h"

class HashTable
{
private:	
	vector<Token> table;
	long long M = 911382323, N = 972663749;
	
	int hash_func(Token tok);

public:
	HashTable(int table_size);
	void insert_token(Token tok);
	int get_index(Token tok);
	Token get_tok(int index);
	void print_table(ostream& out);
};