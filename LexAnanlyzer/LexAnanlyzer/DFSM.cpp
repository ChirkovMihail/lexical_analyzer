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
	for (i = 0; i < transit_func[curr_state].size(); ++i) 
		if (weights[i].find(c) != weights[i].end())
			return transit_func[curr_state][i];
	throw exception("Wrong symbol");
	return -1;
}

bool DFSM::is_accept(string& s, string& path)
{
	int curr_state = 0;
	path = "0 ";

	for (auto c : s) {
		curr_state = get_state(curr_state, c);
		if (curr_state == -1)
			return false;
		path += to_string(curr_state) + " ";
	}
	if (final_states[curr_state])
		return true;
	else
		return false;
}