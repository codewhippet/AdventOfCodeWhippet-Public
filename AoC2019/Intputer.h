#pragma once

#include <istream>
#include <map>
#include <deque>
#include <vector>

struct IntputerIO
{
	std::deque<int64_t> Read;
	std::deque<int64_t> Write;
};

class Intputer
{
public:

	enum class ExecutionResult
	{
		Finished,
		PendingIo,
		Breakpoint,
		Exception,
	};

	Intputer() = default;
	Intputer(Intputer&&) = default;
	explicit Intputer(std::istream& input);

	virtual ~Intputer() = default;

	void ReadProgram(std::istream& input);
	void CopyProgram(const std::vector<int64_t>& program);
	void Reset(const std::vector<int64_t>& program);

	ExecutionResult Execute(int64_t breakAfter = std::numeric_limits<int64_t>::max());

	std::deque<int64_t>* GetReadQueue();
	std::deque<int64_t>* GetWriteQueue();

	void SetReadQueue(std::deque<int64_t>* readQueue);
	void SetWriteQueue(std::deque<int64_t>* writeQueue);
	void SetReadWriteQueues(std::deque<int64_t>* readQueue, std::deque<int64_t>* writeQueue);
	void SetReadWriteQueues(IntputerIO *io);

protected:

	Intputer(const Intputer& other);

	enum class ParameterMode
	{
		Position,
		Immediate,
		Relative,
	};

	int64_t ReadParam(int64_t parameter, ParameterMode mode);
	void WriteParam(int64_t parameter, ParameterMode mode, int64_t value);

	std::map<int64_t, int64_t> Program;
	int64_t PC = 0;
	int64_t RB = 0;

	std::deque<int64_t>* ReadQueue = nullptr;
	std::deque<int64_t>* WriteQueue = nullptr;
};

class IntputerWithIO : public Intputer
{
public:
	IntputerWithIO();
	IntputerWithIO(const IntputerWithIO& other);
	IntputerWithIO(IntputerWithIO&& other);
	explicit IntputerWithIO(std::istream& input);

private:
	IntputerIO Io;
};
