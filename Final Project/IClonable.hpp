#pragma once
#include <memory>

struct IClonable
{
	virtual std::unique_ptr<IClonable> clone() const = 0;
	virtual ~IClonable() = default;
};