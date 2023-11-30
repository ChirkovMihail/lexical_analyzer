#pragma once
struct ParseTree
{
	string value = "";
	vector<ParseTree*> children;
};