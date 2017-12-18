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
unordered_map<string, node> process(const string &input)
{
    unordered_map<string, node> nodes;
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
    return std::move(nodes);
}
void draw_nodes(const unordered_map<string, node> &nodes, const node &root, int count = 0)
{
    char *tabs = new char[count + 2];
    memset(tabs, '\t', count + 1);
    tabs[count + 1] = '\0';

    printf("%s", root.name.c_str());
    if(root.children.size() > 0) printf("\n%s\\", tabs);
    for (auto child : root.children)
    {
        printf("\n%s ", tabs);
        draw_nodes(nodes, nodes.find(child)->second, count + 1);
    }
    delete[] tabs;
}

string puzzle_a(const string &input)
{
    unordered_map<string, node> nodes = process(input);
    node current = nodes.begin()->second;
    while (current.parent != "")
    {
        current = nodes[current.parent];
    }
    draw_nodes(nodes, current);
    // find the root parent
    return current.name;
}

string puzzle_b(const string &input)
{
    return "";
}

/*

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

*/