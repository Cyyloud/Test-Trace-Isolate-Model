#include "model.h"

//node current_state: S E A(infected with symptoms) I(infected without symptoms) R V
int SIRModel(int repTime, char* dirName,double beta_home, double beta_sch, double beta_work, double beta_community, double symbol_NPI, int sd, int enhanced, int temporal, int fixed, double p_test, double Pid, int mu_ST, int mu_TR, int time_to_trace)
{
	int i, j, k, age, temp;
	int h_index;
	long node_index, temp_node;
	long seedNode;
	int time_MAX = 400;

	char fileName[100];             //keep the name of the directory

	//keeps the index of new and old symptomatic nodes
	vector< long int > newXNodes;	//new symptomatic infectious nodes
	vector< long int > oldXNodes;	//old symptomatic infectious nodes 

	//keeps the number of nodes in these current_state
	long new_symptomaticCase;
	long current_symptomaticCase;

	long new_asymptomaticCase;
	long current_asymptomaticCase;

	//keeps the number of nodes in these current_state
	vector<long> newAsymSeq;			// I and A
	vector<long> newSymSeq;			//show symptoms 

	vector<long> currAsymSeq;			// I and A
	vector<long> currSymSeq;			//show symptoms

	int cum_symptomaticCase = 0;

	int num_infections_h = 0;		//number of infections in household
	int num_infections_c = 0;		//number of infections in community
	int num_infections_w = 0;		//number of infections in workplace
	int num_infections_s = 0;		//number of infections in school

	int isolated_days_test=0;
	int isolated_days = 0;

	vector<long> hSetting;	//household
	vector<long> sSetting;	//school
	vector<long> wSetting;	//workplace
	vector<long> cSetting;	//comumunity

	vector<long> hSetting_t;	//household
	vector<long> sSetting_t;	//school
	vector<long> wSetting_t;	//workplace
	vector<long> cSetting_t;	//comumunity

	vector<long> curr_postive;	//positive nodes

	int cum_positive = 0;		//cumulative number of positive individuals

	vector<long> oldQuarantinedNodeSeq;
	vector<long> newQuarantinedNodeSeq;

	vector<long> isolatedNumber;	// daily new isolation numbers
	vector<long> isolatePerson;		// simutaneously isolated numbers
	vector<long> isolatedTest;

	vector<long> susAgents;
	vector<long> posAgents;
	vector<long> infAgents;
	vector<long> isoAgents;

	vector<long> tracingNumber;

	int infectTime = 0;

	// initlization
	for (i = 0; i < 1; i++)				//repeats times
	{
		for (j = 1; j < n; j++)
		{
			nodeList[j].current_state = 'S';
			nodeList[j].infect_time = 10000;				//when it is infected
			nodeList[j].incubation_time = 10000;			//duartion
			nodeList[j].infectious_time = 0;			//duartion
			nodeList[j].T_ST = 10000;
			nodeList[j].T_TR = 10000;
			nodeList[j].testResult = 0;     // 0:not tested, -1: negative, 1: positive
			nodeList[j].contact = '0';        // '0' not traced, '1' contacted

			nodeList[j].quarantine_state = '0';	// '0' not quarantined, '1' quarantined at home, '2' quarantined at special place
			nodeList[j].quarantine_time = 10000;
			nodeList[j].isTest = 0;				// if a symptomatic individual is willing to get RT-PCR test
			nodeList[j].sus = getSus(nodeList[j].age);
			nodeList[j].infector = -1;
		}

		//node indexes
		vector <long int>().swap(oldInfectNodes);
		vector <long int>().swap(newInfectNodes);

		vector <long int>().swap(oldXNodes);
		vector <long int>().swap(newXNodes);

		vector <long>().swap(old_toBeCheckedNodeList);      //check the test result
		vector <long>().swap(new_toBeCheckedNodeList);

		vector<int>().swap(temp_TG);

		vector<long>().swap(oldQuarantinedNodeSeq);
		vector<long>().swap(newQuarantinedNodeSeq);


		//initlization
		int timeStep = 0;
		for (j = 0; j < num_seeds; j++)
		{
			seedNode = rand() % n;
			age = nodeList[seedNode].age;
			while (nodeList[seedNode].current_state != 'S')
			{
				seedNode = rand() % n;
				age = nodeList[seedNode].age;
			}
			oldInfectNodes.push_back(seedNode);
			//update newly infected node's information
			updateNodeInfectionInfo(seedNode, timeStep, mu_ST, mu_TR);
		}

		int cum_infections;
		cum_infections = 0;

		//number of infections in each setting by time step
		int t_num_infections_h;
		int t_num_infections_s;
		int t_num_infections_w;
		int t_num_infections_c;

		int t_infections_h;
		int t_infections_s;
		int t_infections_w;
		int t_infections_c;

		int contact_is = 0;
		int contact_iw = 0;

		// enhanced social distancing: randomly close part of companies

		vector <int> closed_companies;
		if(enhanced == 1)
		{
			for (j = 0; j < n_w_group; j++)
			{
				closed_companies.push_back(0);
			}
			// closed p percent company
			int closed_company = n_w_group * 0.1;
			for(j = 0; j < closed_company; j++)
			{
				int company_index = rand() % n_w_group;
				closed_companies[company_index] = 1;
			}
		}
		else if(enhanced == 2)
		{
			for (j = 0; j < n_w_group; j++)
			{
				closed_companies.push_back(0);
			}
			// closed p percent company
			int closed_company = n_w_group * 0.3;
			for(j = 0; j < closed_company; j++)
			{
				int company_index = rand() % n_w_group;
				closed_companies[company_index] = 1;
			}
		}
		else if(enhanced == 3)
		{
			for (j = 0; j < n_w_group; j++)
			{
				closed_companies.push_back(0);
			}
			// closed p percent company
			int closed_company = n_w_group * 0.5;
			for(j = 0; j < closed_company; j++)
			{
				int company_index = rand() % n_w_group;
				closed_companies[company_index] = 1;
			}
		}				
		//loop
		while (oldInfectNodes.size() > 0 && timeStep < 365 && cum_infections < 5000000)            // start the simulation
		{
			new_symptomaticCase = 0;
			current_symptomaticCase = 0;

			new_asymptomaticCase = 0;
			current_asymptomaticCase = 0;

			t_infections_h = 0;
			t_infections_s = 0;
			t_infections_w = 0;
			t_num_infections_c = 0;

			int isolated_number = 0;
			int isolated_number_test = 0;

			int pcr_valid = n;

			int traced_number = 0;
			int susNumber = 0;
			int infNumber = 0;
			int isoNumber = 0;

			if(symbol_NPI >= 1 and symbol_NPI <= 7)
			{
				for(j = 1; j < n; j++)
				{
					
					if(nodeList[j].isTest == 1 and timeStep > nodeList[j].testTime)
					{
						isolated_days_test += 1;
						isolated_number_test += 1;
					}
					if(nodeList[j].current_state == 'S' and nodeList[j].quarantine_state == '0' and (nodeList[j].isTest == 0 or (nodeList[j].isTest == 1 and timeStep <= nodeList[j].testTime)))
					{
						susNumber++;
					}
					if(nodeList[j].current_state != 'S' and nodeList[j].current_state != 'R' )
					{
						infNumber ++;
						if (nodeList[j].quarantine_state != '0' or (nodeList[j].isTest == 1 and timeStep > nodeList[j].testTime))
							isoNumber ++;
					}
				}
			}

			else if (symbol_NPI == 0)
			{
				for(j = 1; j < n; j++)
				{
					if(nodeList[j].current_state == 'S')
					{
						susNumber++;
					}
					if(nodeList[j].current_state != 'S' and nodeList[j].current_state != 'R')
					{
						infNumber ++;
					}
				}
			}

			susAgents.push_back(susNumber);
			infAgents.push_back(infNumber);
			isoAgents.push_back(isoNumber);

			//transmission process
			for (j = 0; j < oldInfectNodes.size(); j++)
			{
				long nodeIndex = oldInfectNodes[j];
				int hIndex = nodeList[nodeIndex].index_h;
				int index_s = nodeList[nodeIndex].index_sg;
				int index_w = nodeList[nodeIndex].index_wg;

				int startInfectime = nodeList[nodeIndex].infect_time + nodeList[nodeIndex].latent_time;
				int testTime = nodeList[nodeIndex].testTime;

				if (timeStep <= startInfectime)
					continue;

				t_num_infections_h = 0;
				t_num_infections_s = 0;
				t_num_infections_w = 0;
			
				//here these conditions only consider this infector can have or not have the contacts in these settings
				if (symbol_NPI == 0)  //no npi
				{
					// household infection
					if (hMembers[hIndex].size() > 1)
					{
						t_num_infections_h = node_infection_Household(timeStep, nodeIndex, hMembers[hIndex], beta_home, seedNode, mu_ST, mu_TR);
						t_infections_h += t_num_infections_h;
					}
					// school infection
					if (index_s > -1)
					{
						if (schoolGroups[index_s].size() > 1)
						{
							vector<long> member;
							member = getConnectors(nodeIndex, contact_is, 2, nodeList[nodeIndex].contact_by_time[timeStep], temporal);
							int temp = node_infection_setting(timeStep, nodeIndex, member, beta_sch, seedNode, symbol_NPI, mu_ST, mu_TR);
							t_num_infections_s += temp;
							t_infections_s += temp;
						}
					}
					// workplace infection
					if (index_w > -1)
					{
						if (workplaceGroups[index_w].size() > 1)
						{
							vector<long> member;
							member = getConnectors(nodeIndex, contact_iw, 1, nodeList[nodeIndex].contact_by_time[timeStep], temporal);
							int temp = node_infection_setting(timeStep, nodeIndex, member, beta_work, seedNode, symbol_NPI, mu_ST, mu_TR);
							t_num_infections_w += temp;
							t_infections_w += temp;
						}
					}
				}

				else if (symbol_NPI >= 1 and symbol_NPI <= 7)	// TTI implement
				{
					// not live alone and not in isolation at special place
					if (hMembers[hIndex].size() > 1 and nodeList[nodeIndex].quarantine_state != '2')
					{
						t_num_infections_h = node_infection_Household(timeStep, nodeIndex, hMembers[hIndex], beta_home, seedNode, mu_ST, mu_TR);
						t_infections_h += t_num_infections_h;
					}
					// infectious individual not in isolation, school infection process
					if (nodeList[nodeIndex].quarantine_state == '0')
					{
						// the agent didnt do the test then contact, and the school is real
						if (index_s > -1 and (nodeList[nodeIndex].isTest == 0 or (nodeList[nodeIndex].isTest == 1 and timeStep <= testTime)))
						{
							if (schoolGroups[index_s].size() > 1)
							{
								vector<long> member;
								member = getConnectors(nodeIndex, contact_is, 2, nodeList[nodeIndex].contact_by_time[timeStep], temporal);
								// no enhanced social distancing
								if (enhanced == 0)
								{
									int temp = node_infection_setting(timeStep, nodeIndex, member, beta_sch, seedNode, symbol_NPI, mu_ST, mu_TR);
									t_num_infections_s += temp;
									t_infections_s += temp;
								}
								// close part of schools
								else if (enhanced != 0)
								{
									if (schoolGroups[index_s].size() > 1 and nodeList[nodeIndex].education == 4)
									{
										int temp = node_infection_setting(timeStep, nodeIndex, member, beta_sch, seedNode, symbol_NPI, mu_ST, mu_TR);
										t_num_infections_s += temp;
										t_infections_s += temp;
									}
								}
							}
						}
						if (index_w > -1 and (nodeList[nodeIndex].isTest == 0 or (nodeList[nodeIndex].isTest == 1 and timeStep <= testTime)))
						{
							if (workplaceGroups[index_w].size() > 1)
							{	
								vector<long> member;
								member = getConnectors(nodeIndex, contact_iw, 1, nodeList[nodeIndex].contact_by_time[timeStep], temporal);
								// no workplace limitation
								if (enhanced == 0)
								{
									int temp = node_infection_setting(timeStep, nodeIndex, member, beta_work, seedNode, symbol_NPI, mu_ST, mu_TR);
									t_num_infections_w += temp;
									t_infections_w += temp;
								}
								// random companies close
								else 
								{
									// closed_companies[index_w] == 1 means that the company w is closed
									if (closed_companies[index_w] == 0)
									{
										int temp = node_infection_setting(timeStep, nodeIndex, member, beta_work, seedNode, symbol_NPI, mu_ST, mu_TR);
										t_num_infections_w += temp;
										t_infections_w += temp;
									}
								}
							}
								
						}
					}
				}
				else
				{
					cout << "symbol_NPI error: " << symbol_NPI << endl;
				}
				//record the transmission in household and other settings
				num_infections_h += t_num_infections_h;
				num_infections_w += t_num_infections_w;
				num_infections_s += t_num_infections_s;
			}


			//transmission in the general community
			
			t_num_infections_c = node_infection_community(timeStep, community[0], beta_community, seedNode, sd, mu_ST, mu_TR);

			//record the transmission in the general community
			num_infections_c += t_num_infections_c;

			cum_infections = num_infections_h + num_infections_s + num_infections_c + num_infections_w;

			new_asymptomaticCase = newInfectNodes.size();  //record the new_I and new_A

			hSetting_t.push_back(t_infections_h);
			sSetting_t.push_back(t_infections_s);
			wSetting_t.push_back(t_infections_w);
			cSetting_t.push_back(t_num_infections_c);

			// examined 7.24 11.50

			// developing symptom and recovery
			for (k = 0; k < oldInfectNodes.size(); k++)		//I A X nodes in oldInfectNodes
			{
				temp_node = oldInfectNodes[k];

				int t0 = nodeList[temp_node].infect_time;
				int latent_time = nodeList[temp_node].latent_time;
				int incubation_time = nodeList[temp_node].incubation_time;
				int infectious_time = nodeList[temp_node].infectious_time;

				int time_toX = t0 + incubation_time;
				int time_toR = t0 + incubation_time + infectious_time;
				char node_state = nodeList[temp_node].current_state;

				//  at this time,PCR test then have no contact in community at next timestep
				if (node_state == 'I')
				{
					newInfectNodes.push_back(temp_node);
					if (timeStep >= time_toX)
					{
						nodeList[temp_node].current_state = 'X';
						newXNodes.push_back(temp_node);
						new_symptomaticCase += 1;
						cum_symptomaticCase += 1;

						//test or not, test result, check test on time
						if (rand() / (double)RAND_MAX < p_test and pcr_valid > 0 and nodeList[temp_node].isTest != 1)
						{
							collectSample_test(timeStep, temp_node);
							pcr_valid --;
						}
					}
					else
					{
						current_asymptomaticCase += 1;
					}

				}
				else if (node_state == 'A')
				{
					if (timeStep < time_toR)	//still in infectious period
					{
						newInfectNodes.push_back(temp_node);
						current_asymptomaticCase += 1;
					}
					else
						nodeList[temp_node].current_state = 'R';		//node in A current_state can directly become R
				}
				else if (node_state == 'X')
				{
					if (timeStep < time_toR)	//still in infectious period
					{
						newInfectNodes.push_back(temp_node);
						oldXNodes.push_back(temp_node);		//record the old X nodes
						current_symptomaticCase += 1;
					}
					else
						nodeList[temp_node].current_state = 'R';		//node in A current_state can directly become R
				}
				else
					cout << "node type error in oldInfectNodes " << node_state << "\t" << nodeList[temp_node].infect_time << endl;
			}
			//current includeing the new
			current_asymptomaticCase += new_asymptomaticCase;
			current_symptomaticCase += new_symptomaticCase;

			int postivenodes = 0;

			//test and contact trace
			if(symbol_NPI >= 1)
			{
				vector<long> tracingSeq;
				for (j = 0; j < old_toBeCheckedNodeList.size(); j++)
				{
					int temp_node = old_toBeCheckedNodeList[j];

					int T_ST = nodeList[temp_node].T_ST;
					int T_TR = nodeList[temp_node].T_TR;
					int testTime = nodeList[temp_node].testTime;
					int time_checkResult = testTime + T_TR;

					if (timeStep >= time_checkResult)	// is time to check result and only check once
					{
						if (nodeList[temp_node].testResult == 1)
						{
							cum_positive += 1;
							postivenodes ++;
							// store the new positive nodes at this timeStep
							tracingSeq.push_back(temp_node);
							nodeList[temp_node].isTest = 0;
						}
						else if(nodeList[temp_node].testResult == -1)
						{
							//Cancel contact restrictions due to testing
							nodeList[temp_node].isTest = 0;
							nodeList[temp_node].contact = '0';
						}
						else
							cout<<"Wrong check:"<<temp_node<<endl;
					}
					else
						new_toBeCheckedNodeList.push_back(temp_node);
				}
				posAgents.push_back(postivenodes);
				int m = 0;

				for (m = 0; m < tracingSeq.size(); m++)
				{
					long temp_node = tracingSeq[m];
					// positive and not in isolation
					if(nodeList[temp_node].quarantine_state == '0' and nodeList[temp_node].testResult == 1)
					{
						// positive cases isolated in home
						if(symbol_NPI == 1 or symbol_NPI == 2 or symbol_NPI == 3 or symbol_NPI == 5 or symbol_NPI == 6)
							nodeList[temp_node].quarantine_state = '1';
						// positive cases isolated at special place
						else if (symbol_NPI == 4 or symbol_NPI == 7)
							nodeList[temp_node].quarantine_state = '2';
						else
							cout<<"Wrong symbol_NPI isolating"<<endl;
						nodeList[temp_node].quarantine_time = timeStep;
						newQuarantinedNodeSeq.push_back(temp_node);
					}
					vector<long> contactSet;
					// direct contact tracing(L2-L4)
					if(symbol_NPI >= 2 and symbol_NPI <=4)
					{
						int testTime = nodeList[temp_node].testTime;
						contactSet = contact_tracing(temp_node, Pid, symbol_NPI, time_to_trace, testTime);	//changed in 8.22
						traced_number += contactSet.size();
					}
					// secondary contact tracing
					else if (symbol_NPI >= 5 and symbol_NPI <=7)
					{
						contactSet = contact_of_contactTracing(temp_node, Pid, symbol_NPI, time_to_trace, timeStep);
						sort(contactSet.begin(), contactSet.end());
    					auto it = unique(contactSet.begin(), contactSet.end());
    					contactSet.erase(it, contactSet.end());
						traced_number += contactSet.size();
					}
					else if (symbol_NPI != 1)
						cout<<"Wrong symbol_NPI contact tracing"<<endl;
					
					int ii = 0;
					//quarantine the contacts
					for (ii = 0; ii < contactSet.size(); ii++)
					{
						int contact_index = contactSet[ii];
						// directly isolated at home for 14 days
						if (symbol_NPI == 3 or symbol_NPI == 6)
						{
							if (nodeList[contact_index].quarantine_state == '0')
							{
								newQuarantinedNodeSeq.push_back(contact_index);
								nodeList[contact_index].quarantine_state = '1';
							}
							nodeList[contact_index].quarantine_time = timeStep;		//free at time+14
						}
						// directly isolated at special place for 14 days
						else if (symbol_NPI == 4 or symbol_NPI == 7)
						{
							if (nodeList[contact_index].quarantine_state == '0')
							{
								newQuarantinedNodeSeq.push_back(contact_index);
								nodeList[contact_index].quarantine_state = '2';
							}
							nodeList[contact_index].quarantine_time = timeStep;		//free at time+14
						}
						// test contacts, if result is negative, then release, if positive treat it as positive case
						else if (symbol_NPI == 2 or symbol_NPI == 5)
						{
							if(pcr_valid > 0 and nodeList[contact_index].isTest != 1)
							{
								collectSample_test_contact(timeStep, contact_index, mu_TR);
								pcr_valid -- ;
							}
						}
					}
				}
				// numbers in isolation and the isolation due to test
				isolatedNumber.push_back(newQuarantinedNodeSeq.size() + isolated_number_test);
				isolatePerson.push_back(oldQuarantinedNodeSeq.size() + isolated_number_test);
				isolatedTest.push_back(isolated_number_test);
				tracingNumber.push_back(traced_number + tracingSeq.size());
				isolated_days += oldQuarantinedNodeSeq.size();

				//update the quarantine current_state
				for (j = 0; j < oldQuarantinedNodeSeq.size(); j++)
				{
					int node = oldQuarantinedNodeSeq[j];
					if (timeStep == nodeList[node].quarantine_time + 14)
					{
						nodeList[node].quarantine_state = '0';
						nodeList[node].quarantine_time = 10000;
						nodeList[node].contact = '0';
						nodeList[node].isTest = 0;
					}
					else
						newQuarantinedNodeSeq.push_back(node);
				}
			}
			hSetting.push_back(num_infections_h);
			cSetting.push_back(num_infections_c);
			sSetting.push_back(num_infections_s);
			wSetting.push_back(num_infections_w);

			newSymSeq.push_back(new_symptomaticCase);
			newAsymSeq.push_back(new_asymptomaticCase);

			currSymSeq.push_back(current_symptomaticCase);
			currAsymSeq.push_back(current_asymptomaticCase);

			//update the vectors
			oldInfectNodes.swap(newInfectNodes);
			oldXNodes.swap(newXNodes);
			old_toBeCheckedNodeList.swap(new_toBeCheckedNodeList);
			oldQuarantinedNodeSeq.swap(newQuarantinedNodeSeq);

			vector<long>().swap(newInfectNodes);
			vector<long>().swap(newXNodes);
			vector<long>().swap(newQuarantinedNodeSeq);
			vector<long>().swap(new_toBeCheckedNodeList);

			timeStep = timeStep + 1;
		}

		FILE* fTime_newCurr_sym;
		sprintf(fileName, "%s%s", dirName, "sim_t_new_currSym.txt");
		fTime_newCurr_sym = fopen(fileName, "a");
		for (j = 0; j < newSymSeq.size(); j++)
		{
			char buffer[100];
			snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\n", repTime, j, newSymSeq[j], currSymSeq[j]);
			fputs(buffer, fTime_newCurr_sym);
		}
		fclose(fTime_newCurr_sym);

		FILE* fTime_newCurr_Asym;
		sprintf(fileName, "%s%s", dirName, "sim_t_new_currAsym.txt");
		fTime_newCurr_Asym = fopen(fileName, "a");
		for (j = 0; j < newAsymSeq.size(); j++)
		{
			char buffer[100];
			snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\n", repTime, j, newAsymSeq[j], currAsymSeq[j]);
			fputs(buffer, fTime_newCurr_Asym);
		}
		fclose(fTime_newCurr_Asym);


		FILE* fTime_I_A_X;
		sprintf(fileName, "%s%s", dirName, "sim_t_sym_asym.txt");
		fTime_I_A_X = fopen(fileName, "a");
		for (j = 0; j < newSymSeq.size(); j++)
		{
			char buffer[100];
			snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\n", repTime, j, newSymSeq[j], newAsymSeq[j]);
			fputs(buffer, fTime_I_A_X);
		}
		fclose(fTime_I_A_X);

		if (cum_infections > 10000)
		{
			FILE* fp;
			sprintf(fileName, "%s%s", dirName, "sim_t_h_s_w_c_other.txt");
			fp = fopen(fileName, "a");
			for (i = 0; i < hSetting.size(); i++)
			{

				double total;
				total = hSetting[i] + cSetting[i] + sSetting[i] + wSetting[i];
				char buffer[100];
				snprintf(buffer, sizeof(buffer), "%d\t%d\t%f\t%f\t%f\t%f\n", repTime, i, hSetting[i] / total, sSetting[i] / total, wSetting[i] / total, cSetting[i] / total);
				fputs(buffer, fp);
			}
			fclose(fp);
		}

		if(cum_infections > 10000)
		{
			FILE* fp2;
			sprintf(fileName, "%s%s", dirName, "sim_p_h_s_w_c.txt");
			fp2 = fopen(fileName, "a");
			for (i = 0; i < hSetting.size(); i++)
			{
				char buffer[100];
				snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\t%d\t%d\n", repTime, i,  hSetting_t[i], sSetting_t[i], wSetting_t[i], cSetting_t[i]);
				fputs(buffer, fp2);
			}
			fclose(fp2);
		}

		FILE* fp1;
		sprintf(fileName, "%s%s", dirName, "sim_t_TG.txt");
		fp1 = fopen(fileName, "a");
		for (i = 0; i < temp_TG.size(); i++)
		{
			char buffer[100];
			snprintf(buffer, sizeof(buffer), "%d\t%d\n", repTime, temp_TG[i]);
			fputs(buffer, fp1);
		}
		fclose(fp1);

		// the number of isolated persons at every timestep
		FILE* fp3;
		sprintf(fileName, "%s%s", dirName, "sim_t_iso.txt");
		fp3 = fopen(fileName, "a");
		for (i = 0; i < isolatedNumber.size(); i++)
		{
			char buffer[100];
			snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", repTime, i, isolatedNumber[i], isolatePerson[i], isolated_days, isolated_days_test, tracingNumber[i], isolatedTest[i]);
			fputs(buffer, fp3);
		}
		fclose(fp3);

		if(symbol_NPI >= 1)
		{
			FILE* fp4;
			sprintf(fileName, "%s%s", dirName, "sim_t_sus_inf.txt");
			fp4 = fopen(fileName, "a");
			for (i = 0; i < susAgents.size(); i++)
			{
				char buffer[100];
				snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\t%d\n", repTime, i, susAgents[i], infAgents[i], isoAgents[i]);
				fputs(buffer, fp4);
			}
			fclose(fp4);
		}
		if(symbol_NPI == 0)
		{
			FILE* fp4;
			sprintf(fileName, "%s%s", dirName, "sim_t_sus_inf.txt");
			fp4 = fopen(fileName, "a");
			for (i = 0; i < susAgents.size(); i++)
			{
				char buffer[100];
				snprintf(buffer, sizeof(buffer), "%d\t%d\t%d\t%d\n", repTime, i, susAgents[i], infAgents[i]);
				fputs(buffer, fp4);
			}
			fclose(fp4);
		}
		


		cout << cum_infections << endl;
	}
	return cum_symptomaticCase;
}
