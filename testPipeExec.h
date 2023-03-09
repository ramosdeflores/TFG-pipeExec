class adder : public PipeBase {
	bool run(void* data);
	adder * clone() const;
};

class subs : public PipeBase {
	bool run(void* data);
	subs * clone() const;
};

class printer : public PipeBase {
	bool run(void* data);
	printer * clone() const;
};
