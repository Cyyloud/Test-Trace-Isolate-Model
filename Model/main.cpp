#include "model.h"

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	clock_t startTime, endTime;
	startTime = clock();
	int i, j;
	char* char_seed = NULL;
	R_GLOBAL = gsl_rng_alloc(gsl_rng_taus2);
	gsl_rng_set(R_GLOBAL, time(NULL));

	char* dirName = argv[1];
	//strategy parameters
	double beta_scale = atof(argv[2]);      // scale factor of beta
	double beta_home = 0.58 * beta_scale;       // beta of household
	double beta_work = 0.27 * beta_scale;       // beta of company
	double beta_sch = 0.38 * beta_scale;        // beta of school
	double beta_community = 0.36 * beta_scale;  //  beta of community

	int symbol_NPI = atoi(argv[3]);
	m_infectious_period = atof(argv[4]);
	int sd = atoi(argv[5]);
	int enhanced = atoi(argv[6]);
	int temporal = atoi(argv[7]);
	int fixed = atoi(argv[8]);
	double p_test = atof(argv[9]);
	double Pid = atof(argv[10]);                 //probability of tracing identifiable contacts outside the household
	int mu_ST = atof(argv[11]);
	int mu_TR = atof(argv[12]);
	int time_to_trace = atof(argv[13]);

	double sum = 0;
	int num = 0;
	int ii = 0;

	// read Node information
	readNodeInfo();
	readWPlace();
	readSPlace();
	readHPlace();

    int loop_times = 1;
    for (int i = 1; i <= loop_times; i++)
    {
		if (fixed == 0)
		{
			string school_path = "../data/networks/school_connections.txt";
			parseFile(school_path, contact_schoolplace[i - 1]);
			cout<<"Successfully read contact_school" << i <<endl;
			string work_path = "../data/networks/work_connections.txt";
			parseFile(work_path, contact_workplace[i - 1]);
			cout<<"Successfully read contact_work" << i <<endl;
		}
		else if (fixed == 1)
		{
			string school_path = "../Contact_Part/networks_0/school_connections_2.txt";
			parseFile(school_path, contact_schoolplace[i - 1]);
			cout<<"Successfully read contact_school" << i <<endl;
			string work_path = "../Contact_Part/networks_0/work_connections_2.txt";
			parseFile(work_path, contact_workplace[i - 1]);
			cout<<"Successfully read contact_work" << i <<endl;
		}
		else if (fixed == 2)
		{
			string school_path = "../Contact_Part/networks_100/school_connections_2.txt";
			parseFile(school_path, contact_schoolplace[i - 1]);
			cout<<"Successfully read contact_school" << i <<endl;
			string work_path = "../Contact_Part/networks_100/work_connections_2.txt";
			parseFile(work_path, contact_workplace[i - 1]);
			cout<<"Successfully read contact_work" << i <<endl;
		}
        
    }
	while (ii < 10000)
	{
		cout << "start simulation: " << ii << endl;
		int cumulativeCase = 0;
		int infectTime = 0;
		if (ii % 200 == 0)
		{
			for(int j = 1; j < n ; j++)
			{
				nodeList[j].contact_by_time.clear();
				if (temporal == 0)
				{
					infectTime = rand() % 100;
					// choose one fixed contact network
					nodeList[j].contact_by_time.push_back(infectTime);
				}
			// temporal
				else if (temporal == 1)
				{
					int mm = 0;
					for(mm = 0; mm < 365; mm++)
					{
						if(mm < 100)
							nodeList[j].contact_by_time.push_back(mm);
						else
						{
							int rand_contact = rand() % 100;
							nodeList[j].contact_by_time.push_back(rand_contact);
						}
					}
				}
			}
		}
		cumulativeCase = SIRModel(ii, dirName, beta_home, beta_sch, beta_work, beta_community, symbol_NPI, sd, enhanced, temporal, fixed, p_test, Pid, mu_ST, mu_TR, time_to_trace);
		cout << "end simulation " << ii << endl;
		ii += 1;
	}
	endTime = clock();

	cout << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

	return 0;
}