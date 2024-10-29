#include "model.h"

gsl_rng* R_GLOBAL;

// node infomation
vector<nodeInfo> nodeList;
vector<long int> hMembers[n_household];
vector<long int> schoolGroups[n_s_group];
vector<long int> workplaceGroups[n_w_group];
vector<long int> community[1];

// model variables
vector<long int> oldInfectNodes;
vector<long int> newInfectNodes;

// model parameters
int DAYS_RUNNING;
int num_seeds = 1;

// incubation gamma distribution
double shape_incubation = 2.08;
double scale_incubation = 0.33;

// asym's and sym's infectiousness
double k1 = 0.5;    // asym
double k2 = 1;      // sym

// time parameters
double preSymptom_period = 2.0; //fix to 2 , latent = incubation - preSym
double m_infectious_period;

// PCR-RT Test result vector
vector<long> old_toBeCheckedNodeList;
vector<long> new_toBeCheckedNodeList;
vector<long> wait_toBeTestNodeList;

// TG and SI
vector<int> temp_TG;
vector<int> temp_SI;

// show symptom probability
double p_sym = 0.5;

// social distance 
double socialDistance_s = 0.5;
double socialDistance_w = 0.5;
double socialDistance_c = 0.5;

// average community contacts
double mean_com = 5.2;

// contact
vector<vector<vector<vector<long int>>>> contact_schoolplace(1, vector<vector<vector<long int>>>(499306, vector<vector<long int>>(100)));
vector<vector<vector<vector<long int>>>> contact_workplace(1, vector<vector<vector<long int>>>(499306, vector<vector<long int>>(100)));


// functions
int sampleGammaDis(double shape, double scale)
{
    double temp = gsl_ran_gamma(R_GLOBAL, shape, scale);
    int number = round(temp);   // incubation time is an integer
    return number;
}

int sampleExponetialDis(double mu)
{
    double temp = gsl_ran_exponential(R_GLOBAL, mu);
    float p;
    int number = (int)temp;
    p = temp - number;
    if (rand() / (double)RAND_MAX < p)
        number = number + 1;
    return number;
}

void readNodeInfo()
{
    long int i = 0;
    int j = 0;
    ifstream inFile("../data/Population.csv");
    string lineStr;
    vector<vector<string>> strArray;
    getline(inFile, lineStr);
    while (j < n)
    {
        nodeList.push_back(nodeInfo());
        if (j != n-1)
            community[0].push_back(j+1);
        j++;
    }

    while (getline(inFile, lineStr))
    {
        stringstream ss(lineStr);
        string str;
        vector<string> lineArray;
        while (getline(ss, str, ','))
        {
            lineArray.push_back(str);
        }
        i = stoi(lineArray[1]);
        nodeList[i].index = stoi(lineArray[1]);	//start from 1
        nodeList[i].age = stoi(lineArray[3]);
        nodeList[i].index_h = stoi(lineArray[0]);
        nodeList[i].index_sg = stoi(lineArray[5]);
        nodeList[i].index_wg = stoi(lineArray[6]);
        nodeList[i].education = stoi(lineArray[4]);
        if (lineArray[11] == "1")
            nodeList[i].isTeacher = 1;
    }
    cout << "done Node" << endl;
}

void readHPlace()
{
    ifstream inFile("../data/Population.csv");
    char delims[] = ",";
    char tempS[200000];
    int node;
    string lineStr;	// store the table-header
    getline(inFile, lineStr);

    // store the members of each company
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string data;
        int Hp;
        int colIdx = 0;
        while (getline(ss, data, ','))
        {
            colIdx++;
            if (colIdx == 1)
            {
                Hp = stoi(data);
            }
            if (colIdx == 2)
            {
                node = stoi(data);	// start from 1
                hMembers[Hp].push_back(node);
            }
        }
    }
    inFile.close();
}

void readSPlace()
{
    ifstream inFile("../data/Population.csv");
    char delims[] = ",";
    char tempS[200000];
    int node;
    string lineStr;	// store the table-header
    getline(inFile, lineStr);

    // store the members of each school
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string data;
        int colIdx = 0;
        int Sp = -1;
        int gd = 0;
        int cs = -1;
        while (getline(ss, data, ','))
        {
            colIdx++;
            if (colIdx == 2)
            {
                node = stoi(data);	// start from 1
            }
            if (colIdx == 6)
            {
                Sp = stoi(data);
                if (Sp != -1)
                    schoolGroups[Sp].push_back(node);
            }
        }
    }
    inFile.close();
}

void readWPlace()
{
    ifstream inFile("../data/Population.csv");
    char delims[] = ",";
    char tempS[200000];
    int node;
    string lineStr;	// store the table-header
    getline(inFile, lineStr);

    // store the members of each company
    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string data;
        int colIdx = 0;
        int Wp = -1;
        while (getline(ss, data, ','))
        {
            colIdx++;
            if (colIdx == 2)
            {
                node = stoi(data);	// start from 1
            }
            if (colIdx == 7)
            {
                Wp = stoi(data);
                if (Wp != -1)
                    workplaceGroups[Wp].push_back(node);
            }
        }
    }
    inFile.close();
}


void updateNodeInfectionInfo(int nodeIndex, int timeStep, int mu_ST, int mu_TR)
{
    int infectious_period = sampleExponetialDis(m_infectious_period);
    int incubation_time = sampleGammaDis(shape_incubation, 1 / scale_incubation);
    while (infectious_period == 0)
        infectious_period = sampleExponetialDis(m_infectious_period);
    while (incubation_time == 0)
        incubation_time = sampleGammaDis(shape_incubation, 1 / scale_incubation);
    int latent_time;
    int age = nodeList[nodeIndex].age;

    //For Sensitivity
    int T_ST = round(gsl_ran_poisson(R_GLOBAL, mu_ST));
    int T_TR = round(gsl_ran_poisson(R_GLOBAL, mu_TR));

    nodeList[nodeIndex].T_ST = T_ST;
    nodeList[nodeIndex].T_TR = T_TR;

    if (incubation_time <= 2)
    {
        latent_time = 0;
    }
    else
    {
        latent_time = incubation_time - preSymptom_period;
    }
    nodeList[nodeIndex].infect_time = timeStep;
    nodeList[nodeIndex].latent_time = latent_time;
    nodeList[nodeIndex].incubation_time = incubation_time;
    nodeList[nodeIndex].infectious_time = infectious_period;

    if (symTrue(age))
        nodeList[nodeIndex].current_state = 'I';				//with symptoms
    else
        nodeList[nodeIndex].current_state = 'A';				//without symptoms	

}

vector<long int> getConnectors(long index_node, int index, int mode, int time, int temporal)
{
    // workplace contact pattern
    if (mode == 1)
    {
        if(temporal == 0)
        {
            vector<long int> member_w = contact_workplace[0][index_node][nodeList[index_node].contact_by_time[0]];
            return member_w;
        }
        if(temporal == 1)
        {
            vector<long int> member_w = contact_workplace[0][index_node][time];
            return member_w;
        }
        
    }
    // school contact pattern
    else if (mode == 2)
    {
        if (temporal == 0)
        {
            vector<long int> member_s = contact_schoolplace[0][index_node][nodeList[index_node].contact_by_time[0]];
            return member_s;
        }
        if (temporal == 1)
        {
            vector<long int> member_s = contact_schoolplace[0][index_node][time];
            return member_s;
        }
    }
}

void parseFile(const string& filePath, vector<vector<vector<long int>>>& data)
{
    ifstream file(filePath);
    if (!file.is_open()) { 
        cout << "File error" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string idString;
        getline(iss, idString, ' ');
        int id = stoi(idString);

        string vectorString;
        getline(iss, vectorString);

        vector<int> mainVector;
        istringstream mainIss(vectorString);
        string mainElement;
        int i = 0;
        while (getline(mainIss, mainElement, '[')) {
            vector<int> subVector;
            auto start = find_if(mainElement.begin(), mainElement.end(), [](char c) { return isdigit(c); });
            auto end = find_if(mainElement.rbegin(), mainElement.rend(), [](char c) { return isdigit(c); }).base();

            string subElement(start, end);

            istringstream subIss(subElement);
            string subItem;

            while (getline(subIss, subItem, ',')) {
                int value = stoi(subItem);
                subVector.push_back(value);
            }

            if (!subVector.empty()) {
                data[id][i].insert(data[id][i].end(), subVector.begin(), subVector.end());
                //cout << data[id][i][0];
                ++i;
            }
        }
    }

    file.close();
}


int node_infection_Household(int t, long index_case, vector<long> member, double beta, long seed_node, int mu_ST, int mu_TR)
{
    long i, j;
    int num_infections = 0;
    int size = member.size();
    double temp = 0;        //transimission ability of the infected node
    int t0 = nodeList[index_case].infect_time;
    int latent_time = nodeList[index_case].latent_time;
    int incubation_time = nodeList[index_case].incubation_time;
    int infectious_time = nodeList[index_case].infectious_time;

    int start_infectTime = t0 + latent_time;
    int show_symptomTime = t0 + incubation_time;
    int end_infectTime = t0 + incubation_time + infectious_time;

    int infect_numbers = 0;

    // infect process
    if (nodeList[index_case].current_state == 'A')
    {
        if (t > start_infectTime && t <= end_infectTime)
            temp = k1 * beta;
        else if (t > end_infectTime)
            cout<<"Error in A household infection state updating"<<endl;
    }
    else if (nodeList[index_case].current_state == 'I')
    {
        if (t > start_infectTime && t <= show_symptomTime)
        {
            // show no symptom but can infect nodes
            temp = k1 * beta;
        }
        else if (t > end_infectTime)
        {
            cout<<"Error in I household infection state updating"<<endl;
        }
    }
    else if (nodeList[index_case].current_state == 'X')
    {
        // in end_infectTime it will be R but recover is next to infection
        if (t <= end_infectTime)
            temp = k2 * beta;
        else if (t > end_infectTime) 
            cout<<"Error in X household infection state updating"<<endl;
    }
    long temp_node;
    for (i = 0; i < member.size(); i++)
    {
        temp_node = member[i];
        if (nodeList[temp_node].current_state == 'S' and nodeList[temp_node].quarantine_state != '2')       // is a S individual and not in isolation at special place
        {
            double p_infected = temp * nodeList[temp_node].sus;
            if (rand() / (double)RAND_MAX < p_infected)
            {
                newInfectNodes.push_back(temp_node);
                nodeList[temp_node].infector = index_case;
                num_infections += 1;
                // update node state
                updateNodeInfectionInfo(temp_node, t, mu_ST, mu_TR);
                // calculate the average generation time
                if (index_case == seed_node)
                {
                    int gt = t - nodeList[index_case].infect_time;
                    temp_TG.push_back(gt);
                }
            }
        }
    }
    return num_infections;
}


// infection in school and workplace
int node_infection_setting(int t, long index_case, vector<long> member, double beta, long seed_node, int symbol_NPI, int mu_ST, int mu_TR)
{
    long i, j;
    int num_infections = 0;
    int size = member.size();
    double temp = 0;        //transimission ability of the infected node

    int t0 = nodeList[index_case].infect_time;
    int latent_time = nodeList[index_case].latent_time;
    int incubation_time = nodeList[index_case].incubation_time;
    int infectious_time = nodeList[index_case].infectious_time;

    int start_infectTime = t0 + latent_time;
    int show_symptomTime = t0 + incubation_time;
    int end_infectTime = t0 + incubation_time + infectious_time;

    int infect_numbers = 0;

    // infect process
    if (nodeList[index_case].current_state == 'A')
    {
        if (t > start_infectTime && t <= end_infectTime)
            temp = k1 * beta;
        else if (t > end_infectTime)
            cout<<"Error in setting infection A updating"<<endl;
    }
    else if (nodeList[index_case].current_state == 'I')
    {
        if (t > start_infectTime && t <= show_symptomTime)
        {
            // show no symptom but can infect nodes
            temp = k1 * beta;
        }
        else if (t > end_infectTime)
            cout<<"Error in setting infection I updating"<<endl;
    }
    else if (nodeList[index_case].current_state == 'X')
    {
        if (t <= end_infectTime)
            temp = k2 * beta;
        else if (t > end_infectTime)
            cout<<"Wrong X state updating in setting"<<endl;
    }
    else
        cout<<"Wrong node state."<<endl;

    long temp_node;
    for (i = 0; i < member.size(); i++)
    {
        temp_node = member[i];

        // infectees must be S state and not in isolation
        if (nodeList[temp_node].current_state == 'S' && nodeList[temp_node].quarantine_state == '0' && ((nodeList[temp_node].isTest == 1 and t <= nodeList[temp_node].testTime) or nodeList[temp_node].isTest == 0))
        {
            double p_infected = temp * nodeList[temp_node].sus;

            if (rand() / (double)RAND_MAX < p_infected)
            {
                newInfectNodes.push_back(temp_node);
                nodeList[temp_node].infector = index_case;
                num_infections += 1;
                // update node state
                updateNodeInfectionInfo(temp_node, t, mu_ST, mu_TR);
                // calculate the average generation time
                if (index_case == seed_node)
                {
                    int gt = t - nodeList[index_case].infect_time;
                    temp_TG.push_back(gt);
                }
            }
        }
    }
    return num_infections;
}

// infection in community
int node_infection_community(int t, vector<long> member, double beta, long seed_node, int sd, int mu_ST, int mu_TR)
{
    long i, j, k;
    int num_infections = 0;
    double temp = 0;        //transimission ability of the infected node
    int infect_persons = 0;
    int size_m = member.size();
    long index_case;
    long temp_node;
    vector <long> infectors;
    vector<long>().swap(infectors);
    vector <long> infectees;
    vector<long>().swap(infectees);
    for (i = 0; i < oldInfectNodes.size(); i++)
    {
        index_case = oldInfectNodes[i];
        // if one person have pcr test, then he wouldn't have community contact
        if (nodeList[index_case].quarantine_state != '0')
        {
            continue;
        }
        // if test there must be tti implement
        if (nodeList[temp_node].isTest == 1 and t > nodeList[temp_node].testTime)
        {
            continue;
        }
        int t0 = nodeList[index_case].infect_time;
        int latent_time = nodeList[index_case].latent_time;
        int incubation_time = nodeList[index_case].incubation_time;
        int infectious_time = nodeList[index_case].infectious_time;

        int start_infectTime = t0 + latent_time;
        int show_symptomTime = t0 + incubation_time;
        int end_infectTime = t0 + incubation_time + infectious_time;

        // infect process
        if (nodeList[index_case].current_state == 'A')
        {
            if (t > start_infectTime && t <= end_infectTime)
            {
                temp += k1 * beta;
                infectors.push_back(index_case);
            }
            else if (t > end_infectTime)
                cout<<"Wrong state updating in community infection A"<<endl;
        }
        else if (nodeList[index_case].current_state == 'I')
        {
            if (t > start_infectTime && t <= show_symptomTime)
            {
                // show no symptom but can infect nodes
                temp += k1 * beta;
                infectors.push_back(index_case);
            }
            else if (t > end_infectTime)
                cout<<"Wrong state updating in community infection I"<<endl;
        }
        else if (nodeList[index_case].current_state == 'X')
        {
            if (t <= end_infectTime)
            {
                temp += k2 * beta;
                infectors.push_back(index_case);
            }
            else
                cout<<"Wrong state updating in community infection X"<<endl;
        }
    }
    double infect_numbers;
    // Soical Distance Limitation
    if (sd == 0)
        infect_numbers = temp * mean_com;
    else if (sd == 1)
        infect_numbers = temp * mean_com * 0.3;
    else if (sd == 2)
        infect_numbers = temp * mean_com * 0.5;
    else if (sd == 3)
        infect_numbers = temp * mean_com * 0.7;
    else
        cout << "Wrong sd" << endl;

    infect_persons = gsl_ran_poisson(R_GLOBAL, infect_numbers);

    for (i = 0; i < infect_persons; i++)
    {
        j = rand() % size_m;
        temp_node = member[j];
        // if quarantine or have pcr-test, then would not have community contact
        if (nodeList[temp_node].current_state == 'S' and nodeList[temp_node].quarantine_state == '0' and ((nodeList[temp_node].isTest == 1 and t <= nodeList[temp_node].testTime) or nodeList[temp_node].isTest == 0))
        {
            int age = nodeList[temp_node].age;
            double p_infected;
            if (age < 15)
                p_infected = 0.58 / 1.65;
            else if (age < 65)
                p_infected = 1 / 1.65;
            else
                p_infected = 1;
            if (rand() / (double)RAND_MAX < p_infected)
            {
                newInfectNodes.push_back(temp_node);
                infectees.push_back(temp_node);
                num_infections += 1;
                // update node state
                updateNodeInfectionInfo(temp_node, t, mu_ST, mu_TR);
            }
        }
    }
    if (nodeList[seed_node].current_state != 'R')
    {
        for (i = 0; i < infectees.size(); i++)
        {
            temp_node = infectees[i];
            int index_infector = rand() % infectors.size();
            long infector = infectors[index_infector];
            nodeList[temp_node].infector = infector;
            if (infector == seed_node)
            {
                int gt = t - nodeList[seed_node].infect_time;
                temp_TG.push_back(gt);
            }
        }
    }

    return num_infections;
}

bool isIntegerInVector(const std::vector<long>& vec, long num) {
    for (const auto& m : vec) {
        if (m == num) {
            return true;
        }
    }
    return false;
}

void collectSample_test(int t, long nodeIndex) {

    int T_ST = nodeList[nodeIndex].T_ST;
    int T_TR = nodeList[nodeIndex].T_TR;

    new_toBeCheckedNodeList.push_back(nodeIndex);

    nodeList[nodeIndex].isTest = 1;
    nodeList[nodeIndex].testTime = t + T_ST;

    char state = nodeList[nodeIndex].current_state;
    if (state == 'X')
    {
        bool ToF = resultTrue(T_ST);
        if (ToF == true)
            nodeList[nodeIndex].testResult = 1;
        else
            nodeList[nodeIndex].testResult = -1;
    }
    else
        nodeList[nodeIndex].testResult = -1;
}
// changed in 1.9
void collectSample_test_contact(int t, long nodeIndex, int mu_TR) {

    if(nodeList[nodeIndex].T_TR == 10000)
        nodeList[nodeIndex].T_TR = round(gsl_ran_poisson(R_GLOBAL, mu_TR));
    int T_TR = nodeList[nodeIndex].T_TR;
    new_toBeCheckedNodeList.push_back(nodeIndex);
    nodeList[nodeIndex].isTest = 1;
    nodeList[nodeIndex].testTime = t;

    char state = nodeList[nodeIndex].current_state;
    if (state == 'X')
    {
        bool ToF = resultTrue(t - nodeList[nodeIndex].infect_time - nodeList[nodeIndex].incubation_time);
        if (ToF == true)
            nodeList[nodeIndex].testResult = 1;
        else
            nodeList[nodeIndex].testResult = -1;
    }
    else
        nodeList[nodeIndex].testResult = -1;
}


bool resultTrue(int days)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 100.0);
    double random_value = dis(gen);
    bool result = false;
    if (days <= 7) {
        double probability = 97.9;
        result = (random_value < probability);
    }
    else if (days > 7 && days <= 14) {
        double probability = 68.6;
        result = (random_value < probability);
    }
    else if (days > 14 && days <= 21) {
        double probability = 36.3;
        result = (random_value < probability);
    }
    else if (days > 21 && days <= 28) {
        double probability = 30.0;
        result = (random_value < probability);
    }
    else {
        result = false;
    }
    return result;
}

bool symTrue(int age)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 100.0);
    double random_value = dis(gen);
    bool result = false;
    if (age < 20) {
        double probability = 18.1;
        result = (random_value < probability);
    }
    else if (age >= 20 && age < 40) {
        double probability = 22.4;
        result = (random_value < probability);
    }
    else if (age >= 40 && age < 60) {
        double probability = 30.5;
        result = (random_value < probability);
    }
    else if (age >= 60 && age < 80) {
        double probability = 35.5;
        result = (random_value < probability);
    }
    else if (age >= 80) {
        double probability = 64.6;
        result = (random_value < probability);
    }
    else {
        result = false;
    }
    return result;
}

vector<long> contact_tracing(long node_index, double Pid, int NPI, int time_to_trace, int timeStep)
{
    int i, j;
    long temp_node;
    vector<long> contact_set;

    int h_index = nodeList[node_index].index_h;
    for (i = 0; i < hMembers[h_index].size(); i++)
    {
        temp_node = hMembers[h_index][i];
        if (temp_node != node_index)
        {
            if (nodeList[temp_node].contact == '0')        //trace all individuals
            {
                contact_set.push_back(temp_node);
                nodeList[temp_node].contact = '1';
            }
        }
    }

    for (i = 0; i < time_to_trace; i++)
    {
        int size_trace = nodeList[node_index].contact_by_time.size();
        vector <long> contacts;
        // static
        if (size_trace == 1)
        {
            if (nodeList[node_index].index_sg != -1)
                contacts = getConnectors(node_index, 0, 2, nodeList[node_index].contact_by_time[0], 0);
            if (nodeList[node_index].index_wg != -1)
                contacts = getConnectors(node_index, 0, 1, nodeList[node_index].contact_by_time[0], 0);
            for (j = 0; j < contacts.size(); j++)
            {
                temp_node = contacts[j];
                if (rand() / (double)RAND_MAX < Pid)
                {
                    if (nodeList[temp_node].contact == '0' && nodeList[temp_node].quarantine_state == '0')
                    {
                        contact_set.push_back(temp_node);
                        nodeList[temp_node].contact = '1';
                    }
                }
            }
        }
        // temporal
        else if (size_trace == 365)
        {
            if(timeStep < time_to_trace)
            {
                time_to_trace = timeStep + 1;
            }
            if (nodeList[node_index].index_sg != -1)
                contacts = getConnectors(node_index, 0, 2, nodeList[node_index].contact_by_time[timeStep-1-i], 1);
            if (nodeList[node_index].index_wg != -1)
                contacts = getConnectors(node_index, 0, 1, nodeList[node_index].contact_by_time[timeStep-1-i], 1);
            for (j = 0; j < contacts.size(); j++)
            {
                temp_node = contacts[j];
                if (rand() / (double)RAND_MAX < Pid)
                {
                    if (nodeList[temp_node].contact == '0' && nodeList[temp_node].quarantine_state == '0')
                    {
                        contact_set.push_back(temp_node);
                        nodeList[temp_node].contact = '1';
                    }
                }
            }
        }
    }
    return contact_set;
}

vector<long> contact_of_contactTracing(long node_index, double Pid, int NPI, int time_to_trace, int timeStep)
{
    vector<long> contactSet;
    int testTime = nodeList[node_index].testTime;
    contactSet = contact_tracing(node_index, Pid, NPI, time_to_trace, testTime);

    vector<long> contact_contactSet;
    int i, j;
    long temp_node;
    for (i = 0; i < contactSet.size(); i++)
    {
        temp_node = contactSet[i];
        vector<long> temp_contactSet = contact_tracing(temp_node, Pid, NPI, time_to_trace, timeStep);
        contact_contactSet.push_back(temp_node);
        for (j = 0; j < temp_contactSet.size(); j++)
        {
            contact_contactSet.push_back(temp_contactSet[j]);
        }
    }
    return contact_contactSet;
}

int extractInteger(const std::string& str) {
    size_t start = str.find('[') + 1;
    size_t end = str.find(']');
    return std::stoi(str.substr(start, end - start));
}

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::vector<int> integers;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            int value = extractInteger(line);
            integers.push_back(value);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return integers;
}

int getRandomSample(const std::vector<int>& data) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, data.size() - 1);
    return data[dis(gen)];
}

double getSus(int age)
{
    if (age < 15)
        return 0.58;
    else if (age < 65)
        return 1;
    else
        return 1.65;
}