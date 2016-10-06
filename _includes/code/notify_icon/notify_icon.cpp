#include <memory>

struct Icon
{
	Icon(int, const char*) {}
};

struct NotifyIcon
{
	void setText(const char *) {}
	void setIcon(Icon *) {}
	void setIcon(std::unique_ptr<Icon>) {}
	void setVisible(bool) {}
};

int GetType()
{
	return 0;
}

namespace bad
{
#include "bad.cpp"
}

namespace good
{
#include "good.cpp"
}
