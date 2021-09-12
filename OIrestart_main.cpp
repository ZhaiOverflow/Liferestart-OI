#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


int iscsym_A(int c) {
  return (isalnum(c) || ( c == '_' ));

}

string trim(string s) {
    int x = 0, y = s.length() - 1;
    while(isspace(s[x])) x++;
    while(isspace(s[y])) y--;
    return s.substr(x, y - x + 1);
}

vector<string> remove_all(vector<string> vs, string remv) {
    vector<string> result;
    for(int i = 0; i < vs.size(); i++) {
        if(remv != vs[i]) result.push_back(vs[i]);
    }
    return result;
}

vector<string> split(string text, char c, bool space = true) {
    string cur = "";
    bool includeSpace = false;
    vector<string> tmp;
    vector<string> ret;
    int top[4] = {
        0, 0, 0, 0
    };
    for(int i = 0; i < text.length(); i++) {
        string cur2 = cur + text[i];
        if(text[i] == '(') {
            top[0]++;
        } else if(text[i] == ')') {
            top[0]--;
        } else if(text[i] == '[') {
            top[1]++;
        } else if(text[i] == ']') {
            top[1]--;
        } else if(text[i] == '{') {
            top[2]++;
        } else if(text[i] == '}') {
            top[2]--;
        } else if(text[i] == '"' && (i == 0 || text[i - 1] != '\\')) {
            top[3] = !top[3];
        }
        if((((c == text[i]) || (isspace(text[i])))) && !top[0] && !top[1] && !top[2] && !top[3]) {
            tmp.push_back(cur);
            cur = "";
            if(!isspace(text[i]) || includeSpace) {
                cur += text[i];
                tmp.push_back(cur);
                cur = "";
            }
        } else {
            if(top[3]) cur += text[i];
            else if(!isspace(text[i])) cur += text[i];
        }
    }
    tmp.push_back(cur);
    for(int i = 0; i < tmp.size(); i++) {
        if(tmp[i].length() && !isspace(tmp[i][0])) {
            ret.push_back(tmp[i]);
        }
    }
    return remove_all(ret, (string)("") + c);
}

vector<string> tokenize(string text) {
	vector<string> tmp;
	string cur = "";
	for(int i = 0; i < text.length(); i++) {
		if(text[i] == '(') {
			int top = 1;
			bool inq = false;
			if(cur != "") {
				tmp.push_back(cur);
			}
			cur = "(";
			while(top && i < text.length()) {
				i++;
				if((!isspace(text[i]) && text[i] != '\\') || inq) cur = cur + text[i];
				if(text[i] == '(') top++;
				else if(text[i] == ')') top--;
				if(text[i] == '\\') {
					if(i < text.length() - 1) {
						i++;
						cur = cur + text[i];
					}
				}
				else if(text[i] == '"') {
					inq = !inq;
				}
			}
			tmp.push_back(cur);
			cur = "";
		}
		else if(text[i] == '[') {
			int top = 1;
			bool inq = false;
			if(cur != "") {
				tmp.push_back(cur);
			}
			cur = "[";
			while(top && i < text.length()) {
				i++;
				if((!isspace(text[i]) && text[i] != '\\') || inq) cur = cur + text[i];
				if(text[i] == '[') top++;
				else if(text[i] == ']') top--;
				if(text[i] == '\\') {
					if(i < text.length() - 1) {
						i++;
						cur = cur + text[i];
					}
				}
				else if(text[i] == '"') {
					inq = !inq;
				}
			}
			tmp.push_back(cur);
			cur = "";
		}
		else if(text[i] == '{') {
			int top = 1;
			bool inq = false;
			if(cur != "") {
				tmp.push_back(cur);
			}
			cur = "{";
			while(top && i < text.length()) {
				i++;
				if((!isspace(text[i]) && text[i] != '\\') || inq) cur = cur + text[i];
				if(text[i] == '{') top++;
				else if(text[i] == '}') top--;
				if(text[i] == '\\') {
					if(i < text.length() - 1) {
						i++;
						cur = cur + text[i];
					}
				}
				else if(text[i] == '"') {
					inq = !inq;
				}
			}
			tmp.push_back(cur);
			cur = "";
		}
		else if(text[i] == '"') {
			int top = 1;
			if(cur != "") {
				tmp.push_back(cur);
			}
			cur = "\"";
			while(i < text.length()) {
				i++;
				if(text[i] == '\\') {
					cur = cur + text[i];
					if(i < text.length() - 1) {
						i++;
						cur = cur + text[i];
					}
				}
				else if(text[i] == '"') {
					cur = cur + text[i];
					break;
				}
				else cur = cur + text[i];
			}
			tmp.push_back(cur);
			cur = "";
		}
		else if(!iscsym_A(text[i]) && text[i] != '~' && text[i] != '.') {
			if(cur != "") {
				tmp.push_back(cur);
			} 
			if(!isspace(text[i])) {
				cur = "";
				cur += text[i];
				tmp.push_back(cur);
			}
			cur = "";
		}
		else if(iscsym_A(text[i]) && text[i] != '~' || text[i] == '.') {
			cur += text[i];
		}
	}
	if(cur != "") {
		tmp.push_back(cur);
	}
	vector<string> ret;
	vector<string> final;
	for(int i = 0; i < tmp.size(); i++) {
		if(tmp[i] != "") {
			ret.push_back(trim(tmp[i]));
		}
	}
    for(int i = 0; i < ret.size(); i++) {
        if(i > 0) {
            if(ret[i] == "=") {
                if(ret[i - 1] == ">" || ret[i - 1] == "<" || ret[i - 1] == "!" || ret[i - 1] == "=") final[final.size() - 1] += "=";
                else final.push_back(ret[i]);
            } else if(ret[i] == "&") {
                if(ret[i - 1] == "&") final[final.size() - 1] += "&";
                else final.push_back(ret[i]);
            } else if(ret[i] == "|") {
                if(ret[i - 1] == "|") final[final.size() - 1] += "|";
                else final.push_back(ret[i]);
            } else if(ret[i] == ">") {
                if(ret[i - 1] == ">") final[final.size() - 1] += ">";
                else final.push_back(ret[i]);
            } else if(ret[i] == "<") {
                if(ret[i - 1] == "<") final[final.size() - 1] += "<";
                else final.push_back(ret[i]);
            } else if(isdigit(ret[i][0])) {
            	if(ret[i - 1] == "-" || ret[i - 1] == "+") final[final.size() - 1] += ret[i];
                else final.push_back(ret[i]);
			}
			else final.push_back(ret[i]);
        } else final.push_back(ret[i]);
    }
	return final;
}

const int PROP_CNT = 8;

const int RP = 0; // RP
const int DP = 1; // Dynamic Programming
const int DS = 2; // Data Structure
const int GT = 3; // Graph Theory
const int SM = 4; // Simulation
const int FS = 5; // Family Support
const int SC = 6; // Self Confidence
const int AG = 7; // Age

string val_name[PROP_CNT] = {
	"RP", "DP", "DS", "GT", "SM", "FS", "SC", "AG"
};

int val_id(string name) {
	for(int i = 0; i < name.length(); i++) name[i] = toupper(name[i]);
	for(int i = 0; i < PROP_CNT; i++) if(val_name[i] == name) return i;
	return -1;
}

const int NONE = -1;

const char HAS_OCCURED = '1';
const char NOT_OCCURED = '0';
const char BIGGER = '>';
const char SMALLER = '<';
const char BIGGER_E = '}';
const char SMALLER_E = '{';
const char EQUAL = '=';
const char NOT_EQUAL = '!';

char flip_op(char op) {
	if(op == '1') return '0';
	if(op == '0') return '1';
	if(op == '}') return '<';
	if(op == '{') return '>';
	if(op == '>') return '{';
	if(op == '<') return '}';
	if(op == '=') return '!';
	if(op == '!') return '=';
	return '?';
}

char get_op(string name) {
	if(name == "#") return HAS_OCCURED;
	if(name == "@") return NOT_OCCURED;
	if(name == ">") return BIGGER;
	if(name == "<") return SMALLER;
	if(name == ">=") return BIGGER_E;
	if(name == "<=") return SMALLER_E;
	if(name == "==") return EQUAL;
	if(name == "!=") return NOT_EQUAL;
	return '?';
}

bool has_occured[50000];

struct Player {
	int ability[PROP_CNT];
	string name;
};

struct Condition {
	char op;
	int param;
	int which;
	bool met(Player player) {
//		cout << "Condition : " << val_name[which] << " " << op << " " << param << endl;
//		cout << "Player: " << player.ability[which] << endl;
		int val = player.ability[which];
		switch(op) {
			case HAS_OCCURED: {
				return has_occured[param];
				break;
			}
			case NOT_OCCURED: {
				return !has_occured[param];
				break;
			}
			case BIGGER: {
				return val > param;
				break;
			}
			case SMALLER: {
				return val < param;
				break;
			}
			case BIGGER_E: {
				return val >= param;
				break;
			}
			case SMALLER_E: {
				return val <= param;
				break;
			}
			case EQUAL: {
				return val == param;
				break;
			}
			case NOT_EQUAL: {
				return val != param;
				break;
			}
			default: {
				cout << "Warning: unknown conditon. op = " << op << ", param = " << param << "." << endl;
				return true;
				break;
			}
		}
		return true;
	}
	
	string to_string() {
		stringstream ss("");
		ss << val_name[which] + " " + op + " " << param;
		return ss.str();
	}
};

Condition make_condition(char op, int param, int which) {
	
	Condition condition;
	condition.op = op;
	condition.param = param;
	condition.which = which;
	
	return condition;
}

struct Rule {
	vector<bool> need;
	vector<vector<Condition> > conditions;
	
	void flip() {
		for(int i = 0; i < conditions.size(); i++) {
			for(int j = 0; j < conditions[i].size(); j++) {
				conditions[i][j].op = flip_op(conditions[i][j].op);
			}
		}
	}
	
	bool check_one(int loc, Player player) {
		for(int i = 0; i < conditions[loc].size(); i++) {
			if(!conditions[loc][i].met(player)) return false;
		}
		return true;
	}
	
	bool met(Player player) {
		if(conditions.size() == 0) return true;
		for(int i = 0; i < conditions.size(); i++) {
			if(need[i]) {
				if(check_one(i, player)) return true;
			}
			else {
				if(!check_one(i, player)) return true;
			}
		}
		return false;
	}
	
	void make_and(vector<Condition> cond) {
		conditions.push_back(cond);
		need.push_back(true);
	}
	
	void make_or(vector<Condition> cond) {
		for(int i = 0; i < cond.size(); i++) {
			vector<Condition> tmp;
			tmp.push_back(cond[i]);
			conditions.push_back(tmp);
			need.push_back(true);
		}
	}
	
	void make_not_and(vector<Condition> cond) {
		conditions.push_back(cond);
		need.push_back(false);
	}
	
	void make_not_or(vector<Condition> cond) {
		for(int i = 0; i < cond.size(); i++) {
			vector<Condition> tmp;
			tmp.push_back(cond[i]);
			conditions.push_back(tmp);
			need.push_back(false);
		}
	}
	
	string to_string() {
		if(conditions.size() == 0) return "Empty";
		string ret = "";
		if(!need[0]) ret += "not ";
		ret += conditions[0][0].to_string() + " ";
		for(int i = 1; i < conditions[0].size(); i++) {
			ret += "and " + conditions[0][i].to_string() + " ";
		}
		for(int i = 1; i < conditions.size(); i++) {
			ret += "\nor ";
			if(!need[i]) ret += "not ";
			ret += conditions[i][0].to_string() + " ";
			for(int j = 1; j < conditions[i].size(); j++) {
				ret += "and " + conditions[i][j].to_string() + " ";
			}
		}
		return ret;
	}
};

Rule make_rule() {
	Rule rule;
	return rule;
}

void occur_event(int id);
bool can_occur_event(int id, Player player);
vector<int> event_queue;
const int PRIOR_ADD = 3;

struct Event {
	int id;
	string msg;
	int change[PROP_CNT];
	int next_event;
	vector<int> prior;
	Rule rule;
	
	bool can_occur(Player player) {
		return rule.met(player);
	}
	
	void occur() {
		has_occured[id] = true;
		for(int i = 0; i < prior.size(); i++) {
			for(int j = 1; j <= PRIOR_ADD; j++) event_queue.push_back(prior[i]);
		}
		if(next_event != NONE) occur_event(next_event);
	}
	
	string to_string() {
		stringstream ss("");
		ss << "Event #" << id << endl;
		ss << "    MSG = " << msg << endl;
		for(int i = 0; i < PROP_CNT; i++) {
			ss << "    CHANGE-" << val_name[i] << " = " << change[i] << endl;
		}
		ss << "    NEXT-EVENT = " << next_event << endl;
		ss << "    PRIOR = [";
		if(prior.size() == 0) ss << "]" << endl;
		else {
			ss << prior[0];
			for(int i = 1; i < prior.size(); i++) ss << ", " << prior[i];
			ss << "]" << endl;
		}
		ss << "    RULE = " << rule.to_string() << endl;
		return ss.str();
	}
};

Event events[50000];
vector<int> EMPTY;

void occur_event(int id) {
	cout << "Event #" << id << " " << events[id].msg << " occured." << endl;
	events[id].occur();
}

bool can_occur_event(int id, Player player) {
	return events[id].can_occur(player);
}

bool has_event(int id) {
	for(int i = 0; i < event_queue.size(); i++) {
		if(event_queue[i] == id) return true;
	}
	return false;
}

void regist_event(	int id, string msg, 
					int rp, int dp, int ds, int gt, int sm, int fs, int sc, int ag,
					Rule rule,
					int next = NONE,
					vector<int> prior = EMPTY) {
	
	Event event;
	event.id = id;
	event.msg = msg;
	event.change[RP] = rp, event.change[DP] = dp, event.change[DS] = ds;
	event.change[GT] = gt, event.change[SM] = sm, event.change[FS] = fs;
	event.change[SC] = sc, event.change[AG] = ag;
	event.rule = rule;
	event.next_event = next, event.prior = prior;
	events[id] = event;
	if(!has_event(id)) event_queue.push_back(id);
	else {
		cout << "Warning: duplicated event id: " << id << "!" << endl;
	}
}

Player make_player(string name,
				int rp = 0, int dp = 0, int ds = 0,
				int gt = 0, int sm = 0, int fs = 0, int sc = 0, int ag = 0) {
	Player player;
	player.ability[RP] = rp, player.ability[DP] = dp, player.ability[DS] = ds;
	player.ability[GT] = gt, player.ability[SM] = sm, player.ability[FS] = fs;
	player.ability[SC] = sc, player.ability[AG] = ag, player.name = name;
	return player;
}

Rule spawn_condition(string str) {
	vector<string> tokens = tokenize(str);
	Rule ret;
	if(tokens.size() == 3) {
		string oprand1 = tokens[0];
		string oper = tokens[1];
		string oprand2 = tokens[2];
		
		if(oper == "||") {
			Rule left = spawn_condition(oprand1);
			Rule right = spawn_condition(oprand2);
			for(int i = 0; i < left.conditions.size(); i++) {
				ret.conditions.push_back(left.conditions[i]);
				ret.need.push_back(true);
			}
			for(int i = 0; i < right.conditions.size(); i++) {
				ret.conditions.push_back(right.conditions[i]);
				ret.need.push_back(true);
			}
			return ret;
		}
		else if(oper == "&&") {
			Rule left = spawn_condition(oprand1);
			Rule right = spawn_condition(oprand2);
			for(int i = 0; i < left.conditions.size(); i++) {
				ret.conditions.push_back(left.conditions[i]);
				ret.need.push_back(true);
			}
			for(int i = 0; i < right.conditions.size(); i++) {
				if(i >= ret.conditions.size()) {
					ret.conditions.push_back(vector<Condition>());
					ret.need.push_back(true);
				}
				for(int j = 0; j < right.conditions[i].size(); j++) ret.conditions[i].push_back(right.conditions[i][j]);
			}
			return ret;
		}
		else {
			int which = val_id(oprand1);
			int val = atoi(oprand2.c_str());
			char op = get_op(oper);
			if(which == -1 || op == '?') {
				cout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
				return make_rule(); 
			}
			Condition condition = make_condition(op, val, which);
			vector<Condition> c;
			c.push_back(condition);
			ret.make_and(c);
			return ret;
		}
	}
	else if(tokens.size() == 2) {
		string oper = tokens[0];
		string oprand = tokens[1];
		if(oper == "!") {
			Rule left = spawn_condition(oprand);
			for(int i = 0; i < left.conditions.size(); i++) {
				ret.conditions.push_back(left.conditions[i]);
			}
			ret.flip();
			return ret;
		}
		else {
			int val = atoi(oprand.c_str());
			char op = get_op(oper);
			if(op == '?') {
				cout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
				return make_rule(); 
			}
			Condition condition = make_condition(op, val, 0);
			vector<Condition> c;
			c.push_back(condition);
			ret.make_and(c);
			return ret;
		}
	}
	else {
		if(str.length() && str[0] == '(') {
			str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
			return spawn_condition(str);
		}
		else {
			cout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
			return ret; 
		}
	}
	return ret;
}

vector<int> make_change_arr(string str) {
	str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
	vector<string> args = split(str, ',');
	vector<int> vi;
	vi.resize(PROP_CNT);
	for(int i = 0; i < args.size(); i++) {
		vector<string> tokens = tokenize(args[i]);
		if(tokens.size() != 2) {
			cout << "Invalid change string: " << str << "! Will return an empty list." << endl;
			return EMPTY;
		}
		string name = tokens[0];
		string val = tokens[1];
		int id = val_id(name);
		if(id == -1) {
			cout << "Invalid change string: " << str << "! Will return an empty list." << endl;
			return EMPTY;
		}
		vi[id] = atoi(val.c_str());
	}
	return vi;
}

vector<int> make_arr(string str) {
	str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
	vector<string> args = split(str, ',');
	vector<int> vi;
	for(int i = 0; i < args.size(); i++) vi.push_back(atoi(args[i].c_str()));
	return vi;
}

void regist_event(string str) {
	vector<string> tokens = tokenize(str);
	if(tokens.size() < 5) {
		regist_event(0, "Default Event", 0, 0, 0, 0, 0, 0, 0, 0, spawn_condition(""));
		cout << "Invalid event string: " << str << "! Will regist a default event with id 0." << endl;
	}
	else {
		string xstr = "";
		vector<int> params = make_change_arr(tokens[2]);
		for(int i = 5; i < tokens.size(); i++) xstr += tokens[i] + " ";
		
		regist_event(
		atoi(tokens[0].c_str()), xstr, params[RP], params[DP], params[DS],
		params[GT], params[SM], params[FS], params[SC], params[AG], spawn_condition(tokens[1].c_str()), 
		atoi(tokens[3].c_str()), make_arr(tokens[4])
		);
		cout << "Registed Event: " << events[atoi(tokens[0].c_str())].to_string() << endl;
	}
}

void next_event(Player & player) {
	int id;
	do {
		id = event_queue[rand() % event_queue.size()];
	} while(!can_occur_event(id, player));
	occur_event(id);
	for(int i = 0; i < PROP_CNT; i++) {
		player.ability[i] += events[id].change[i];
	}
}

void color(int co){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),co);
}

int main() {
	string cmd;
	
	color(14);
	
	cout << "Initializing player...";
	
	Player player = make_player("Test");
	cout << "Done." << endl;
	cout << "Opening events file...";
	ifstream fcin("events.txt");
	if(!fcin) {
		cout << "Can not found events.txt." << endl;
		cout << "Program will exit." << endl;
		exit(1);
	}
	
	cout << "Done." << endl;
	cout << "Reading events configurations..." << endl; 
	
	while(getline(fcin, cmd)) regist_event(cmd);
	
	cout << "All events registered. Events count: " << event_queue.size() << "." << endl;
	
	cout << "Press any key to start" << endl;
	
	system("pause");
	
	while(true) {
		next_event(player);
		system("pause");
	}
	cout << "Game Over." << endl;
	
	return 0;
}
