#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

std::default_random_engine e;							// Random engine for getting random values
std::uniform_real_distribution<float> dist(0.0f,1.0f);
std::uniform_real_distribution<float> dist2(-1.0f,1.0f);

// TODO: Research fuzzy logic it seems like it will have decent applications in this code, Define the fuzzy process 
// TODO: Get the multilayer percetron neural net working

struct PatientProfile
{
	float	age;				// 1) Age (REAL) 
	bool	sex;				// 2) Sex (BINARY) 
	float	painType;			// 3) Chest Pain Type (NOMINAL, values 0,1,2,3) 
	bool	painTypeNorm[4];	
	float	restBloodPressure;	// 4) Resting Blood Pressure (REAL) 
	float	cholesterolLevel;	// 5) Serum Cholesterol in mg/dl (REAL) 
	bool	fastBloodSugar;		// 6) Fasting Blood Sugar > 120 mg/dl (BINARY)  
	float	restECG;			// 7) Resting ECG results (NOMINAL, values 0,1,2) 
	bool	restECGNorm[3];
	float	maxHeartRate;		// 8) Max heart rate achieved (REAL) 
	bool	exerciseAngina;		// 9) Exercise induced angina (BINARY) 
	float	excerciseSTDep;		// 10) ST depression induced by exercise (REAL) 
	float	excercisePeakS;		// 11) Slope of peak exercise segment (ORDERED) 
	float	vesselsIdentified;	// 12) Number of blood vessels coloured by fluoroscopy (REAL) 
	float	thal;				// 13) thal (NOMINAL, values 0,1,2)
	bool	thalNorm[3];
	bool	diseased;			// Patient classification - Yes or no
};

vector<float> trainingListRaw;
vector<float> testingListRaw;
vector<PatientProfile> trainList;
vector<PatientProfile> testList;

float maxVal[20];

//vector<vector<float>> population;	// Population to be used evolved
int numHiddenLayers = 9;
int epochs = 510;
int evoPop = 20;
vector<vector<float>> hiddenLayers;	// Weights for input into the hidden layers
vector<vector<float>> hiddenLayersBias;
vector<float> hiddenOutputWeights;		// Weights for input into the output layer

float weights[20];				// The weights associated with each variable
float thresh[2];				// The threshold as the bias [0] = -1, [1] is its weight

int trainedCorrect = 0;			// Variables to track how many were successfully predicted
int totalCorrect = 0;

vector<float> ReadFile(string filename)
{
	vector<float> list;

	ifstream myReadFile;
	float output;
	myReadFile.open(filename);

	cout << "Reading in : " << filename << endl;

	if (myReadFile.is_open()) 
	{
		while (!myReadFile.eof()) 
		{
			myReadFile >> output;
			//if(list.size() % 14 == 0 && list.size() != 0)
			//	cout << endl;
			//cout << output;			
			
			list.push_back(output);
		}
	}
	myReadFile.close();
	cout << "Read successful." << endl;
	return list;
}

vector<PatientProfile> FilterRawDate(vector<float> list)
{
	vector<PatientProfile> objList;
	PatientProfile obj;
	int indexes = 14;

	for(int i = 0; i < list.size(); i++)
	{
		if(i % indexes == 0 && i != 0)
		{
			objList.push_back(obj);
		}

		if(i % indexes == 0)
		{
			obj.age = list[i];
		}
		else if(i % indexes == 1)
		{
			obj.sex = list[i];
		}
		else if(i % indexes == 2)
		{
			obj.painType = list[i];
		}
		else if(i % indexes == 3)
		{
			obj.restBloodPressure = list[i];
		}
		else if(i % indexes == 4)
		{
			obj.cholesterolLevel = list[i];
		}
		else if(i % indexes == 5)
		{
			obj.fastBloodSugar = list[i];
		}
		else if(i % indexes == 6)
		{
			obj.restECG = list[i];
		}
		else if(i % indexes == 7)
		{
			obj.maxHeartRate = list[i];
		}
		else if(i % indexes == 8)
		{
			obj.exerciseAngina = list[i];
		}
		else if(i % indexes == 9)
		{
			obj.excerciseSTDep = list[i];
		}
		else if(i % indexes == 10)
		{
			obj.excercisePeakS = list[i];
		}
		else if(i % indexes == 11)
		{
			obj.vesselsIdentified = list[i];
		}
		else if(i % indexes == 12)
		{
			obj.thal = list[i];
		}
		else if(i % indexes == 13)
		{
			obj.diseased = list[i];
		}
	}
	
	cout << objList.size() << " records created." << endl;

	return objList;
}

void FindMax(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		if(list[i].age > maxVal[0])					maxVal[0] = list[i].age;
		if(list[i].restBloodPressure > maxVal[1])	maxVal[1] = list[i].restBloodPressure;
		if(list[i].cholesterolLevel > maxVal[2])	maxVal[2] = list[i].cholesterolLevel;
		if(list[i].maxHeartRate > maxVal[3])		maxVal[3] = list[i].maxHeartRate;
		if(list[i].excerciseSTDep > maxVal[4])		maxVal[4] = list[i].excerciseSTDep;
		if(list[i].excercisePeakS > maxVal[5])		maxVal[5] = list[i].excercisePeakS;
		if(list[i].vesselsIdentified > maxVal[6])	maxVal[6] = list[i].vesselsIdentified;
	}
}

void Clamp(vector<PatientProfile>& list)
{
	// This function clamps the values after theyve been normalised
	// at 1 and 0.
	for(int i = 0; i < list.size(); i++)
	{
		if(list[i].age > 1)						maxVal[0] = 1;
		else if(list[i].age < 0)				maxVal[0] = 0;
		if(list[i].restBloodPressure > 1)		maxVal[1] = 1;
		else if(list[i].restBloodPressure < 0)	maxVal[1] = 0;
		if(list[i].cholesterolLevel > 1)		maxVal[2] = 1;
		else if(list[i].cholesterolLevel < 0)	maxVal[2] = 0;
		if(list[i].maxHeartRate > 1)			maxVal[3] = 1;
		else if(list[i].maxHeartRate < 0)		maxVal[3] = 0;
		if(list[i].excerciseSTDep > 1)			maxVal[4] = 1;
		else if(list[i].excerciseSTDep < 0)		maxVal[4] = 0;
		if(list[i].excercisePeakS > 1)			maxVal[5] = 1;
		else if(list[i].excercisePeakS < 0)		maxVal[5] = 0;
		if(list[i].vesselsIdentified > 1)		maxVal[6] = 1;
		else if(list[i].vesselsIdentified < 0)	maxVal[6] = 0;
	}
}

void NormaliseAge(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].age = list[i].age/maxVal[0];
	}
}

void NormalisePain(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		for(int j = 0; j < 4; j++)
		{
			list[i].painTypeNorm[j] = false;
		}

		if(list[i].painType == 1.0f)
			list[i].painTypeNorm[0] = true;
		else if(list[i].painType == 2.0f)
			list[i].painTypeNorm[1] = true;
		else if(list[i].painType == 3.0f)
			list[i].painTypeNorm[2] = true;
		else if(list[i].painType == 4.0f)
			list[i].painTypeNorm[3] = true;
	}
}

void NormaliseBloodPressure(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].restBloodPressure = list[i].restBloodPressure / maxVal[1];
	}
}

void NormaliseCholesterol(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].cholesterolLevel = list[i].cholesterolLevel/maxVal[2];
	}
}

void NormaliseECG(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		for(int j = 0; j < 3; j++)
		{
			list[i].restECGNorm[j] = false;
		}

		if(list[i].restECG == 0.0f)
			list[i].restECGNorm[0] = true;
		else if(list[i].restECG == 1.0f)
			list[i].restECGNorm[1] = true;
		else if(list[i].restECG == 2.0f)
			list[i].restECGNorm[2] = true;
	}
}

void NormaliseHeartRate(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].maxHeartRate = list[i].maxHeartRate/maxVal[3];
	}
}

void NormaliseSTDep(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].excerciseSTDep = list[i].excerciseSTDep/maxVal[4];
	}
}

void NormaliseEPeaks(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].excercisePeakS = list[i].excercisePeakS/maxVal[5];
	}
}

void NormaliseVessels(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		list[i].vesselsIdentified = list[i].vesselsIdentified/maxVal[6];
	}
}

void NormaliseThal(vector<PatientProfile>& list)
{
	for(int i = 0; i < list.size(); i++)
	{
		for(int j = 0; j < 3; j++)
		{
			list[i].thalNorm[j] = false;
		}
		if(list[i].thal == 3.0f)
			list[i].thalNorm[0] = true;
		else if(list[i].thal == 6.0f)
			list[i].thalNorm[1] = true;
		else if(list[i].thal == 7.0f)
			list[i].thalNorm[2] = true;
	}
}

void NormaliseData(vector<PatientProfile>& list)
{
	NormaliseAge(list);
	NormalisePain(list);
	NormaliseBloodPressure(list);
	NormaliseCholesterol(list);
	NormaliseECG(list);
	NormaliseHeartRate(list);
	NormaliseSTDep(list);
	NormaliseEPeaks(list);
	NormaliseVessels(list);
	NormaliseThal(list);
}

void InitHiddenLayers()
{
	for(int i = 0; i < numHiddenLayers; i++)
	{
		hiddenLayers.push_back(vector<float>());
		for(int j = 0; j < 20; j++)
		{
			hiddenLayers[i].push_back(dist(e));			
		}

		hiddenOutputWeights.push_back(dist(e));

		hiddenLayersBias.push_back(vector<float>());
		hiddenLayersBias[i].push_back(-1);
		hiddenLayersBias[i].push_back(dist(e));
	}
}

void Initialise()
{
	// Load files
	trainingListRaw = ReadFile("cwTRAIN.txt");
	testingListRaw = ReadFile("cwTEST.txt");

	trainList = FilterRawDate(trainingListRaw);
	testList = FilterRawDate(testingListRaw);

	FindMax(trainList);			// Find the max value in the normalising list
	NormaliseData(trainList);	
	NormaliseData(testList);

	Clamp(trainList);	// Clamp values incase they are out of range
	Clamp(testList);

	for(int i = 0; i < 20; i++)
	{
		weights[i] = dist2(e);
	}
	
	InitHiddenLayers();

	thresh[0] = -1.0f;
	thresh[1] = 1.0f;
}

float addProperties(PatientProfile& profile)
{
	float added =	profile.age * weights[0] +
					profile.sex * weights[1] +
					profile.painTypeNorm[0] * weights[2] +
					profile.painTypeNorm[1] * weights[3] +
					profile.painTypeNorm[2] * weights[4] +
					profile.painTypeNorm[3] * weights[5] +
					profile.restBloodPressure * weights[6] +
					profile.cholesterolLevel * weights[7] +
					profile.fastBloodSugar * weights[8] +
					profile.restECGNorm[0] * weights[9] +
					profile.restECGNorm[1] * weights[10] +
					profile.restECGNorm[2] * weights[11] +
					profile.maxHeartRate * weights[12] +
					profile.exerciseAngina * weights[13] +
					profile.excerciseSTDep * weights[14] +
					profile.excercisePeakS * weights[15] +
					profile.vesselsIdentified * weights[16] +
					profile.thalNorm[0] * weights[17] +
					profile.thalNorm[1] * weights[18] + 
					profile.thalNorm[2] * weights[19];
	added += thresh[0] * thresh[1];

	return added;
}

float addPropertiesHidden(PatientProfile& profile, int neuronID)
{
	float added =	profile.age * hiddenLayers[neuronID][0] +
					profile.sex * hiddenLayers[neuronID][1] +
					profile.painTypeNorm[0] * hiddenLayers[neuronID][2] +
					profile.painTypeNorm[1] * hiddenLayers[neuronID][3] +
					profile.painTypeNorm[2] * hiddenLayers[neuronID][4] +
					profile.painTypeNorm[3] * hiddenLayers[neuronID][5] +
					profile.restBloodPressure * hiddenLayers[neuronID][6] +
					profile.cholesterolLevel * hiddenLayers[neuronID][7] +
					profile.fastBloodSugar * hiddenLayers[neuronID][8] +
					profile.restECGNorm[0] * hiddenLayers[neuronID][9] +
					profile.restECGNorm[1] * hiddenLayers[neuronID][10] +
					profile.restECGNorm[2] * hiddenLayers[neuronID][11] +
					profile.maxHeartRate * hiddenLayers[neuronID][12] +
					profile.exerciseAngina * hiddenLayers[neuronID][13] +
					profile.excerciseSTDep * hiddenLayers[neuronID][14] +
					profile.excercisePeakS * hiddenLayers[neuronID][15] +
					profile.vesselsIdentified * hiddenLayers[neuronID][16] +
					profile.thalNorm[0] * hiddenLayers[neuronID][17] +
					profile.thalNorm[1] * hiddenLayers[neuronID][18] + 
					profile.thalNorm[2] * hiddenLayers[neuronID][19];
	added += hiddenLayersBias[neuronID][0] * hiddenLayersBias[neuronID][1];

	return added;
}

float HiddenNeuron(PatientProfile profile, int neuronID, bool trainingMode)
{
	float modStep = 0.001f;
	
	float added = addPropertiesHidden(profile, neuronID);
	
	// Calculate Output
	added = (1 / ((1 + pow(2.7183f, -(added/1))))); // Sigmoid
	//added = tanh(added);

	return added;
}

float t = -1.0f;
float t1 = 1.0f;

bool OutputNeuron(vector<float> hiddenOutputs, bool status, bool trainingMode)
{
	float added = 0.0f;
	for(int i = 0; i < hiddenOutputs.size(); i++)
	{
		added += hiddenOutputs[i] * hiddenOutputWeights[i];
	}

	added += t * t1;

	float modStep = 0.001f;
		
	// Calculate Output
	//added = (1 / ((1 + 2.7183f) -(added))); // Sigmoid

	// TODO: Train this better
	if(added >= 0 && status)
	{
		return true;
	}
	if(trainingMode)
	{
		if(status && added < 0)
		{
			float error = 0 - added;

			for(int i = 0; i < numHiddenLayers; i++)
			{
				hiddenOutputWeights[i] += modStep;

				for(int j = 0; j < 20; j++)
				{
					hiddenLayers[i][j] += modStep * dist(e);
				}
				hiddenLayersBias[i][1] += modStep * dist(e);
			}
			
			thresh[1] += modStep;
		}
		else if(!status && added >= 0)
		{
			float error = 1 - added;

			for(int i = 0; i < numHiddenLayers; i++)
			{
				hiddenOutputWeights[i] -= modStep;
				
				for(int j = 0; j < 20; j++)
				{
					hiddenLayers[i][j] -= modStep * dist(e);
				}
				hiddenLayersBias[i][1] -= modStep * dist(e);
			}
			t1 -= modStep;
		}
	}
	else if(!status && added < 0)
	{
		return true; 
	}

	return false;
}

bool SimpleNeuron(PatientProfile profile)
{
	float added =	addProperties(profile);

	if(added >= 0)
	{
		return true;
	}
	else return false;
}

bool SimpleTrainingNeuron(PatientProfile profile)
{
	float modStep = 0.02f;
	
	float added = addProperties(profile);
	
	// Calculate Output
	//added = (1 / ((1 + 2.7183f) -(added))); // Sigmoid

	// TODO: Train this better
	if(profile.diseased && SimpleNeuron(profile))
	{
		return true;
	}
	else if(profile.diseased && !SimpleNeuron(profile))
	{
		for(int i = 0; i < 20; i++)
		{
			weights[i] += modStep * dist(e);
		}
		thresh[1] += modStep * dist(e);
	}
	else if(!profile.diseased && SimpleNeuron(profile))
	{
		for(int i = 0; i < 20; i++)
		{
			weights[i] -= modStep * dist(e);
		}
		thresh[1] -= modStep * dist(e);
	}
	else if(!profile.diseased && !SimpleNeuron(profile))
	{
		return true; 
	}
	

	return false;
}

void SimpleNeuralNet()
{
	trainedCorrect = 0;
	totalCorrect = 0;

	for(int e = 0; e < 530; e++)	// Number of epochs 
	{
		for(int i = 0; i < trainList.size(); i++)
		{		
			SimpleTrainingNeuron(trainList[i]);
		}
	}
	for(int i = 0; i < trainList.size(); i++)
	{
		if(trainList[i].diseased == SimpleNeuron(trainList[i]))
		{
			trainedCorrect++;
		}
	}

	for(int i = 0; i < testList.size(); i++)
	{
		if(testList[i].diseased == SimpleNeuron(testList[i]))
		{
			totalCorrect++;
		}
	}
}

void MLPNeuralNet()
{
	int correct = 0;
	int correct2 = 0;

			
	for(int p = 0; p < trainList.size(); p++)
	{
		for(int e = 0; e < epochs; e++)
		{		
			vector<float> hiddenLayerOutput;

			for(int i = 0; i < hiddenLayers.size(); i++)
			{
				hiddenLayerOutput.push_back(HiddenNeuron(trainList[p], i, true));
			}

			OutputNeuron(hiddenLayerOutput, trainList[p].diseased, true);		
		}
	}	

	// Testing and verification of the network
	for(int p = 0; p < trainList.size(); p++)
	{
		vector<float> hiddenLayerOutput;

		for(int i = 0; i < hiddenLayers.size(); i++)
		{
			hiddenLayerOutput.push_back(HiddenNeuron(trainList[p], i, false));
		}

		if(OutputNeuron(hiddenLayerOutput, trainList[p].diseased, false))
		{
			correct++;
		}
	}

	for(int p = 0; p < testList.size(); p++)
	{
		vector<float> hiddenLayerOutput;

		for(int i = 0; i < hiddenLayers.size(); i++)
		{
			hiddenLayerOutput.push_back(HiddenNeuron(testList[p], i, false));
		}

		if(OutputNeuron(hiddenLayerOutput, testList[p].diseased, false))
		{
			correct2++;
		}
	}

	cout << "MLP Network predicts training : " << correct << endl;
	cout << "MLP Network predicts test : " << correct2 << endl;
}

void MLPEvoNeuralNet()
{
	vector<int> correct;
	vector<int> correct2;

	vector<vector<vector<float>>> population;
	vector<vector<vector<float>>> biases;
	vector<vector<float>> hiddenOutputPop;
	// foreach : train the network

	// --- Initialise the population ---
	for(int i = 0; i < evoPop; i++)	// Number in population
	{
		population.push_back(vector<vector<float>>());
		correct.push_back(0);
		correct2.push_back(0);
		biases.push_back(vector<vector<float>>());
		hiddenOutputPop.push_back(vector<float>());

		for(int j = 0; j < numHiddenLayers; j++)
		{
			population[i].push_back(vector<float>());
			biases[i].push_back(vector<float>());

			for(int k = 0; k < 20; k++)
			{
				population[i][j].push_back(dist2(e));
			}
			biases[i][j].push_back(-1);
			biases[i][j].push_back(dist2(e));
			hiddenOutputPop[i].push_back(dist2(e));
		}		
	}
	// ---------------------------------

	// --- Begin tournament! ---
	for(int rounds = 0; rounds < epochs; rounds++)
	{
		for(int i = 0; i < correct.size(); i++)	// Number in population
		{
			correct[i] = 0;
		}

		for(int i = 0; i < correct2.size(); i++)	// Number in population
		{
			correct2[i] = 0;
		}
		// --- Test the population and get a weight ---

		for(int x = 0; x < population.size(); x++)
		{
			//vector<vector<float>> hiddenLayers;	// Weights for input into the hidden layers
			//vector<vector<float>> hiddenLayersBias;
			//vector<float> hiddenOutputWeights;		// Weights for input into the output layer
			hiddenLayers = population[x];
			hiddenLayersBias = biases[x];
			hiddenOutputWeights = hiddenOutputPop[x];
			// Testing and verification of the network
			for(int p = 0; p < trainList.size(); p++)
			{
				vector<float> hiddenLayerOutput;

				for(int i = 0; i < hiddenLayers.size(); i++)
				{
					hiddenLayerOutput.push_back(HiddenNeuron(trainList[p], i, false));
				}

				if(OutputNeuron(hiddenLayerOutput, trainList[p].diseased, false))
				{
					correct[x]++;
				}
			}

			for(int p = 0; p < testList.size(); p++)
			{
				vector<float> hiddenLayerOutput;

				for(int i = 0; i < hiddenLayers.size(); i++)
				{
					hiddenLayerOutput.push_back(HiddenNeuron(testList[p], i, false));
				}

				if(OutputNeuron(hiddenLayerOutput, testList[p].diseased, false))
				{
					correct2[x]++;
				}
			}

			//cout << "MLP Network predicts training : " << correct[x] << endl;
			//cout << "MLP Network predicts test : " << correct2[x] << endl;
		}
		// --------------------------------------------	

		// --- Find the top contender ---
		vector<vector<float>> strongPopulation;		
		vector<vector<float>> strongBiases;
		vector<float> strongHiddenOutputPop;
		int highestValues = 0;
		vector<int> topValues;
		vector<int> indexes;

		for(int x = 0; x < hiddenOutputPop.size(); x++)
		{
			if(correct[x] > highestValues)
			{
				highestValues = correct[x];
				strongHiddenOutputPop = hiddenOutputPop[x];
			}
		}

		highestValues = 0;

		for(int x = 0; x < biases.size(); x++)
		{
			if(correct[x] > highestValues)
			{
				highestValues = correct[x];
				strongBiases = biases[x];
			}
		}

		highestValues = 0;

		for(int x = 0; x < population.size(); x++)
		{
			if(correct[x] > highestValues)
			{
				highestValues = correct[x];
				topValues.push_back(correct[x]);
				strongPopulation = population[x];
				indexes.push_back(x);
			}
		}
		// -----------------------------
		/*
		// --- Randomise Population ---
		for(int i = 0; i < 20; i++)	// Number in population
		{
			for(int j = 0; j < numHiddenLayers; j++)
			{
				for(int k = 0; k < 20; k++)
				{
					population[i][j][k] = dist2(e);
				}
				biases[i][j][1] = dist2(e);
				hiddenOutputPop[i][j] = dist2(e);
			}		
		}*/
		// ----------------------------
		/*
		// --- Merge Genome ---
		for(int i = 0; i < 20; i++)	// Number in population
		{
			for(int j = 0; j < numHiddenLayers; j++)
			{
				for(int k = 0; k < 20; k++)
				{
					if(dist2(e) >= 0)
						population[i][j][k] = strongPopulation[j][k];
				}
				if(dist2(e) >= 0)
				{
					biases[i][j][1] = strongBiases[j][1];
					hiddenOutputPop[i][j] = strongHiddenOutputPop[j];
				}
			}		
		}
		// ----------------------------
		*/
		
		float mod = 0.001f;
		// --- Merge Genome ---
		for(int i = 0; i < 20; i++)	// Number in population
		{
			for(int j = 0; j < numHiddenLayers; j++)
			{
				for(int k = 0; k < 20; k++)
				{
					if(dist2(e) >= 0 && population[i][j][k] != strongPopulation[j][k])
						population[i][j][k] = strongPopulation[j][k];
					else
					population[i][j][k] += mod * dist2(e);
				}
				if(dist2(e) >= 0  && biases[i][j][1] != strongBiases[j][1])
				{
					biases[i][j][1] = strongBiases[j][1];
				}
				else
				biases[i][j][1] += mod * dist2(e);
				if(dist2(e) >= 0 && hiddenOutputPop[i][j] != strongHiddenOutputPop[j])
				{
					hiddenOutputPop[i][j] = strongHiddenOutputPop[j];
				}
				else
				hiddenOutputPop[i][j] += mod * dist2(e);

			}		
		}
		// ----------------------------
		
		/*
		// --- Half Merge Genome ---
		for(int i = 0; i < 20; i++)	// Number in population
		{
			for(int j = 0; j < numHiddenLayers/2; j++)
			{
				for(int k = 0; k < 20/2; k++)
				{
					population[i][j][k] = strongPopulation[j+numHiddenLayers/2][k];
				}
				biases[i][j][1] = strongBiases[j+numHiddenLayers/2][1];
				hiddenOutputPop[i][j] = strongHiddenOutputPop[j+numHiddenLayers/2];
			}		
		}
		// ----------------------------
		*/
		population[0] = strongPopulation;
		hiddenOutputPop[0] = strongHiddenOutputPop;
		biases[0] = strongBiases ;

		cout << "MLP Network predicts test : " << correct2[0] << endl;
		cout << "MLP Network predicts training : " << highestValues << endl;
		
		
		// ----------------------------
	}
	// ------------------------------------

}

void VerifyPredictions()
{

	for(int i = 0; i < 20; i++)
	{
		cout << "Weight " << i <<  " : " << weights[i] << endl;
	}

	cout << "Threshold : " << thresh[0] << ", threshold weight : " << thresh[1] << endl;
	cout << "Correct Train Predictions : " << trainedCorrect << endl;
	cout << "Correct Test Predictions : " << totalCorrect << endl;
}

int main()
{	
	Initialise();

	// TODO: Have this selected based on user choice
	MLPEvoNeuralNet();

	//MLPNeuralNet();	// TODO: Get this function working!

	//SimpleNeuralNet();

	VerifyPredictions();

	return 0;
}

// Notes fitness is number of correct records found
// TODO: Have user enter their user details to find likely hood of heart disease
//		 Output the weights for the network 
//		 Define number of training iterations at runtime and iteration step