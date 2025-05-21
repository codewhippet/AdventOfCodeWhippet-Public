#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle16_2019_Types
{
	struct SignalModulationIterator
	{
		using value_type = int64_t;
		using difference_type = ptrdiff_t;

		int64_t Index = -1;
		int64_t Scale = -1;

		int64_t Current = 0;

		SignalModulationIterator() = default;
		SignalModulationIterator(int64_t index, int64_t scale)
			: Index(index - 1)
			, Scale(scale)
		{
			GenerateNext();
		}

		SignalModulationIterator& operator++()
		{
			GenerateNext();
			return *this;
		}

		SignalModulationIterator operator++(int)
		{
			SignalModulationIterator copy(*this);
			GenerateNext();
			return copy;
		}

		const value_type& operator*() const
		{
			return Current;
		}

		const value_type* operator->() const
		{
			return &Current;
		}

		auto operator<=>(const SignalModulationIterator&) const = default;

	private:
		void GenerateNext()
		{
			Index++;

			int64_t shiftedIndex = Index + 1;
			int64_t scaledIndex = shiftedIndex / Scale;

			static const array<int64_t, 4> basePattern{ 0, 1, 0, -1 };

			Current = basePattern[scaledIndex % basePattern.size()];
		}
	};

	static_assert(input_or_output_iterator<SignalModulationIterator>);

	struct SignalModulationRange
	{
		int64_t Scale = -1;

		SignalModulationRange() = default;
		explicit SignalModulationRange(int64_t scale)
			: Scale(scale)
		{
		}

		SignalModulationIterator begin() const
		{
			return SignalModulationIterator{ 0, Scale };
		}

		SignalModulationIterator end() const
		{
			return SignalModulationIterator{ numeric_limits<int64_t>::max(), Scale };
		}
	};

	static_assert(ranges::input_range<SignalModulationRange>);

	SignalModulationRange SignalModulation(int64_t scale)
	{
		return SignalModulationRange{ scale };
	}
}

using namespace Puzzle16_2019_Types;


static vector<int64_t> ReadSignal(istream& input)
{
	vector<int64_t> signal;
	ranges::copy(ReadSingleLine(input) | views::transform([](char c) { return c - '0'; }), back_inserter(signal));
	return signal;
}

static vector<int64_t> StepSignal(const vector<int64_t>& signal)
{
	vector<int64_t> newSignal(signal.size());
	for (int64_t i = 0; i < (int64_t)signal.size(); i++)
	{
		auto signalModulation = SignalModulation(i + 1);
		int64_t product = inner_product(signal.begin(), signal.end(), signalModulation.begin(), 0ll);
		newSignal[i] = abs(product) % 10;
	}
	return newSignal;
}

static void Puzzle16_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> signal = ReadSignal(input);

	for (int64_t i = 0; i < 100; i++)
	{
		signal = StepSignal(signal);
	}

	auto answerDigits = signal | views::take(8);
	int64_t answer = accumulate(answerDigits.begin(), answerDigits.end(), 0ll,
		[](int64_t a, int64_t b)
		{
			return a * 10 + b;
		});

	printf("[2019] Puzzle16_A: %" PRId64 "\n", answer);
}


static void Puzzle16_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const vector<int64_t> originalSignal = ReadSignal(input);
	vector<int64_t> signal;
	signal.reserve(originalSignal.size() * 10000);
	for (int64_t i = 0; i < 10000; i++)
	{
		signal.insert(signal.end(), originalSignal.begin(), originalSignal.end());
	}

	auto offsetDigits = signal | views::take(7);
	int64_t offset = accumulate(offsetDigits.begin(), offsetDigits.end(), 0ll,
		[](int64_t a, int64_t b)
		{
			return a * 10 + b;
		});

	// By the time the offset is in the upper half of the range, the first 111111 pattern
	// has grown to cover the entire end of the signal for each row. This allows us to do
	// a simple running sum to calculate the tail
	assert(offset > ((int64_t)signal.size() / 2));

	for (int64_t step = 0; step < 100; step++)
	{
		int64_t runningSum = 0;
		for (int64_t i = (int64_t)signal.size() - 1; i >= offset; i--)
		{
			runningSum += signal[i];
			signal[i] = runningSum % 10;
		}
	}

	auto answerDigits = signal | views::drop(offset) | views::take(8);
	int64_t answer = accumulate(answerDigits.begin(), answerDigits.end(), 0ll,
		[](int64_t a, int64_t b)
		{
			return a * 10 + b;
		});

	printf("[2019] Puzzle16_B: %" PRId64 "\n", answer);
}

void Puzzle16_2019(const string& filename)
{
	Puzzle16_A(filename);
	Puzzle16_B(filename);
}
