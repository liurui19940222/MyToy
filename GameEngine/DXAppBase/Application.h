#pragma once

class Application
{
public:
	explicit Application() = default;
	virtual ~Application() = default;

	virtual void init() = 0;
	virtual void run() = 0;
};

