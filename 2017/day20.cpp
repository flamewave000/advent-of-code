#include "util.h"
#include <chrono>
#include <regex>

#define TEST false

using namespace std;
using namespace strx;
using namespace linq;

const char *test_data = R"(p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>
p=<4,0,0>, v=<0,0,0>, a=<-2,0,0>)";

struct vec3 {
	int x, y, z;
	vec3() : x(0), y(0), z(0) {}
	vec3(int x, int y, int z) : x(x), y(y), z(z) {}
	vec3 &operator+=(const vec3 &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
	bool operator==(const vec3 &rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
};

struct vec3_hash {
	size_t operator()(const vec3 &key) const {
		return (std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1) >> 1) ^ (std::hash<int>()(key.z) << 1);
	}
};
inline ostream &
operator<<(ostream &lhs, const vec3 &rhs) {
	return lhs << '{' << rhs.x << ',' << rhs.y << ',' << rhs.z << '}';
}

struct particle {
	vec3 pos, vel, acc;
	bool dead;

	particle() : dead(false) {}

	inline void tick(bool print = false) {
		if (print)
			cout << "Before pos:" << pos << ", vel:" << vel << ", acc:" << acc << ", dist:" << distance() << endl;
		vel += acc;
		pos += vel;
		if (print)
			cout << "After pos:" << pos << ", vel:" << vel << ", acc:" << acc << ", dist:" << distance() << endl;
	}
	uint64_t distance() { return abs(pos.x) + abs(pos.y) + abs(pos.z); }
};
inline ostream &operator<<(ostream &lhs, const particle &rhs) {
	return lhs << "<pos:" << rhs.pos << ", vel:" << rhs.vel << ", acc:" << rhs.acc << '>';
}

size_t puzzle_a(vector<particle> &particles) {

	uint64_t distance = UINT64_MAX;
	size_t index = 0;

	const int ITERATIONS = 1000;

	cout << "Iterations: " << ITERATIONS << endl;
	for (size_t c = 0, i = 0, size = particles.size(); i < size; i++) {
		for (c = 0; c < ITERATIONS; c++)
			particles[i].tick();
		auto dist = particles[i].distance();
		if (dist < distance) {
			distance = dist;
			index = i;
		}
	}
	return index;
}

struct entry {
	size_t initial;
	size_t count;
	entry() : initial(0), count(0) {}
	entry(size_t initial, size_t count) : initial(initial), count(count) {}
};

int puzzle_b(vector<particle> &particles) {
	const int ITERATIONS = 1000000;
	unordered_map<vec3, entry, vec3_hash> positions;
	double avg = 0;
	for (size_t c = 0, i, size = particles.size(); c < ITERATIONS; c++) {
		PROGRESS(c, ITERATIONS, 1000)
		positions.clear();
		PERF_START();
		for (i = 0; i < size; i++) {
			particle &p = particles[i];
			if (p.dead)
				continue;
			p.tick();
			entry &e = positions[particles[i].pos];
			if (e.count == 0) {
				e.initial = i;
				e.count = 1;
				continue;
			}
			e.count++;
			if (e.count >= 1) {
				p.dead = true;
				if (e.count == 2)
					particles[e.initial].dead = true;
			}
		}
		avg += PERF_END();
	}
	cout << "\nDuration: " << (avg > 1000.0 ? avg / 1000.0 : avg) << (avg > 1000.0 ? "s Avg: " : "ms Avg: ") << (avg / (double)ITERATIONS) << "ms" << endl;
	int count = 0;
	for (size_t c = 0, size = particles.size(); c < size; c++) {
		if (!particles[c].dead)
			count++;
	}
	return count;
}

int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

#if TEST
	vector<string> lines = split(test_data, '\n');
#else
	vector<string> lines = split(c.input, '\n');
#endif

	vector<particle> particles;
	regex rx("p=<(-?\\d+),(-?\\d+),(-?\\d+)>, v=<(-?\\d+),(-?\\d+),(-?\\d+)>, a=<(-?\\d+),(-?\\d+),(-?\\d+)>");
	smatch matches;
	for (auto line : lines) {
		regex_search(line, matches, rx);
		particle p;
		p.pos.x = stoi(matches[1].str());
		p.pos.y = stoi(matches[2].str());
		p.pos.z = stoi(matches[3].str());
		p.vel.x = stoi(matches[4].str());
		p.vel.y = stoi(matches[5].str());
		p.vel.z = stoi(matches[6].str());
		p.acc.x = stoi(matches[7].str());
		p.acc.y = stoi(matches[8].str());
		p.acc.z = stoi(matches[9].str());
		particles.push_back(move(p));
	}

	size_t result = c.puzzle == 1 ? puzzle_a(particles) : puzzle_b(particles);

	// auto result = c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input);
	cout << "\nResult: " << result << endl;
	return 0;
}

/*
http://adventofcode.com/2017/day/20

--- Day 20: Particle Swarm ---

Suddenly, the GPU contacts you, asking for help. Someone has asked it to simulate too many particles, and it won't be able to finish them all in time to render the next frame at this rate.

It transmits to you a buffer (your puzzle input) listing each particle in order (starting with particle 0, then particle 1, particle 2, and so on). For each particle, it provides the X, Y, and Z coordinates for the particle's position (p), velocity (v), and acceleration (a), each in the format <X,Y,Z>.

Each tick, all particles are updated simultaneously. A particle's properties are updated in the following order:

    Increase the X velocity by the X acceleration.
    Increase the Y velocity by the Y acceleration.
    Increase the Z velocity by the Z acceleration.
    Increase the X position by the X velocity.
    Increase the Y position by the Y velocity.
    Increase the Z position by the Z velocity.

Because of seemingly tenuous rationale involving z-buffering, the GPU would like to know which particle will stay closest to position <0,0,0> in the long term. Measure this using the Manhattan distance, which in this situation is simply the sum of the absolute values of a particle's X, Y, and Z position.

For example, suppose you are only given two particles, both of which stay entirely on the X-axis (for simplicity). Drawing the current states of particles 0 and 1 (in that order) with an adjacent a number line and diagram of current X positions (marked in parentheses), the following would take place:

p=< 3,0,0>, v=< 2,0,0>, a=<-1,0,0>    -4 -3 -2 -1  0  1  2  3  4
p=< 4,0,0>, v=< 0,0,0>, a=<-2,0,0>                         (0)(1)

p=< 4,0,0>, v=< 1,0,0>, a=<-1,0,0>    -4 -3 -2 -1  0  1  2  3  4
p=< 2,0,0>, v=<-2,0,0>, a=<-2,0,0>                      (1)   (0)

p=< 4,0,0>, v=< 0,0,0>, a=<-1,0,0>    -4 -3 -2 -1  0  1  2  3  4
p=<-2,0,0>, v=<-4,0,0>, a=<-2,0,0>          (1)               (0)

p=< 3,0,0>, v=<-1,0,0>, a=<-1,0,0>    -4 -3 -2 -1  0  1  2  3  4
p=<-8,0,0>, v=<-6,0,0>, a=<-2,0,0>                         (0)   

At this point, particle 1 will never be closer to <0,0,0> than particle 0, and so, in the long run, particle 0 will stay closest.

Which particle will stay closest to position <0,0,0> in the long term?

Your puzzle answer was 308.

--- Part Two ---

To simplify the problem further, the GPU would like to remove any particles that collide. Particles collide if their positions ever exactly match. Because particles are updated simultaneously, more than two particles can collide at the same time and place. Once particles collide, they are removed and cannot collide with anything else after that tick.

For example:

p=<-6,0,0>, v=< 3,0,0>, a=< 0,0,0>    
p=<-4,0,0>, v=< 2,0,0>, a=< 0,0,0>    -6 -5 -4 -3 -2 -1  0  1  2  3
p=<-2,0,0>, v=< 1,0,0>, a=< 0,0,0>    (0)   (1)   (2)            (3)
p=< 3,0,0>, v=<-1,0,0>, a=< 0,0,0>

p=<-3,0,0>, v=< 3,0,0>, a=< 0,0,0>    
p=<-2,0,0>, v=< 2,0,0>, a=< 0,0,0>    -6 -5 -4 -3 -2 -1  0  1  2  3
p=<-1,0,0>, v=< 1,0,0>, a=< 0,0,0>             (0)(1)(2)      (3)   
p=< 2,0,0>, v=<-1,0,0>, a=< 0,0,0>

p=< 0,0,0>, v=< 3,0,0>, a=< 0,0,0>    
p=< 0,0,0>, v=< 2,0,0>, a=< 0,0,0>    -6 -5 -4 -3 -2 -1  0  1  2  3
p=< 0,0,0>, v=< 1,0,0>, a=< 0,0,0>                       X (3)      
p=< 1,0,0>, v=<-1,0,0>, a=< 0,0,0>

------destroyed by collision------    
------destroyed by collision------    -6 -5 -4 -3 -2 -1  0  1  2  3
------destroyed by collision------                      (3)         
p=< 0,0,0>, v=<-1,0,0>, a=< 0,0,0>

In this example, particles 0, 1, and 2 are simultaneously destroyed at the time and place marked X. On the next tick, particle 3 passes through unharmed.

How many particles are left after all collisions are resolved?

Your puzzle answer was 504.

*/