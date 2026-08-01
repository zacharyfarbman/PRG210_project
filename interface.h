#pragma once

class Interface {
protected:
	int getValidatedChoice(int min_val, int max_val);

public:
	virtual void run() = 0;
	virtual ~Interface();
};

class AdminInterface : public Interface {
public:
	void run() override;
};