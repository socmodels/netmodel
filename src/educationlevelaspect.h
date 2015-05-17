#ifndef EDUCASP
  #define EDUCASP

struct EducationLevelInfo {
	int EducationLevel;
	};

class EducationLevelAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	int GetInfoSize();
	EducationLevelInfo * EducationLevelAspect::operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 std::vector <double> &values);
	~EducationLevelAspect();
	float pdistrHomophilia(int gap);
	float pdistrHeterophilia(int gap);

private:
	long *Seed;
	long _seed;
	int _levels;
	float _toleranceHeterophilia;
	float _toleranceHomophilia;
};

#endif


