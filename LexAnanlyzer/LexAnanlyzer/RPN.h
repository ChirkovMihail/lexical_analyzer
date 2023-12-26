#pragma once
string CYK::CreateRPN()
{
	return CreateRPN(tree);
}

string CYK::CreateRPN(ParseTree* vert)
{
	string s = vert->value;
	if (s == "Function" || s == "Function1" || s == "Function2"
		|| (s == "Operators" && vert->children[0]->value == "OpWithSpace" && 
			vert->children[1]->value == "Operators"))
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]);

	if (s == "Fun1" || s == "Fun2" || s == "Fun3" || s == "OpWithSpace" || 
		(s == "SimpleExpr" && vert->children.size() > 1 && vert->children[0]->value == "SimpleExpr1"
			&& vert->children[1]->value == "CloseRoundBracket"))
		return CreateRPN(vert->children[0]);

	if (s == "Begin") {
		string res = "";

		res += atrib[vert->num] + " ";
		vector<pair<int, char> > name;
		fill_name(vert->children[1]->children[0], name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " 2 DECL ";
		return res;
	}

	if (s == "End") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert->children[0]->children[1]->children[0], name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " RETURN ";
		return res;
	}

	if (s == "Descriptions" && vert->children.size() > 1 &&
		vert->children[0]->value == "DescrWithSpace" && vert->children[1]->value == "Descriptions") {
		return CreateRPN(vert->children[0]->children[0]) + CreateRPN(vert->children[1]);
	}

	if ((s == "Descr" || s == "Descriptions") && vert->children.size() > 1
		&& vert->children[0]->value == "Descr1" && vert->children[1]->value == "Descr2") {
		string res = "", w;
		int cnt = 1;
		res += atrib[vert->children[0]->children[0]->num] + " ";

		vector<pair<int, char> > name;
		fill_name(vert->children[1]->children[0]->children[0], name);

		for (auto p : name)
			if (!(p.second >= 'a' && p.second <= 'z' || p.second >= 'A' && p.second <= 'Z')) {
				if (w.size() > 0) {
					res += w + " ";
					cnt++;
					w = "";
				}
			}
			else
				w += p.second;
		if (w.size() > 0) {
			res += w + " ";
			cnt++;
		}
		res += to_string(cnt) + " ";
		res += "DECL ";
		return res;
	}

	if ((s == "Op" || s == "Operators") && vert->children[0]->value == "Op1"
		&& vert->children[1]->value == "Op2") {
		string res;

		vector<pair<int, char> > name;
		fill_name(vert->children[0]->children[0]->children[0], name);
		for (auto p : name)
			res += p.second;
		res += " ";

		res += CreateRPN(vert->children[1]->children[0]->children[0]);
		res += "= ";
		return res;
	}

	if (s == "SimpleExpr1" && vert->children.size() > 1 &&
		vert->children[0]->value == "OpenRoundBracket" && vert->children[1]->value == "Expr") {
		return CreateRPN(vert->children[1]);
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Expr1" && vert->children[1]->value == "ExprPlus") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "+ ";
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Expr1" && vert->children[1]->value == "ExprMinus") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "- ";
	}

	if (s == "Expr1" && vert->children.size() > 1 &&
		vert->children[0]->value == "Term" && vert->children[1]->value == "Space") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "ExprPlus" && vert->children.size() > 1 &&
		vert->children[0]->value == "PlusWithSpace" && vert->children[1]->value == "Expr") {
		return CreateRPN(vert->children[1]);
	}

	if (s == "ExprMinus" && vert->children.size() > 1 &&
		vert->children[0]->value == "MinusWithSpace" && vert->children[1]->value == "Expr") {
		return CreateRPN(vert->children[1]);
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprMult" && vert->children[1]->value == "SpaceWithTerm") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "* ";
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprDiv" && vert->children[1]->value == "SpaceWithTerm") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "/ ";
	}

	if (s == "SpaceWithTerm" && vert->children.size() > 1 &&
		vert->children[0]->value == "Space" && vert->children[1]->value == "Term") {
		return CreateRPN(vert->children[1]);
	}

	if (s == "SimpleExprMult" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExpr" && vert->children[1]->value == "SpaceWithMult") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "SimpleExprDiv" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExpr" && vert->children[1]->value == "SpaceWithDiv") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExpr1" && vert->children[1]->value == "CloseRoundBracket") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "SimpleExpr" && vert->children.size() == 1) {
		return vert->children[0]->value + " ";
	}

	if (s == "SimpleExpr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Letter" && vert->children[1]->value == "LetNum") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "SimpleExpr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Digit" && vert->children[1]->value == "IntNumber") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "SimpleExpr" && vert->children.size() > 1 &&
		vert->children[0]->value == "IntNumber" && vert->children[1]->value == "RealPart") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Term" && vert->children.size() > 0 &&
		vert->children[0]->value == "SimpleExpr1" && vert->children[1]->value == "CloseRoundBracket") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "Expr" && vert->children.size() > 0 &&
		vert->children[0]->value == "SimpleExpr1" && vert->children[1]->value == "CloseRoundBracket") {
		return CreateRPN(vert->children[0]);
	}

	if (s == "Term" && vert->children.size() == 1) {
		return vert->children[0]->value + " ";
	}

	if (s == "Expr" && vert->children.size() == 1) {
		return vert->children[0]->value + " ";
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprMult" && vert->children[1]->value == "SpaceWithTerm") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "* ";
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "SimpleExprDiv" && vert->children[1]->value == "SpaceWithTerm") {
		return CreateRPN(vert->children[0]) + CreateRPN(vert->children[1]) + "/ ";
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Letter" && vert->children[1]->value == "LetNum") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "Digit" && vert->children[1]->value == "IntNumber") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Expr" && vert->children.size() > 1 &&
		vert->children[0]->value == "IntNumber" && vert->children[1]->value == "RealPart") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "Letter" && vert->children[1]->value == "LetNum") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "Digit" && vert->children[1]->value == "IntNumber") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}

	if (s == "Term" && vert->children.size() > 1 &&
		vert->children[0]->value == "IntNumber" && vert->children[1]->value == "RealPart") {
		string res = "";
		vector<pair<int, char> > name;
		fill_name(vert, name);
		sort(all(name));
		for (auto p : name)
			res += p.second;
		res += " ";
		return res;
	}
	return "";
}