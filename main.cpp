#include <bits/stdc++.h>

using namespace std;

enum Opcode {
    OP_READ, OP_WRITE, OP_DUPE, OP_PUSH, OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_AND, OP_OR, OP_XOR, OP_REVERSE, OP_JEQ, OP_JGT, OP_JMP, OP_CALL,
    OP_RET, OP_SIZE, OP_EMPTY, OP_HALT, OP_LOAD
};

stack<int> stck;
int program_counter =0;
vector<pair<Opcode,string>> program;
unordered_map<string, int> flags;
unordered_map<string, Opcode> opcode_map = {
    {"READ", OP_READ}, {"WRITE", OP_WRITE}, {"DUPE", OP_DUPE}, {"PUSH", OP_PUSH},
    {"ADD", OP_ADD}, {"SUB", OP_SUB}, {"MUL", OP_MUL}, {"DIV", OP_DIV},
    {"AND", OP_AND}, {"OR", OP_OR}, {"XOR", OP_XOR}, {"REVERSE", OP_REVERSE},
    {"JEQ", OP_JEQ}, {"JGT", OP_JGT}, {"JMP", OP_JMP}, {"CALL", OP_CALL},
    {"RET", OP_RET}, {"SIZE", OP_SIZE}, {"EMPTY", OP_EMPTY}, {"HALT", OP_HALT},
    {"LOAD", OP_LOAD}
};

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

void reverse_stack(int n) {
    queue<int> q;
    while(n--) {q.push(stck.top()); stck.pop();}
    while (!q.empty()) stck.push(q.front()), q.pop();
}

void REVERSE(const string &operand) {
    if (operand.empty()) {
        stack<int> temp;
        while(!stck.empty()) {
            temp.push(stck.top());
            stck.pop();
        }
        stck = temp;
    } else if (operand  == "TOP") {
        int x = stck.top();
        stck.pop();
        reverse_stack(x);
    }
    else reverse_stack(stoi(operand));
}

void ADD() {
    auto [a,b] = GET();
    stck.push(a + b);
}

void SUB() {
    auto [a,b] = GET();
    stck.push(a - b);
}

void AND() {
    auto [a,b] = GET();
    stck.push(a & b);
}

void OR() {
    auto [a,b] = GET();
    stck.push(a | b);
}

void XOR() {
    auto [a,b] = GET();
    stck.push(a ^ b);
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

void WRITE(const string &operand) {
    if (operand.empty()) {
        cout<<stck.top()<<"\n";
        stck.pop();
    }
    else cout<<operand<<"\n";
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
        auto it = opcode_map.find(instr);
        if (it != opcode_map.end()) {
            program.emplace_back(it->second, operand);
        }
    }
}

// DANGEROUS ONE

void LOAD(const string &file) {
    program.clear();
    flags.clear();
    program_counter = 0;
    parse_program(file);
    program_counter = flags["MAIN"];

}
int main() {

    parse_program("program.dio");
    program_counter = flags["MAIN"];

    while (program[program_counter].first != OP_HALT) {
        auto [opcode, operand] = program[program_counter];
        program_counter++;

        switch (opcode) {
            case OP_READ: READ(); break;
            case OP_WRITE: WRITE(operand); break;
            case OP_DUPE: DUPE(); break;
            case OP_PUSH: PUSH(stoi(operand)); break;
            case OP_ADD: ADD(); break;
            case OP_SUB: SUB(); break;
            case OP_MUL: MUL(); break;
            case OP_DIV: DIV(); break;
            case OP_AND: AND(); break;
            case OP_OR: OR(); break;
            case OP_XOR: XOR(); break;
            case OP_REVERSE: REVERSE(operand); break;
            case OP_JEQ: JEQ(operand); break;
            case OP_JGT: JGT(operand); break;
            case OP_JMP: JMP(operand); break;
            case OP_CALL: CALL(operand); break;
            case OP_RET: RET(); break;
            case OP_SIZE: SIZE(); break;
            case OP_EMPTY: EMPTY(); break;
            case OP_LOAD: LOAD(operand); continue;
            case OP_HALT: break;
        }
    }
    return 0;
}



