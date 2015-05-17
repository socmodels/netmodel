#ifndef SOCCIRASP
  #define SOCCIRASP

struct SocialCircleInfo {
	int SocialCircle;
	};

class SocialCircleAspect : public AspectBase
{
public:
	void Evolve();
	void InitializeAspect();
	void ReleaseAspect();
	int GetInfoSize();
	SocialCircleInfo * SocialCircleAspect::operator[](key agentId);
	void virtual ShowValues(int agentId, std::vector <char *> &fields,
								 std::vector <double> &values);
	~SocialCircleAspect();
	int CircleSizeOfAgent(int agentId);
private:
	long *	Seed;
	long	_seed;
	int *	_circleSizes;

};

#endif
