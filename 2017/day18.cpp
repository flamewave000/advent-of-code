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

enum commands { snd = '>', set = '=', add = '+', mul = '*', mod = '%', rcv = '<', jgz = '^' };
ostream& operator<<(ostream &o, const commands &cmd) { return o << (char)cmd; }

typedef vector<instruction*> command;
typedef vector<command> cmd_set;

class processor
{
	unordered_map<reg_type, val_type> _r;
	const cmd_set &_c;
  public:
	int64_t idx, sub_idx;

  public:
	processor(const cmd_set &commands) : _c(commands), idx(0), sub_idx(0) {}
	inline val_type &get( reg_type key) {
		if (_r.find(key) == _r.end()) _r[key] = 0;
		return _r[key];
	}
	inline reg_type nextc() { return ((char_inst*)_c[idx][sub_idx++])->val; }
	inline val_type nexti() { return ((int_inst*)_c[idx][sub_idx++])->val; }
	inline val_type get_value() {
		return nextc() == 'i' ? nexti() : get(nextc());
	}
	void print() {
		printf("\nREGISTERS\n");
		for(auto pair : _r) {
			printf("\t%c: %lld\n", pair.first, pair.second);
		}
		fflush(stdout);
	}

  public:
	void set() {
		const reg_type reg = nextc();
		get(reg) = get_value();
	}
	void add() {
		const reg_type reg = nextc();
		get(reg) += get_value();
	}
	void mul() {
		const reg_type reg = nextc();
		get(reg) *= get_value();
	}
	void mod() {
		val_type &i = get(nextc());
		const val_type val = get_value();
		i = i - ((i / val) * val);
	}
	void jgz() {
		if (get_value() > 0)
			idx += get_value() - 1;
	}
};

val_type puzzle_a(const cmd_set &cmds)
{
	processor proc(cmds);
	val_type played = 0;
	 reg_type reg;
	val_type val;
	for (size_t size = cmds.size(); proc.idx < size; proc.idx++)
	{
		proc.sub_idx = 0;
		switch (proc.nextc())
		{
		case set:
			proc.set();
			continue;
		case add:
			proc.add();
			continue;
		case mul:
			proc.mul();
			continue;
		case mod:
			proc.mod();
			continue;
		case jgz:
			proc.jgz();
			continue;
		case snd:
			played = proc.get_value();
			continue;
		case rcv:
			if(proc.get_value() > 0)
				proc.idx = size;
			continue;
		default:
			cout << "UNKNOWN COMMAND" << endl;
		}
	}
	proc.print();
	return played;
}

val_type puzzle_b(const cmd_set &cmds)
{
	struct program {
		processor proc;
		queue<val_type> queue;
		int send_count;
		bool blocked;
		program(const cmd_set &cmds) : proc(cmds), send_count(0), blocked(false) {}
	};
	program p0(cmds), p1(cmds);
	p0.proc.get('p') = 0;
	p1.proc.get('p') = 1;

	reg_type reg;
	val_type val;
	program *current = &p0, *pending = &p1;
	for (size_t size = cmds.size(); current->proc.idx < size; current->proc.idx++)
	{
		if(current->blocked && current->queue.size() == 0)
			break;
		current->proc.sub_idx = 0;
		switch (current->proc.nextc())
		{
		case set:
			current->proc.set();
			continue;
		case add:
			current->proc.add();
			continue;
		case mul:
			current->proc.mul();
			continue;
		case mod:
			current->proc.mod();
			continue;
		case jgz:
			current->proc.jgz();
			continue;
		case snd:
			val = current->proc.get_value();
			pending->queue.push(val);
			current->send_count++;
			cout << current << " sent " << pending << " the value " << val << endl;
			continue;
		case rcv:
			if(current->queue.size() == 0) {
				current->blocked = true;
				pending->proc.idx--;
				swap(current, pending);
				cout << "SWAPPED" << endl;
			} else {
				current->blocked = false;
				current->proc.nextc(); // throw away the character identifier
				reg = current->proc.nextc();
				current->proc.get(reg) = current->queue.front();
				current->queue.pop();
				cout << current << " received " << current->proc.get(reg) << " for " << reg << endl;
			}
			break;
		default:
			cout << "UNKNOWN COMMAND" << endl;
		}
		if(current->proc.idx == size - 1) {
			current->proc.idx++;
			current->blocked = true; // program is finished and now blocked
			swap(current, pending);
		}
	}

	p0.proc.print();
	p1.proc.print();

	return p1.send_count;
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


string test_data = R"(snd 1
snd 2
snd p
rcv a
rcv b
rcv c
rcv d)";

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
				}
				break;
			case 'm': // mul/mod
				if(cmd[1] == 'u') { // multiply
					com.push_back(new char_inst(commands::mul));
					com.push_back(getc(data, &c));
					insert_typed(&com, data, &c);
				}
				else { // modulo
					com.push_back(new char_inst(commands::mod));
					com.push_back(getc(data, &c));
					insert_typed(&com, data, &c);
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

Your puzzle answer was 9423.

--- Part Two ---

As you congratulate yourself for a job well done, you notice that the documentation has been on the back of the tablet this entire time. While you actually got most of the instructions correct, there are a few key differences. This assembly code isn't about sound at all - it's meant to be run twice at the same time.

Each running copy of the program has its own set of registers and follows the code independently - in fact, the programs don't even necessarily run at the same speed. To coordinate, they use the send (snd) and receive (rcv) instructions:

	snd X sends the value of X to the other program. These values wait in a queue until that program is ready to receive them. Each program has its own message queue, so a program can never receive a message it sent.
	rcv X receives the next value and stores it in register X. If no values are in the queue, the program waits for a value to be sent to it. Programs do not continue to the next instruction until they have received a value. Values are received in the order they are sent.

Each program also has its own program ID (one 0 and the other 1); the register p should begin with this value.

For example:

snd 1
snd 2
snd p
rcv a
rcv b
rcv c
rcv d

Both programs begin by sending three values to the other. Program 0 sends 1, 2, 0; program 1 sends 1, 2, 1. Then, each program receives a value (both 1) and stores it in a, receives another value (both 2) and stores it in b, and then each receives the program ID of the other program (program 0 receives 1; program 1 receives 0) and stores it in c. Each program now sees a different value in its own copy of register c.

Finally, both programs try to rcv a fourth time, but no data is waiting for either of them, and they reach a deadlock. When this happens, both programs terminate.

It should be noted that it would be equally valid for the programs to run at different speeds; for example, program 0 might have sent all three values and then stopped at the first rcv before program 1 executed even its first instruction.

Once both of your programs have terminated (regardless of what caused them to do so), how many times did program 1 send a value?

*/