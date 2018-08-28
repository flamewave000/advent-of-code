#include "util.h"

#define TEST false

using namespace std;
using namespace strx;
using namespace linq;

typedef char reg_type;
typedef int64_t val_type;

struct instruction { virtual ~instruction(){} };
struct char_inst : public instruction {
    const reg_type val;
    char_inst(reg_type val) : val(val) {}
 };
struct int_inst : public instruction {
    const val_type val;
    int_inst(val_type val) : val(val) {}
 };
 ostream& operator<<(ostream &o, const instruction* ptr) {
     auto iptr = dynamic_cast<const int_inst*>(ptr);
     auto cptr = dynamic_cast<const char_inst*>(ptr);
     if(iptr != nullptr) return o << iptr->val;
     if(cptr != nullptr) return o << cptr->val;
     return o << '_';
 }

enum commands {
    snd = '>',
    set = '=',
    add = '+',
    mul = '*',
    mod = '%',
    rcv = '<',
    jgz = '^'
};
 ostream& operator<<(ostream &o, const commands &cmd) {
     return o << (char)cmd;
 }

typedef vector<instruction*> command;
typedef vector<command> cmd_set;

class processor
{
    unordered_map<reg_type, val_type> _r;
    const cmd_set &_c;
  public:
    size_t idx;
    size_t sub_idx;
    val_type played;


  public:
    processor(const cmd_set &commands) : _c(commands), idx(0), sub_idx(0) {}
    inline val_type &get(char key) {
        auto it = _r.find(key);
        if (it == _r.end()) {
            _r[key] = 0;
            cout << "new register: " << key << endl;
        }
        return _r[key];
    }
    inline char nextc() {
        return ((char_inst*)_c[idx][sub_idx++])->val;
    }
    inline val_type nexti() {
        return ((int_inst*)_c[idx][sub_idx++])->val;
    }
    inline val_type get_value() {
        const char type = nextc();
        const val_type val = type == 'i' ? nexti() : get(nextc());
        return val;
    }
    void play_sound() {
        played = get_value();
        cout << "play " << played << endl;
    }
    void set() {
        const char reg = nextc();
        get(reg) = get_value();
        cout << reg << " = " << _r[reg] << endl;
    }
    void add() {
        const char reg = nextc();
        const val_type val = get_value();
        get(reg) += val;
        cout << reg << " + " << val << " = " << _r[reg] << endl;
    }
    void multiply() {
        const char reg = nextc();
        const val_type val = get_value();
        get(reg) *= val;
        cout << reg << " * " << val << " = " << _r[reg] << endl;
    }
    void modulo() {
        const char reg = nextc();
        const val_type mod = get_value();
        val_type &i = get(reg);
        i = i - ((i / mod) * mod);
        cout << reg << " % " << mod << " = " << i << endl;
    }
    inline bool try_recover() {
        bool result = get_value() > 0;
        cout << "recover: " << (result ? "true" : "false") << endl;
        return result;
    }
    inline void jump() {
        const bool cond = get_value() > 0;
        const val_type offset = get_value();
        if (cond)
        {
            idx += (offset - 1);
            cout << "jump " << offset << endl;
        }
        else
            cout << "jump NO-OP" << endl;
    }
    void print() {
        printf("\nREGISTERS\n");
        for(auto pair : _r) {
            printf("\t%c: %lld\n", pair.first, pair.second);
        }
        fflush(stdout);
    }
};

val_type puzzle_a(const cmd_set &cmds)
{
    cout << "\nSTART" << endl;
    processor proc(cmds);
    for (size_t size = cmds.size(); proc.idx < cmds.size(); proc.idx++)
    {
        proc.sub_idx = 0;
        const char command = proc.nextc();
        switch (command)
        {
        case snd:
            proc.play_sound();
            continue;
        case set:
            proc.set();
            continue;
        case add:
            proc.add();
            continue;
        case mul:
            proc.multiply();
            continue;
        case mod:
            proc.modulo();
            continue;
        case rcv:
            if(proc.try_recover()) {
                proc.print();
                return proc.played;
            }
            continue;
        case jgz:
            proc.jump();
            continue;
        default:
            cout << "Unkown: " << command << endl;
        }
    }
    proc.print();
    return INT64_MAX;
}

val_type puzzle_b(const cmd_set &instructions)
{
    return INT64_MAX;
}

char_inst* getc(const string &str, size_t *idx) {
    char c = str[*idx];
    (*idx) += 2;
    return new char_inst(c);
}
int_inst* geti(const string &str, size_t *idx) {
    string buff;
    while(str[*idx] != ' ' && str[*idx] != '\n') {
        buff += str[*idx];
        (*idx) += 1;
    }
    (*idx) += 1;
    return new int_inst(stoll(buff));
}
void insert_typed(command *com, const string &str, size_t *idx) {
    if(str[*idx] <= '9') {
        com->push_back(new char_inst('i'));
        com->push_back(geti(str, idx));
    }
    else {
        com->push_back(new char_inst('c'));
        com->push_back(getc(str, idx));
    }
}


string test_data = R"(set a 1
add a 2
mul a a
mod a 5
snd a
set a 0
rcv a
jgz a -1
set a 1
jgz a -2)";

int main(int argc, const char *argv[])
{
    config conf = proc(argc, argv, __FILE__);

    cmd_set commands;

    char cmd[3] = { 0, 0, 0 };
    const string &data = TEST ? test_data : conf.input;
    for(size_t c = 0, size = data.size(); c < size;) {
        cmd[0] = data[c++];
        cmd[1] = data[c++];
        cmd[2] = data[c++];
        c++; // ignore whitespace
        command com;

        switch(cmd[0]) {
            case 'a': // add
                com.push_back(new char_inst(commands::add));
                com.push_back(getc(data, &c));
                insert_typed(&com, data, &c);
                cout << com[0] << com[1] << com[2] << com[3] << endl;
                break;
            case 'r': // recover
                com.push_back(new char_inst(commands::rcv));
                insert_typed(&com, data, &c);
                cout << com[0] << com[1] << com[2] << endl;
                break;
            case 'j': // jump
                com.push_back(new char_inst(commands::jgz));
                insert_typed(&com, data, &c);
                insert_typed(&com, data, &c);
                cout << com[0] << com[1] << com[2] << com[3] << com[4] << endl;
                break;
            case 's': // snd/set
                if(cmd[1] == 'n') { // play sound
                    com.push_back(new char_inst(commands::snd));
                    insert_typed(&com, data, &c);
                    cout << com[0] << com[1] << com[2] << endl;
                }
                else { // set
                    com.push_back(new char_inst(commands::set));
                    com.push_back(getc(data, &c));
                    insert_typed(&com, data, &c);
                    cout << com[0] << com[1] << com[2] << com[3] << endl;
                }
                break;
            case 'm': // mul/mod
                if(cmd[1] == 'u') { // multiply
                    com.push_back(new char_inst(commands::mul));
                    com.push_back(getc(data, &c));
                    insert_typed(&com, data, &c);
                    cout << com[0] << com[1] << com[2] << com[3] << endl;
                }
                else { // modulo
                    com.push_back(new char_inst(commands::mod));
                    com.push_back(getc(data, &c));
                    insert_typed(&com, data, &c);
                    cout << com[0] << com[1] << com[2] << com[3] << endl;
                }
                break;
        }
        commands.push_back(com);
    }

    val_type res;
    if(conf.puzzle == 1)
        res = puzzle_a(commands);
    else
        res = puzzle_b(commands);
    cout << "Result: " << res << endl;
    return 0;
}

/*
http://adventofcode.com/2017/day/18

--- Day 18: Duet ---

You discover a tablet containing some strange assembly code labeled simply "Duet". Rather than bother the sound card with it, you decide to run the code yourself. Unfortunately, you don't see any documentation, so you're left to figure out what the instructions mean on your own.

It seems like the assembly is meant to operate on a set of registers that are each named with a single letter and that can each hold a single integer. You suppose each register should start with a value of 0.

There aren't that many instructions, so it shouldn't be hard to figure out what they do. Here's what you determine:

    snd X plays a sound with a frequency equal to the value of X.
    set X Y sets register X to the value of Y.
    add X Y increases register X by the value of Y.
    mul X Y sets register X to the result of multiplying the value contained in register X by the value of Y.
    mod X Y sets register X to the remainder of dividing the value contained in register X by the value of Y (that is, it sets X to the result of X modulo Y).
    rcv X recovers the frequency of the last sound played, but only when the value of X is not zero. (If it is zero, the command does nothing.)
    jgz X Y jumps with an offset of the value of Y, but only if the value of X is greater than zero. (An offset of 2 skips the next instruction, an offset of -1 jumps to the previous instruction, and so on.)

Many of the instructions can take either a register (a single letter) or a number. The value of a register is the integer it contains; the value of a number is that number.

After each jump instruction, the program continues with the instruction to which the jump jumped. After any other instruction, the program continues with the next instruction. Continuing (or jumping) off either end of the program terminates it.

For example:

set a 1
add a 2
mul a a
mod a 5
snd a
set a 0
rcv a
jgz a -1
set a 1
jgz a -2

    The first four instructions set a to 1, add 2 to it, square it, and then set it to itself modulo 5, resulting in a value of 4.
    Then, a sound with frequency 4 (the value of a) is played.
    After that, a is set to 0, causing the subsequent rcv and jgz instructions to both be skipped (rcv because a is 0, and jgz because a is not greater than 0).
    Finally, a is set to 1, causing the next jgz instruction to activate, jumping back two instructions to another jump, which jumps again to the rcv, which ultimately triggers the recover operation.

At the time the recover operation is executed, the frequency of the last sound played is 4.

What is the value of the recovered frequency (the value of the most recently played sound) the first time a rcv instruction is executed with a non-zero value?

*/