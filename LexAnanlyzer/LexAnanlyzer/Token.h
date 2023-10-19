#pragma once
class Token
{
public:
	string s;
	string type;
	bool empty;

	Token() { s = ""; type = ""; empty = true; };
	Token(string& value, string tp) { s = value; type = tp; empty = false; };
	void print(ostream& out) {
		out << "value : " << s << '\n';
		out << "type : " << type << '\n';
	}
};