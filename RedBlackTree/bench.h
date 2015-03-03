#pragma once

#include <vector>
#include <string>
#include <functional>

struct Bench;
typedef std::vector<const Bench *> BenchList;
class BenchManager {
public:
	static BenchManager* Get() {
		static BenchManager instance;
		return &instance;
	}

	void AddBench(const Bench* bench) {
		benches_.push_back(bench);
	}

	const std::vector<const Bench*>& benches() const {
		return benches_;
	}

private:
	std::vector<const Bench*> benches_;
};

struct BenchContext {
	std::function<void(int)> insert;
	std::function<void(int)> erase;
	std::function<bool(int)> search;
};

struct Bench {
	Bench(
		const char* fname,
		BenchContext* context)
		:
		name(fname),
		context(context)
	{
		BenchManager::Get()->AddBench(this);
	}
	~Bench() {
		delete context;
	}

	const char* name;
	BenchContext* context;
};


/*#define STRINGIFY(x) #x*/
#define REGISTER_BENCH(f, c) static Bench gRegister##c(f, new c)