#include <bits/stdc++.h>

using namespace std;

stack<int> stck;
int program_counter =0;
vector<pair<string,string>> program;
unordered_map<string, int> flags;

void PUSH(int a) {
    stck.push(a);
}

pair<int,int> GET() {
    int a = stck.top();
    stck.pop();
    int b = stck.top();
    stck.pop();
    return make_pair(a,b);
}


void DUPE() {
    stck.push(stck.top());
}

void REVERSE() {
    stack<int> temp;
    while(!stck.empty()) {
        temp.push(stck.top());
        stck.pop();
    }
    stck = temp;
}

void ADD() {
    auto [a,b] = GET();
    stck.push(a + b);
}

void SUB() {
    auto [a,b] = GET();
    stck.push(a - b);
}

void MUL() {
    auto [a,b] = GET();
    stck.push(a * b);
}

void DIV() {
    auto [a,b] = GET();
    stck.push(a / b);
}

void READ() {
    int a;
    cin >> a;
    stck.push(a);
}

void WRITE() {
    cout<<stck.top()<<"\n";
    stck.pop();
}

void JEQ(string label) {
    int a = stck.top();
    stck.pop();
    if (a==0)
        program_counter = flags[label];
}

void JGT(const string &label) {
    int a = stck.top();
    stck.pop();
    if (a>0)
        program_counter = flags[label];
}

void JMP(const string &label) {
    program_counter = flags[label];
}

void CALL(const string &label) {
    stck.push(program_counter);
    program_counter = flags[label];
}

void RET() {
    program_counter = stck.top();
    stck.pop();
}

void SIZE() {
    stck.push(stck.size());
}
void EMPTY() {
    stck.push(stck.empty());
}
void parse_program(const string& file_path) {
    ifstream in(file_path);
    if (!in.is_open()) return;

    auto ltrim = [](string &s){ s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch){ return !isspace(ch); })); };
    auto rtrim = [](string &s){ s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), s.end()); };
    auto trim = [&](string &s){ ltrim(s); rtrim(s); };

    string line;
    while (std::getline(in, line)) {
        trim(line);
        if (line.empty()) continue;
        if (!line.empty() && line[0] == '#') continue;
        if (!line.empty() && line.back() == ':') {
            string label = line.substr(0, line.size() - 1);
            trim(label);
            flags[label] = program.size();
            continue;
        }

        // instruction [operand]
        string instr;
        string operand;
        size_t sp = line.find(' ');
        if (sp == string::npos) {
            instr = line;
            operand = "";
        } else {
            instr = line.substr(0, sp);
            operand = line.substr(sp + 1);
            trim(operand);
        }
        program.emplace_back(instr, operand);
    }
}

int main() {
    parse_program("program.stack");

    while (program[program_counter].first!="HALT") {
        auto [instruction,operand] = program[program_counter];
        program_counter++;

        if (instruction == "READ")
            READ();
        else if (instruction == "WRITE")
            WRITE();
        else if (instruction == "DUPE")
            DUPE();
        else if (instruction == "ADD")
            ADD();
        else if (instruction == "SUB")
            SUB();
        else if (instruction == "MUL")
            MUL();
        else if (instruction == "REVERSE")
            REVERSE();
        else if (instruction == "PUSH")
            PUSH(stoi(operand));
        else if (instruction == "JEQ")
            JEQ(operand);
        else if (instruction == "JGT")
            JGT(operand);
        else if (instruction == "JMP")
            JMP(operand);
        else if (instruction == "CALL")
            CALL(operand);
        else if (instruction == "RET")
            RET();
        else if (instruction == "SIZE")
            SIZE();
    }
    return 0;
}



