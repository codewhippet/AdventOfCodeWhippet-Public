#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle17_2024_Types
{
	struct Program
	{
		int64_t Register[3];

		vector<int64_t> Operations;
	};

	enum Register
	{
		A, B, C
	};

	enum class Operation
	{
		adv,
		bxl,
		bst,
		jnz,
		bxc,
		out,
		bdv,
		cdv,
	};
}

using namespace Puzzle17_2024_Types;


static Program ParseInput(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	Program p;

	assert(sscanf(lines[0].c_str(), "Register A: %lld", &p.Register[0]) == 1);
	assert(sscanf(lines[1].c_str(), "Register B: %lld", &p.Register[1]) == 1);
	assert(sscanf(lines[2].c_str(), "Register C: %lld", &p.Register[2]) == 1);

	string ops = lines[4].substr(sizeof("Program: ") - 1);
	istringstream opIn{ ops };

	while (true)
	{
		string op;
		if (!getline(opIn, op, ','))
			break;

		p.Operations.push_back(atoll(op.c_str()));
	}

	return p;
}

static int64_t ComboOperand(int64_t r, const Program* p)
{
	assert(r != 7);
	return (r > 3 ? p->Register[r - 4] : r);
}

static int64_t Execute(Program* p, string* tapeOut, const vector<int64_t>* checksum)
{
	size_t pc = 0;
	size_t chk = 0;

	while (pc < p->Operations.size())
	{
		int64_t opcode = p->Operations[pc + 0];
		int64_t operand = p->Operations[pc + 1];
		assert(operand != 7);

		switch ((Operation)opcode)
		{
		case Operation::adv:
			{
				int64_t numerator = p->Register[Register::A];
				int64_t combo = ComboOperand(operand, p);
				p->Register[Register::A] = numerator >> combo;
			}
			break;

		case Operation::bxl:
			{
				p->Register[Register::B] ^= operand;
			}
			break;

		case Operation::bst:
			{
				int64_t combo = ComboOperand(operand, p) % 8;
				p->Register[Register::B] = combo;
			}
			break;

		case Operation::jnz:
			{
				if (p->Register[Register::A] != 0)
				{
					pc = operand - 2; // Subtract 2 because we're just about to increment by 2
				}
			}
			break;

		case Operation::bxc:
			{
				p->Register[Register::B] ^= p->Register[Register::C];
			}
			break;

		case Operation::out:
			{
				int64_t combo = ComboOperand(operand, p) % 8;

				if (tapeOut)
				{
					char buffer[16];
					(*tapeOut) += _itoa((int)combo, buffer, 10);
					(*tapeOut) += ',';
				}
				if (checksum)
				{
					if (chk == checksum->size())
					{
						return checksum->size();
					}
					if (combo != (*checksum)[chk++])
					{
						return chk;
					}
				}
			}
			break;

		case Operation::bdv:
			{
				int64_t numerator = p->Register[Register::A];
				int64_t combo = ComboOperand(operand, p);
				p->Register[Register::B] = numerator >> combo;
			}
			break;

		case Operation::cdv:
			{
				int64_t numerator = p->Register[Register::A];
				int64_t combo = ComboOperand(operand, p);
				p->Register[Register::C] = numerator >> combo;
			}
			break;
		}

		pc += 2;
	}

	if (checksum)
	{
		return (chk == checksum->size() ? -1 : chk);
	}

	return -1;
}

static int64_t Step(int64_t A)
{
	int64_t B = A % 8;
	B = B ^ 1;
	int64_t C = A >> B;
	B = B ^ 5;
	B = B ^ C;
	return B % 8;
}

static void Solve(int64_t sevenBits, size_t opIndex, const vector<int64_t>& operations, int64_t aPartial, int64_t* aMin)
{
	if (opIndex == operations.size())
	{
		if (sevenBits == 0)
		{
			*aMin = min(*aMin, aPartial);
		}
		return;
	}

	for (int64_t threeBits = 0; threeBits < (1ll << 3); threeBits++)
	{
		int64_t tenBits = (threeBits << 7) | sevenBits;
		int64_t testOutput = Step(tenBits);
		if ((testOutput % 8) == operations[opIndex])
		{
			int64_t newAPartial = aPartial | (threeBits << (7 + 3 * opIndex));
			Solve(tenBits >> 3, opIndex + 1, operations, newAPartial, aMin);
		}
	}
}

static void Puzzle17_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Program p = ParseInput(input);

	string tape;
	Execute(&p, &tape, nullptr);
	tape.pop_back();

	printf("[2024] Puzzle17_A: %s\n", tape.c_str());
}

static void Puzzle17_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Program p = ParseInput(input);

	int64_t answer = numeric_limits<int64_t>::max();
	for (int64_t sevenBits = 0; sevenBits < (1ll << 7); sevenBits++)
	{
		Solve(sevenBits, 0, p.Operations, sevenBits, &answer);
	}

	printf("[2024] Puzzle17_B: %" PRId64 "\n", answer);
}

void Puzzle17_2024(const string& filename)
{
	Puzzle17_A(filename);
	Puzzle17_B(filename);
}
