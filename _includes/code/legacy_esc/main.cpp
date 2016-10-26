#include <memory>

enum codes
{
	ok,
	invalid_config,
	unknown_error
};

struct Config
{
};

struct Object
{
	int init_from_config(Config) { return ok; }
};

struct invalid_config_error {};

struct other_error { int code() const { return unknown_error; } };

bool config_is_valid(Config) { return true; }

namespace legacy {
#include "legacy.cpp"
}

namespace esc {
#include "esc.cpp"
}

int main() {}
