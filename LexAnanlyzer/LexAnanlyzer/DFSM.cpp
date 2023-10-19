#include "Libraries.h"
#include"DFSM.h"

DFSM::DFSM(int num_states, vector<char>& alphabet, vector<set<char> >& weights, vector<bool>& final_states, vector<vector<int> >& transit_func)
{
	this->num_states = num_states;
	this->alphabet = alphabet;
	this->weights = weights;
	this->final_states = final_states;
	this->transit_func.resize(num_states);
	for (int i = 0; i < num_states; ++i)
		this->transit_func[i] = transit_func[i];
}

int DFSM::get_state(int curr_state, char c)
{
	int i = 0;	
	for (i = 0; i < transit_func[curr_state].size(); ++i) {
		if (transit_func[curr_state][i] == -1)
			continue;
		if (weights[i].find(c) != weights[i].end())
			return transit_func[curr_state][i];
	}
	
	return -1;
}

string DFSM::get_state_type(int state)//2 4 6 7 8 9 10 11 12 13 14 15 16 17 18
{
	state++;

	if (state == 2 || state == 3 || state == 4)
		return "integer";
	if (state == 6)
		return "real";
	if (state == 7)
		return "operator";
	if (state == 8)
		return "service char";
	if (state == 9 || state == 10 || state == 12 || state == 13 || state == 14 || state == 15 || state == 16 || state == 17)
		return "identificator";
	if (state == 11 || state == 18)
		return "key word";

	return "";
}

string DFSM::is_accept(string& s, string& path)
{
	int curr_state = 0;
	path = "0 ";

	for (auto c : s) {
		curr_state = get_state(curr_state, c);
		if (curr_state == -1)
			return "";
		path += to_string(curr_state) + " ";
	}
	if (final_states[curr_state])
		return get_state_type(curr_state);
	else
		return "";
}
string DFSM::is_accept(string& s)
{
	int curr_state = 0;

	for (auto c : s) {
		curr_state = get_state(curr_state, c);
		if (curr_state == -1)
			return "";
	}
	if (final_states[curr_state])
		return get_state_type(curr_state);
	else
		return "";
}