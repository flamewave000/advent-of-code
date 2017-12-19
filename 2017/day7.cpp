#include "util.h"

using namespace std;
using namespace strx;

string puzzle_a(const string &input);
string puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, 7);

	// cout << "Data: " << c.input;
	cout << "\n\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}
struct node
{
	string name;
	size_t weight;
	vector<string> children;
	string parent;
};
typedef unordered_map<string, node> node_map;

size_t calc_weights(const node &self, node_map &nodes)
{
	size_t total = self.weight;
	size_t norm = ~0;
	size_t child_weight;
	for (auto child : self.children)
	{
		child_weight = calc_weights(nodes[child], nodes);
		if (norm == ~0)
			norm = child_weight;
		else if (norm != child_weight)
		{
			cout << endl
				 << "norm: {0}, curr: {1}"_f % norm % child_weight << endl;
			throw std::length_error(self.name);
			return -1;
		}
		total += child_weight;
	}
	return total;
}

node process_line(const std::string &line)
{
	vector<string> tokens = split(line, ' ');
	size_t size = tokens.size();
	node n;
	n.name = tokens[0];
	n.weight = atoi(tokens[1].substr(1, tokens[1].size() - 2).c_str());
	if (size > 2)
	{
		// skip index 2 as it is just '->'
		for (size_t c = 3, size = tokens.size(); c < size; c++)
		{
			if (c == size - 1)
			{
				n.children.push_back(tokens[c]);
			}
			else
			{
				n.children.push_back(tokens[c].substr(0, tokens[c].size() - 1));
			}
		}
	}
	return std::move(n);
}
void process(const string &input, node_map &nodes, node &root)
{
	vector<string> lines = split(input, '\n');
	node n;
	for (auto line : lines)
	{
		n = process_line(line);
		nodes[n.name] = n;
	}
	for (auto pair : nodes)
	{
		if (pair.second.children.size() > 0)
		{
			for (auto child : pair.second.children)
			{
				nodes[child].parent = pair.first;
			}
		}
	}
	root = nodes.begin()->second;
	while (root.parent != "")
	{
		root = nodes[root.parent];
	}
}
void draw_nodes(const node_map &nodes, const node &root, int count = 0)
{
	char *tabs = new char[count + 2];
	memset(tabs, '\t', count + 1);
	tabs[count + 1] = '\0';

	printf("%s(%lu)", root.name.c_str(), root.weight);
	if (root.children.size() > 0)
		printf("\n%s\\", tabs);
	for (auto child : root.children)
	{
		printf("\n%s ", tabs);
		draw_nodes(nodes, nodes.find(child)->second, count + 1);
	}
	delete[] tabs;
}

string puzzle_a(const string &input)
{
	node_map nodes;
	node root;
	process(input, nodes, root);
	draw_nodes(nodes, root);
	return root.name;
}

string puzzle_b(const string &input)
{
	node_map nodes;
	node root;
	process(input, nodes, root);
	draw_nodes(nodes, root);
	cout << endl;
	try
	{// find the unbalanced program
		size_t weight = calc_weights(root, nodes);
	}
	catch (std::length_error err)
	{
		node bad = nodes[err.what()];
		unordered_map<size_t, pair<int, node *>> weights;
		auto it = weights.begin();
		for (size_t c = 0, curr = 0, size = bad.children.size(); c < size; c++)
		{
			curr = calc_weights(nodes[bad.children[c]], nodes);
			if ((it = weights.find(curr)) == weights.end())
				weights[curr] = {1, &nodes[bad.children[c]]};
			else
				(it->second.first)++;
		}
		auto target = weights.begin();
		auto invalid = target;
		if (target->second.first == 1)
			target++;
		else
			invalid++;
		return R"({0} is unbalanced
	weight:{1}
	 total:{2}
	target:{3}
new weight:{4})"_f
			   % invalid->second.second->name
			   % invalid->second.second->weight
			   % invalid->first % target->first
			   % (invalid->second.second->weight + (target->first - invalid->first))
			   % endf;
	}
	return "all balanced";
}

/*
http://adventofcode.com/2017/day/7

--- Day 7: Recursive Circus ---
Wandering further through the circuits of the computer, you come upon a tower of programs that have gotten themselves into a bit of trouble. A recursive algorithm has gotten out of hand, and now they're balanced precariously in a large tower.
One program at the bottom supports the entire tower. It's holding a large disc, and on the disc are balanced several more sub-towers. At the bottom of these sub-towers, standing on the bottom disc, are other programs, each holding their own disc, and so on. At the very tops of these sub-sub-sub-...-towers, many programs stand simply keeping the disc below them balanced but with no disc of their own.
You offer to help, but first you need to understand the structure of these towers. You ask each program to yell out their name, their weight, and (if they're holding a disc) the names of the programs immediately above them balancing on that disc. You write this information down (your puzzle input). Unfortunately, in their panic, they don't do this in an orderly fashion; by the time you're done, you're not sure which program gave which information.
For example, if your list is the following:
pbga (66)
xhth (57)
ebii (61)
havc (66)
ktlj (57)
fwft (72) -> ktlj, cntj, xhth
qoyq (66)
padx (45) -> pbga, havc, qoyq
tknk (41) -> ugml, padx, fwft
jptl (61)
ugml (68) -> gyxo, ebii, jptl
gyxo (61)
cntj (57)
...then you would be able to recreate the structure of the towers that looks like this:
                gyxo
              /     
         ugml - ebii
       /      \     
      |         jptl
      |        
      |         pbga
     /        /
tknk --- padx - havc
     \        \
      |         qoyq
      |             
      |         ktlj
       \      /     
         fwft - cntj
              \     
                xhth
In this example, tknk is at the bottom of the tower (the bottom program), and is holding up ugml, padx, and fwft. Those programs are, in turn, holding up other programs; in this example, none of those programs are holding up any other programs, and are all the tops of their own towers. (The actual tower balancing in front of you is much larger.)
Before you're ready to help them, you need to make sure your information is correct. What is the name of the bottom program?

--- Part Two ---
The programs explain the situation: they can't get down. Rather, they could get down, if they weren't expending all of their energy trying to keep the tower balanced. Apparently, one program has the wrong weight, and until it's fixed, they're stuck here.
For any program holding a disc, each program standing on that disc forms a sub-tower. Each of those sub-towers are supposed to be the same weight, or the disc itself isn't balanced. The weight of a tower is the sum of the weights of the programs in that tower.
In the example above, this means that for ugml's disc to be balanced, gyxo, ebii, and jptl must all have the same weight, and they do: 61.
However, for tknk to be balanced, each of the programs standing on its disc and all programs above it must each match. This means that the following sums must all be the same:
ugml + (gyxo + ebii + jptl) = 68 + (61 + 61 + 61) = 251
padx + (pbga + havc + qoyq) = 45 + (66 + 66 + 66) = 243
fwft + (ktlj + cntj + xhth) = 72 + (57 + 57 + 57) = 243
As you can see, tknk's disc is unbalanced: ugml's stack is heavier than the other two. Even though the nodes above ugml are balanced, ugml itself is too heavy: it needs to be 8 units lighter for its stack to weigh 243 and keep the towers balanced. If this change were made, its weight would be 60.
Given that exactly one program is the wrong weight, what would its weight need to be to balance the entire tower?

*/