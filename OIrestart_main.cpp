#include <bits/stdc++.h>

#ifdef WIN32

#include <windows.h>
void color(int co){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), co);
}

#define __Pause system("pause > nul")

#elif defined(linux)

void color(int co){
	std::string __command = "echo -e \"\\e[" + std::to_string(co + 17) + "m\"";
	system(__command.c_str());
}

#define __Pause cin.get()

#else 

#error Your OS does not support OIrestart.

#endif

using namespace std;

stringstream stream("");

#ifdef JSDLL

#define cout stream

#endif

ofstream logcout("oirestart-log.txt");

const int STAGE_CAP = 100;
const int MAX_EVENT_CNT = 50000; 

int iscsym_A(int c) {
  return (isalnum(c) || ( c == '_' ));

}

string to_str(int x) {
	stringstream ss("");
	ss << x;
	return ss.str();
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
            } else if(isdigit(ret[i][0]) || ret[i][0] == '*' || ret[i][0] == '+') {
            	if(ret[i - 1] == "-" || ret[i - 1] == "+" || ret[i - 1] == "*") final[final.size() - 1] += ret[i];
                else final.push_back(ret[i]);
			}
			else final.push_back(ret[i]);
        } else final.push_back(ret[i]);
    }
	return final;
}

const int PROP_CNT = 9;

const int RP = 0; // RP
const int DP = 1; // Dynamic Programming
const int DS = 2; // Data Structure
const int GT = 3; // Graph Theory
const int SM = 4; // Simulation
const int FS = 5; // Family Support
const int SC = 6; // Self Confidence
const int AG = 7; // Age
const int LV = 8; // Level

string val_name[PROP_CNT] = {
	"RP", "DP", "DS", "GT", "SM", "FS", "SC", "AG", "LV"
};

string full_name[PROP_CNT] = {
	"Lucky", "Dynamic Programming", "Data Strucure", "Graph Theory", "Simulation",
	"Family Support", "Self Confidence", "Age", "Current Score"
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
	
	string to_string() {
		stringstream ss("");
		ss << "Player " << name << ": " << endl;
		for(int i = 0; i < PROP_CNT; i++) ss << "    " << full_name[i] << ": " << ability[i] << endl;
		return ss.str();
	}
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
				logcout << "Warning: unknown conditon. op = " << op << ", param = " << param << "." << endl;
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

vector<vector<int> > stages;

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
vector<int> public_area;
vector<int> event_queue;
const int PRIOR_ADD = 1;

struct Event {
	int id;
	string msg;
	int change[PROP_CNT];
	vector<int> next_event;
	vector<int> prior;
	Rule rule;
	
	bool can_occur(Player player) {
		return rule.met(player);
	}
	
	void occur() {
		has_occured[id] = true;
		for(int i = 0; i < prior.size(); i++) {
			for(int j = 1; j <= PRIOR_ADD; j++) {
				int id = prior[i];
				int stageid = id / STAGE_CAP;
				stages[stageid].push_back(prior[i]);
			}
		}
		for(int i = 0; i < next_event.size(); i++) occur_event(next_event[i]);
	}
	
	string to_string() {
		stringstream ss("");
		ss << "Event #" << id << endl;
		ss << "    MSG = " << msg << endl;
		for(int i = 0; i < PROP_CNT; i++) {
			ss << "    CHANGE-" << val_name[i] << " = " << change[i] << endl;
		}
		ss << "    NEXT-EVENT = [";
		if(next_event.size() == 0) ss << "]" << endl;
		else {
			ss << next_event[0];
			for(int i = 1; i < next_event.size(); i++) ss << ", " << next_event[i];
			ss << "]" << endl;
		}
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

Event events[MAX_EVENT_CNT];
vector<int> EMPTY;

const int SPAWN_EVENT = 1001;
const int OVER_EVENT = 49999;

bool has_event(int id) {
	for(int i = 0; i < event_queue.size(); i++) {
		if(event_queue[i] == id) return true;
	}
	return false;
}

void enter_stage(int newstage) {
	event_queue = stages[newstage / STAGE_CAP];
	for(int i = 0; i < public_area.size(); i++) event_queue.push_back(public_area[i]);
}

void occur_event(int id) {
	if(id == -1 || !has_event(id)) {
		logcout << "Can not found event " << id << "! Nothing will happen!" << endl;
		return;
	}
	logcout << "Event #" << id << " " << events[id].msg << " occured." << endl;
	
	vector<string> tokens = tokenize(events[id].msg);
	if(tokens.size() == 3 && tokens[0] == "Jump" && tokens[1] == "to") {
		int newstage = atoi(tokens[2].c_str());
		enter_stage(newstage);
		cout << "Congratulations. Entered new stage.\n" << endl;
		logcout << "Enter " << newstage / STAGE_CAP << "." << endl;
	}
	else {
		cout << events[id].msg << endl;
		cout << endl;
	}
	
	events[id].occur();
	if(id == OVER_EVENT) {
		logcout << "Over event " << OVER_EVENT << " occured. Game over." << endl;
		cout << "Game over." << endl;
		exit(0);
	}
}

bool can_occur_event(int id, Player player) {
	return events[id].can_occur(player);
}

void regist_event(	int id, string msg, 
					int rp, int dp, int ds, int gt, int sm, int fs, int sc, int ag, int lv,
					Rule rule,
					vector<int> next = EMPTY,
					vector<int> prior = EMPTY) {
	
	Event event;
	event.id = id;
	event.msg = msg;
	event.change[RP] = rp, event.change[DP] = dp, event.change[DS] = ds;
	event.change[GT] = gt, event.change[SM] = sm, event.change[FS] = fs;
	event.change[SC] = sc, event.change[AG] = ag, event.change[LV] = lv;
	event.rule = rule;
	event.next_event = next, event.prior = prior;
	events[id] = event;
	if(!has_event(id)) event_queue.push_back(id);
	else {
		logcout << "Warning: duplicated event id: " << id << "!" << endl;
	}
	stages[id / STAGE_CAP].push_back(id);
}

Player make_player(string name,
				int rp = 0, int dp = 0, int ds = 0,
				int gt = 0, int sm = 0, int fs = 0, int sc = 0, int ag = 0) {
	Player player;
	player.ability[RP] = rp, player.ability[DP] = dp, player.ability[DS] = ds;
	player.ability[GT] = gt, player.ability[SM] = sm, player.ability[FS] = fs;
	player.ability[SC] = sc, player.ability[AG] = ag, player.ability[LV] = 0;
	player.name = name;
	return player;
}

Rule spawn_condition(string str) {
	vector<string> tokens = tokenize(str);
	Rule ret = make_rule();
	if(str == "") {
		logcout << "Empty rule! Will return a empty rule." << endl;
		return ret;
	}
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
			for(int k = 0; k < ret.conditions.size(); k++) {
				for(int i = 0; i < right.conditions.size(); i++) {
					if(i >= ret.conditions.size()) {
						ret.conditions.push_back(vector<Condition>());
						ret.need.push_back(true);
					}
					for(int j = 0; j < right.conditions[i].size(); j++) ret.conditions[k].push_back(right.conditions[i][j]);
				}
			}
			return ret;
		}
		else {
			int which = val_id(oprand1);
			int val = atoi(oprand2.c_str());
			char op = get_op(oper);
			if(which == -1 || op == '?') {
				logcout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
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
				logcout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
				return make_rule(); 
			}
			Condition condition = make_condition(op, val, 0);
			vector<Condition> c;
			c.push_back(condition);
			ret.make_and(c);
			return ret;
		}
	}
	else if(tokens.size() == 1) {
		if(str.length() && str[0] == '(') {
			str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
			return spawn_condition(str);
		}
		else {
			logcout << "Invalid condition string: " << str << "! Will return an empty rule." << endl;
			return ret; 
		}
	}
	else {
		string last_expr = tokens[tokens.size() - 1];
		string op = tokens[tokens.size() - 2];
		string front = "";
		for(int i = 0; i < tokens.size() - 2; i++) front += tokens[i];
		return spawn_condition("(" + front + ")" + op + last_expr);
	}
	return ret;
}

vector<int> make_change_arr(string str) {
	str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
	vector<int> vi;
	vi.resize(PROP_CNT);
	if(str == "") return vi;
	vector<string> args = split(str, ',');
	for(int i = 0; i < args.size(); i++) {
		vector<string> tokens = tokenize(args[i]);
		if(tokens.size() != 2) {
			logcout << "Invalid change string: " << str << "! Will return an empty list." << endl;
			return EMPTY;
		}
		string name = tokens[0];
		string val = tokens[1];
		int id = val_id(name);
		if(id == -1) {
			logcout << "Invalid change string: " << str << "! Will return an empty list." << endl;
			return EMPTY;
		}
		vi[id] = atoi(val.c_str());
	}
	return vi;
}

vector<int> make_arr(string str) {
	if(str[0] == '[' || str[0] == '(') str = str.length() >= 2 ? str.substr(1, str.length() - 2) : str;
	if(str == "") {
		return EMPTY;
	}
	vector<string> args = split(str, ',');
	
	vector<int> vi;
	for(int i = 0; i < args.size(); i++) vi.push_back(atoi(args[i].c_str()));
	return vi;
}

#ifdef JSDLL

void regist_eventx(string str) {
	
#else

void regist_event(string str) {
	
#endif
	vector<string> tokens = tokenize(str);
	if(tokens.size() < 6) {
		regist_event(0, "Default Event", 0, 0, 0, 0, 0, 0, 0, 0, 0, spawn_condition(""));
		logcout << "Invalid event string: " << str << "! Will regist a default event with id 0." << endl;
	}
	else {
		string xstr = "";
		vector<int> params = make_change_arr(tokens[2]);
		for(int i = 5; i < tokens.size(); i++) xstr += tokens[i] + " ";
		
		bool once = true;
		bool public_event = false;
		if(tokens[0][0] == '*' && tokens[0][1] == '+') {
			once = false, public_event = true;
			tokens[0] = tokens[0].substr(2, tokens[0].length() - 2);
		}
		else if(tokens[0][1] == '*' && tokens[0][0] == '+') {
			once = false, public_event = true;
			tokens[0] = tokens[0].substr(2, tokens[0].length() - 2);	
		}
		else {
			if(tokens[0][0] == '+') {
				once = false;
				tokens[0] = tokens[0].substr(1, tokens[0].length() - 1);
			}
			else if(tokens[0][0] == '*') {
				public_event = true;
				tokens[0] = tokens[0].substr(1, tokens[0].length() - 1);
			}
		}
		int id = atoi(tokens[0].c_str());
		if(public_event) public_area.push_back(id);
		if(once) {
			if(tokens[1] != "()") tokens[1] += "&&(@" + tokens[0] + ")";
			else tokens[1] = "(@" + tokens[0] + ")";
		}
		if(id != SPAWN_EVENT) tokens[1] += "&&(#" + to_str(SPAWN_EVENT) + (string)")";
		
		regist_event(
		id, xstr, params[RP], params[DP], params[DS],
		params[GT], params[SM], params[FS], params[SC], params[AG], params[LV], spawn_condition(tokens[1]), 
		make_arr(tokens[3]), make_arr(tokens[4])
		);
		logcout << "Registed Event: " << events[atoi(tokens[0].c_str())].to_string() << endl;
	}
}
string next_event(Player & player) {
	string ret = "";
	int id;
	do {
		id = event_queue[rand() % event_queue.size()];
	} while(!can_occur_event(id, player));
	cout << "Age " << player.ability[AG] << ": ";
	occur_event(id);
	for(int i = 0; i < PROP_CNT; i++) {
		player.ability[i] += events[id].change[i];
	}
//	cout << player.to_string() << endl;
	ret = stream.str();
	stream.str("");
	return ret;
}

void init() {
	stages.resize(MAX_EVENT_CNT / STAGE_CAP);
	string name;
	unsigned long long hash = 0;
	for(int i = 0; i < name.length(); i++) hash *= 131, hash += name[i];
}

#ifndef JSDLL

int main() {
	init();
	string cmd;
	srand(time(NULL));
	
//	color(14);
	
	cout << "Initializing..." << endl;
//	system("CHCP 65001");
	
	logcout << "Initializing player...";
	
	Player player = make_player("Test");
	logcout << "Done." << endl;
	logcout << "Opening events file...";
	ifstream fcin("events.txt");
	if(!fcin) {
		logcout << "Can not found events.txt." << endl;
		logcout << "Program will exit." << endl;
		exit(1);
	}
	
	logcout << "Done." << endl;
	logcout << "Reading events configurations..." << endl; 
	
	while(getline(fcin, cmd)) {
		cmd = trim(cmd);
		if(cmd == "") continue;
		regist_event(cmd);
	}
	
	logcout << "All events registered. Events count: " << event_queue.size() << "." << endl;
	
	for(int i = 0; i < MAX_EVENT_CNT / STAGE_CAP; i++) {
		if(stages[i].size()) {
			logcout << "Stage " << i << ": " << endl; 
			for(int j = 0; j < stages[i].size(); j++) logcout << "Event #" << stages[i][j] << endl;
			logcout << endl;
		}
	}
	
	cout << "Done." << endl;
	cout << "Press any key to start" << endl;
	
	__Pause;
	
	enter_stage(SPAWN_EVENT);
	logcout << "Current Stage " << SPAWN_EVENT / STAGE_CAP << ": " << endl;
	for(int i = 0; i < event_queue.size(); i++) logcout << "Event queue " << event_queue[i] << endl;
	
	while(true) {
		next_event(player);
		__Pause;
	}
	cout << "Game Over." << endl;
	
	return 0;
}

#endif
