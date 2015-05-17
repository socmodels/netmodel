struct MarriageInfo
{
	int Couple;
};

class MarriageAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	MarriageInfo * MarriageAspect::operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 std::vector <double> &values);
};
