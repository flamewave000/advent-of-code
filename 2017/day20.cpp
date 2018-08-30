#include "util.h"
#include <regex>

#define TEST false

using namespace std;
using namespace strx;
using namespace linq;

const char *test_data = R"(p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>
p=<4,0,0>, v=<0,0,0>, a=<-2,0,0>)";

struct vec3 {
	int64_t x, y, z;
	vec3() : x(0), y(0), z(0) {}
	vec3(int x, int y, int z) : x(x), y(y), z(z) {}
	vec3 &operator+=(const vec3 &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
};
// inline vec3 operator+(const vec3 &lhs, const vec3 &rhs) {
// 	return vec3 {lhs.x + rhs.x,
// 			lhs.y + rhs.y,
// 			lhs.z + rhs.z};
// }
// inline vec3 operator-(const vec3 &lhs, const vec3 &rhs) {
// 	return {lhs.x - rhs.x,
// 			lhs.y - rhs.y,
// 			lhs.z - rhs.z};
// }
// inline vec3 operator*(const vec3 &lhs, const vec3 &rhs) {
// 	return {lhs.x * rhs.x,
// 			lhs.y * rhs.y,
// 			lhs.z * rhs.z};
// }
// inline vec3 operator/(const vec3 &lhs, const vec3 &rhs) {
// 	return {lhs.x / rhs.x,
// 			lhs.y / rhs.y,
// 			lhs.z / rhs.z};
// }
inline ostream &operator<<(ostream &lhs, const vec3 &rhs) {
	return lhs << '{' << rhs.x << ',' << rhs.y << ',' << rhs.z << '}';
}

struct particle {
	vec3 pos, vel, acc;

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

int puzzle_a(const string &input) {
	return 0;
}

int puzzle_b(const string &input) {
	return 0;
}

int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

	vector<particle> particles;
#if TEST
	vector<string> lines = split(test_data, '\n');
#else
	vector<string> lines = split(c.input, '\n');
#endif
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

	uint64_t distance = UINT64_MAX;
	size_t index = 0;

	const int ITERATIONS = 1000000;

	for (size_t c = 0, i = 0, size = particles.size(); i < size; i++) {
		// cout << "Particle(" << i << "): " << particles[i] << flush;
		for (c = 0; c < ITERATIONS; c++)
			particles[i].tick();
		auto dist = particles[i].distance();
		// cout << " Result: " << particles[i] << " (" << distance << ')' << endl;
		if (dist < distance) {
			distance = dist;
			index = i;
		}
	}

	// auto result = c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input);
	cout << "\nResult: " << index << ": " << distance << endl;
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

*/