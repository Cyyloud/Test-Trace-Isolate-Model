#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <random>
#include <iterator>
#include <algorithm>
#include <math.h>
#include<random>
#include <string.h>
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include<stdbool.h>
#include<cctype>

using namespace std;
extern gsl_rng* R_GLOBAL;

#define n 499306 
#define n_household 159653 
#define n_s_group 132				//preprocess the members in each school
#define n_w_group 885
#define FALSE -1
#define TRUE 0

extern int DAYS_RUNNING;
extern int num_seeds;			//number of seeds

struct nodeInfo {
	int index;
	int age;
	long infector;
	int education = 0;
	int isTeacher = 0;

	//network info
	int index_h;						//index of household
	int index_sg = -2;						//index of group for school
	int index_wg = -2; 						//index of group for workplace

	//current statu info
	char current_state;	//'S', 'I'(will become symptomatic), 'A' (without symptomatic), 'X' (show symptom), 'R'

	int infect_time;		//when this individual is infected;
	int latent_time;		//duration, sampled from an exponetial distribution, after this period it becomes infectious
	int incubation_time;	//duration, depends on p, after this period it shows symptom
	int infectious_time;		//duration, sampled from a exponetial distribution 

	//test info
	int T_ST;		// time from show symptom to test
	int T_TR;		// time from test to get result
	int testResult;		//0,1,-1
	int isTest;		// have tested 1 not 0;
	int testTime;  // the time of get RT-PCR Test

	char quarantine_state;	//'0': not quarantine, '1':quarantined (just isolated at home for 14 days), '2': isolated in special place
	int quarantine_time;	//when he is quarantined

	//heterogneouness of transmission, negative binom distribution
	double sus = 1;		//susceptibility by age

	char contact;	//"1" contacted,0 not

	vector<long> connectors;	// store the infectee which infected by this node
	vector<int> contact_by_time;
};
extern vector <nodeInfo> nodeList;

//mean latent time, infectious period follows exponetial distribution
extern double m_presym_period;		// no symptom, have infectiveness
extern double m_symptom_period;		// show symptom, have infectiveness
extern double m_infectious_period;

//incubation
extern double shape_incubation;	//shape for gamma distribution
extern double scale_incubation;	//scale for gamma distribution

extern double k1;		// infectiveness with asym
extern double k2;		// infectiveness with symptom 

// no symptom percentage
extern double p_sym;

//(parameters to be calibrated) transmission parameters
extern double beta_scale;
extern double beta_home;		//household
extern double beta_sch;			//school
extern double beta_work;		//workplace
extern double beta_community;		//communit

//variables in modeling
extern vector <long int> oldInfectNodes;
extern vector <long int> newInfectNodes;

extern vector<long> old_toBeCheckedNodeList;
extern vector<long> new_toBeCheckedNodeList;

extern vector<int>temp_TG;


// vector store the data
extern vector<long int> hMembers[n_household];
extern vector<long int> schoolGroups[n_s_group];
extern vector<long int> workplaceGroups[n_w_group];
extern vector<long> community[1];

extern vector<vector<vector<vector<long int>>>> contact_schoolplace;
extern vector<vector<vector<vector<long int>>>> contact_workplace;

// functions 
void readNodeInfo();    //read node information
void readHPlace();	//read household information
void readSPlace();	//read school information
void readWPlace();	//read work_place information

//infected node in household, groups in school or workplace or community

int sampleGammaDis(double shape, double scale);

void updateNodeInfectionInfo(int nodeIndex, int timeStep, int mu_ST, int mu_TR);

int sampleExponetialDis(double mu);

int SIRModel(int repTime, char* dirName,double beta_home, double beta_sch, double beta_work, double beta_community, double symbol_NPI, int sd, int enhanced, int temporal, int fixed, double p_test, double Pid, int mu_ST, int mu_TR, int time_to_trace);

vector<long> contact_tracing(long node_index, double Pid, int NPI, int time_to_trace, int timeStep);		//returns the contacts of node_index

int node_infection_setting(int t, long index_case, vector<long>member, double beta, long seed_node, int symbol_NPI, int mu_ST, int mu_TR);

int node_infection_community(int t, vector<long> member, double beta, long seed_node, int NPI, int mu_ST, int mu_TR);

void collectSample_test(int t, long nodeIndex);

void collectSample_test_contact(int t, long nodeIndex, int mu_TR);

vector<long> contact_of_contactTracing(long node_index, double Pid, int NPI, int time_to_trace, int timeStep);

int node_infection_Household(int t, long index_case, vector<long> member, double beta, long seed_node, int mu_ST, int mu_TR);

bool isIntegerInVector(const std::vector<long>& vec, long num);

vector<long int> getConnectors(long index_node, int index, int mode, int time, int temporal);

bool resultTrue(int days);

bool symTrue(int age);

int extractInteger(const std::string& str);

vector<int> readIntegersFromFile(const std::string& filename);

int getRandomSample(const std::vector<int>& data);

double getSus(int age);

void parseFile(const string& filePath, vector<vector<vector<long int>>>& data);