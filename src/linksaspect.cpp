#include "framework.h"
#include "linksaspect.h"
#include "distributions.h"

//#include "snet.h"
#define DT 1.0F

int LinksAspect::GetInfoSize()
{
	return sizeof(LinksInfo);
}
void LinksAspect::InitializeAspect()
{
	// Evaluation buffers
	_lastPrecSize = -1;
	fifov = NULL;

	// Load parameters
	_seed = GetConfigLong("Seed"); Seed = &_seed;
	Friend_Probability = GetConfigFloat("Friend_Probability");
	Alfa = GetConfigFloat("Alfa");
	Homophilia = GetConfigFloat("Homophilia");
	Heterophilia = GetConfigFloat("Heterophilia");
	Random_Probability = GetConfigFloat("Random_Probability");
	Delete_Probability = GetConfigFloat("Delete_Probability");

	// Get aspects
	_distanceAspect = (DistanceAspect *) this->GetAspect(typeid(DistanceAspect).name());
	// Optional...
	_educationLevelAspect = (EducationLevelAspect *) this->TryGetAspect(typeid(EducationLevelAspect).name());
	_socialCircleAspect = (SocialCircleAspect *) this->TryGetAspect(typeid(SocialCircleAspect).name());

	// Initialize the link elements...
	Members = this->begin->GetSize();
	int qqq=0;
	LinksInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		qqq++;
		 element = (*this)[*agentId];
		 element->Degree = 0;
		 element->Predecesors = NULL;
		 list_init(&element->Table);
	}
}

void LinksAspect::Evolve()
{
  bTimeStepChanged = true;
  //former links_step(AdjLst *alst, int **social, int **geo, float *par, int *dim, long *Seed)
  ListElmt  *element;
  ListElmt  *prev;
  int n, m;



  /*All probabilities are probability rates. Hence we multiply these P by the coarse
  grain of time evolution DT*/
  LinksInfo *agentLinkInfo;
  LinksInfo *friendLinkInfo;
  LinksInfo *friendOfFriendLinkInfo;
  LinksInfo *randomLinkInfo;
  int friendId;
  int randomId;
  int friendOfFriendId;
  int deleteId;
  int agentId;

  int q = 0;

  for (Population::Iterator agentIditer = this->begin;
						agentIditer != this->end; ++agentIditer)
  {
	  q++;
	agentId = *agentIditer;
    /**********************************************************************************/
    /* Create contacts from friends of friends or random                              */
    /**********************************************************************************/
    agentLinkInfo = (*this)[agentId];

if (agentLinkInfo->Table.size != agentLinkInfo->Degree)
	throw ("Invalid data at agentLinkInfo");

	/* Explore one friend of friend with probability Friend_Probability * DT */
	if (agentLinkInfo->Degree > 0 && ran1(Seed) < Friend_Probability * DT)
	{
		/* Randomly chosen friend */
		m = (int)floor(agentLinkInfo->Degree * ran1(Seed));
		/* Gets the friend information */
		friendId = get_nth_table_item(&agentLinkInfo->Table, m);
		friendLinkInfo = (*this)[friendId];
		/* Get the friend info */
		if (friendLinkInfo->Degree > 1)
		{
		   friendOfFriendId = agentId;
		   while(friendOfFriendId == agentId)
		   {
			  n = (int)floor(friendLinkInfo->Degree * ran1(Seed));
			  friendOfFriendId = get_nth_table_item(&friendLinkInfo->Table, n);
		   }
		   friendOfFriendLinkInfo = (*this)[friendOfFriendId];
		   /*Note that even when we are excluding transitivity returns: l can be equal to n,
		   we are not excluding the possibility of transitivity returns to another freind of n.
		   There would be no problem with this, since insert biedge can deal with this*/
		   int tertiusId = friendOfFriendId;
		   if (must_create_new_link(agentId, tertiusId))
		   		insert_biedge(agentId, tertiusId, agentLinkInfo, friendOfFriendLinkInfo);
		}
	}
	/* Explore one contact at random with probability Random_Probability * DT */
	if (ran1(Seed) < Random_Probability * DT)
	{
	  randomId = agentId;
	  while (randomId == agentId)
	  {
		m = (int)floor(Members * ran1(Seed));
		Population::Iterator iter = this->begin;
		iter += m; // fetches to the selected guy
		randomId = *iter;
	  }
	  /* In this case we want a different m != m */
	  randomLinkInfo = (*this)[randomId];
	  if (must_create_new_link(agentId, randomId))
		insert_biedge(agentId, randomId, agentLinkInfo, randomLinkInfo);
	}
    /**********************************************************************************/
    /* Delete contacts with probability Delete_Probabilily * DT This runs for all links*/
    /**********************************************************************************/
	element = agentLinkInfo->Table.head;
	prev = NULL;
	while(element != NULL)
	{
	  if (Members * ran1(Seed) < Delete_Probability * DT)
	  {
		/* note the normalization! */
		deleteId = element->data->v2;
		/* Link is always removed. It is also possible to weight this probability using distance */
		delete_biedge(agentId, deleteId);

		if (prev != NULL)
			element = prev->next;
		else
			element = agentLinkInfo->Table.head;
	  }
	  else
	  {
		prev = element;
		element = element->next;
	  }
	} // end of the while
  }
}

void LinksAspect::insert_biedge(int i, int j)
{
	insert_biedge(i, j, (*this)[i], (*this)[j]);
}
void LinksAspect::insert_biedge(int i, int j, LinksInfo *iInfo,
								LinksInfo *jInfo)
{
   if (i == j) return; // cancel (target = source)

   Edge *edge1,*edge2;
   edge1 = (Edge *) malloc (sizeof (Edge));
   //edge1->v1 = i;
   edge1->v2 = j;
   edge1->weight = 1.0;
   adjlst_insert(iInfo, edge1);
   edge2 = (Edge *) malloc (sizeof (Edge));
   //edge2->v1 = j;
   edge2->v2 = i;
   edge2->weight = 1.0;
   adjlst_insert(jInfo, edge2);
}

void LinksAspect::adjlst_insert (LinksInfo *li, Edge *data)
{
  int j;
  ListElmt *element;
  // Checks the request
  //i = ((Edge *)data)->v1;
  j = ((Edge *)data)->v2;

  for (element = li->Table.head; element != NULL; element = element->next)
	if (j == element->data->v2) return; // cancel (target already exists)

  // Adds the link
  list_ins_next(&li->Table, li->Table.tail, data);

  Links++;
  li->Degree++;
}
void LinksAspect::delete_biedge(int i, int j)
{
  LinksInfo *iInfo = (*this)[i];
  LinksInfo *jInfo = (*this)[j];

  ListElmt *element, *prev;
  int retval=-2;

  if (i == j) return;
  prev = NULL;
  for (element = iInfo->Table.head; element != NULL;
		element = element->next)
  {
    if ( j == element->data->v2)
	{
	  list_rem_next(&iInfo->Table, prev);
	  Links--;
	  iInfo->Degree--;
	  break;
	}
    prev = element;
   }
   prev = NULL;
   for (element = jInfo->Table.head; element != NULL;
	   element = element->next)
   {
	 if ( i == element->data->v2)
	 {
	   list_rem_next (&jInfo->Table, prev);

	   Links--;
	   jInfo->Degree--;
	   break;
	 }
     prev = element;
   }
}

bool LinksAspect::must_create_new_link(int agentId1, int agentId2)
{
	float geographicDistance;
	int educationDistance;
	int circleDistance;
	geographicDistance = _distanceAspect->GeographicDistance(agentId1, agentId2);
	educationDistance = _distanceAspect->EducationDistance(agentId1, agentId2);
	circleDistance = _distanceAspect->CircleDistance(agentId1, agentId2);

	float p1,p2,proba;
    /*Same circle*/
    if (circleDistance == 0)
	{
		if (_educationLevelAspect != NULL)
			p1 = (float) Homophilia * _educationLevelAspect->pdistrHomophilia(educationDistance);
		else
			p1 = (float) Homophilia;
		p2 = (float) exp(-Alfa * log(geographicDistance + 1.0F));
     	proba = (float) ((p1+p2) / (Homophilia + 1.0));
		/*Second Normalization*/
		if (_socialCircleAspect != NULL)
			proba /= _socialCircleAspect->CircleSizeOfAgent(agentId1); /* The population of that circle*/
		if(ran1(Seed) < proba)
			return true;
   }
   /*Different circle == random*/
   else {
	    if (_educationLevelAspect != NULL)
			p1=(float)Heterophilia * _educationLevelAspect->pdistrHeterophilia(educationDistance);
		else
			p1=(float)Heterophilia;
		p2=(float) exp(-Alfa * log(geographicDistance + 1.0));
		proba = (float) ((p1+p2) / (Heterophilia + 1.0));
		/*Second Normalization*/
		proba /= (float)Members;
		if(ran1(Seed) < proba)
			return true;
   }
   return false;
}

void LinksAspect::ReleaseAspect()
{
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		 LinksInfo *element = (*this)[*agentId];
		 list_destroy(&element->Table);
		 if (element->Predecesors != NULL)
			delete(element->Predecesors);
	}
	// Buffers
	if (fifov != NULL)
		delete (fifov);
}

LinksInfo * LinksAspect::operator[](key agentId)
{
	return (LinksInfo *) this->GetInfo(agentId);
}

LinksAspect::~LinksAspect()
{

}
void LinksAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 std::vector <double> & values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "Degree")==0)
			values.push_back((*this)[agentId]->Degree);
		else if (strcmp(fields[n], "Clustering")==0)
			values.push_back(clustering((*this)[agentId]));
		else if (strcmp(fields[n], "MeanShortest")==0)
		{
			checkTimeStepChange();
			values.push_back((*this)[agentId]->mean_shortest);
		}
		else if (strcmp(fields[n], "Closeness")==0)
		{
			checkTimeStepChange();
			values.push_back((*this)[agentId]->closeness);
		}
		else if (strcmp(fields[n], "Betweenness")==0)
		{
			checkTimeStepChange();
			values.push_back((*this)[agentId]->betweenness);
		}
		else if (strcmp(fields[n], "SizeComponent")==0)
		{
			checkTimeStepChange();
			values.push_back((*this)[agentId]->size_component);
		}
		else if (strcmp(fields[n], "MaxDiameter")==0)
		{
			checkTimeStepChange();
			values.push_back((*this)[agentId]->max_diameter);
		}
		else
			values.push_back(0);
	}
}
void LinksAspect::checkTimeStepChange()
{
	if (bTimeStepChanged)
	{
		multi_eval();
		bTimeStepChanged = false;
	}
}
