#include "pch.h"
#include <iostream>
#include <locale>
#include <cassert>
#include <stdlib.h>     /* atoi */
//#include <sys/time.h>
#include <string>
#include <windows.h> //para Sleep
//#include <pthread.h>
#include <thread>
#include <future>
#include <chrono>
#include <ctime>
#include <iomanip>
//#include <cpuid.h>
#include <iostream>
#include <locale>
#include <cassert>
#include <stdlib.h>     /* atoi */
#include <time.h> // for clock
#include <string>
using namespace std;

class MyNumPunct : public std::numpunct<char>
{
protected:
	virtual char do_thousands_sep() const { return '.'; }
	virtual std::string do_grouping() const { return "\03"; }
};


struct problemVariables {
	;
	int thNumber;
	int size;
	unsigned long long int solutionsToFind;
	unsigned long long int countSolutionsFound;
	unsigned long long int queensPositioned;
	double timeElapsed;
	float halfSize;
	bool terminateFlag;
	bool stopFlag;
	bool processEndedFlag;
	bool endedFlag;
	string lastSolution;
	string currentPosition;
	string result;
	int currentX;

};
bool stopFlag;
bool terminateFlag;

void findNQueensSolutionWorker(problemVariables& pm, int const x, bool lines[], bool diag1[], bool diag2[], int coordXY[]) {

	if (stopFlag)
	{
		string separator, outputString;
		for (int i = 0; i < x; i++)
		{
			outputString += separator + std::to_string(coordXY[i] + 1);
			separator = ", ";
		}
		pm.currentPosition = outputString;
		pm.currentX = x;
		while (stopFlag)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	if (terminateFlag)
	{
		return;
	}
	if (x == pm.size) {
		// Solution is known here
		/*string separator;
		for(int XX=0; XX<size; XX++)
		{
			cout << separator << coordXY[XX]+1;
			separator = ", ";
		}
		cout << endl;*/

		if (pm.halfSize - 0.5 != (float)coordXY[0])
		{
			/*separator="";
			for(int XX=0; XX<size; XX++)
			{
				cout << separator << (size - coordXY[XX]);
				separator = ", ";
			}
			cout << endl;*/
			pm.countSolutionsFound++;
		}
		pm.countSolutionsFound++; //+ el mirror
		//cout << "Solution found: ";
		string separator, outputString;
		for (int i = 0; i < pm.size; i++)
		{
			outputString += separator + std::to_string(coordXY[i] + 1);
			separator = ", ";
		}
		pm.lastSolution = outputString;
		//pm.result+=pm.result + outputString;
		return;
	}

	for (int y = 0; y < pm.size; y++) {
		if (x == 0)
		{
			if (y >= pm.halfSize)
			{
				return;
			}
		}

		// If position threatened, go to next
		if (lines[y] || diag1[y + x] || diag2[y - x + pm.size])
		{
			continue;
		}
		// position queen
		lines[y] = true;
		diag1[y + x] = true;
		diag2[y - x + pm.size] = true;
		pm.queensPositioned++;
		coordXY[x] = y;
		// search for next queen
		findNQueensSolutionWorker(pm, x + 1, lines, diag1, diag2, coordXY);
		if (pm.solutionsToFind > 0)
		{
			if (pm.solutionsToFind <= pm.countSolutionsFound)
			{
				pm.processEndedFlag = true;
				return;
			}
		}

		// deposition queen
		lines[y] = false;
		diag1[y + x] = false;
		diag2[y - x + pm.size] = false;
	}
}


int thQueen(problemVariables& pm)
{
	static const int x = pm.size;
	bool lines[1000];
	bool diag1[2000];
	bool diag2[2000];
	string separator, outputString;
	separator = "";
	int coordXY[1000];
	// Initialize to false
	for (int i = 0; i < pm.size; i++)
	{
		lines[i] = false;
		coordXY[i] = 0;
	}
	for (int i = 0; i < pm.size * 2; i++)
		diag1[i] = diag2[i] = false;

	// Search
	auto start = std::chrono::system_clock::now();


	/*std::cout << "finished computation at " << std::ctime(&end_time)
			  << "elapsed time: " << elapsed_seconds.count() << "s\n";*/
	findNQueensSolutionWorker(pm, 0, lines, diag1, diag2, coordXY);
	//thread nQueens(findNQueensSolution,std::ref(size), xxx, &lines[0], &diag1[0], &diag2[0], &coordXY[0], std::ref(countSolutionsFound), std::ref(queensPositioned), solutionsToFind);
	//thread nQueens (th, 50, &diag1[0]);
	//nQueens.join();
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	pm.timeElapsed = elapsed_seconds.count();
	//std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	outputString = "------------------------------------ Thread " + std::to_string(pm.thNumber + 1) + " ------------------------------------\n";
	outputString += "N: " + std::to_string(pm.size) +
		"\nTime: " + std::to_string(pm.timeElapsed) + " seconds.";
	if (pm.countSolutionsFound > 0)
	{
		if (pm.terminateFlag)
		{
			outputString += "\nQueens positioned: " + std::to_string(pm.queensPositioned) +
				"\nQueens positioned per second: " + std::to_string((unsigned long long int)(pm.queensPositioned / pm.timeElapsed)) +
				"\nSolutions found: " + std::to_string(pm.countSolutionsFound);
		}
		else
		{

			outputString += "\nQueens positioned before resolving: " + std::to_string(pm.queensPositioned) +
				"\nQueens positioned per second: " + std::to_string((unsigned long long int)(pm.queensPositioned / pm.timeElapsed)) +
				"\nSolutions found: " + std::to_string(pm.countSolutionsFound) +
				"\nLast solution: " + pm.lastSolution;


			/*for(int x=0; x<pm.size; x++)
			{
				outputString += separator + std::to_string(coordXY[x]+1);
				separator = ", ";
			}*/
		}
	}
	else
	{
		outputString += "\nNo solution found!";

	}
	outputString += "\n";
	cout << outputString;
	pm.endedFlag = true;
	/*std::thread myThreads[coresCount];
	for (int qq=0; qq<1; qq++){
		for (int i=0 ; i<size ; i++)
				lines[i] = false;
		for (int i=0 ; i<size*2 ; i++)
			diag1[i] = diag2[i] = false;
		cout << "iniciando"<<endl;
		myThreads[qq] = thread(findNQueensSolution,std::ref(size), xxx, &lines[0], &diag1[0], &diag2[0], &coordXY[0], std::ref(countSolutionsFound), std::ref(queensPositioned), solutionsToFind);
		//new (&myThreads[i]) std::thread(exec, i); //I tried it and it seems to work, but it looks like a bad design or an anti-pattern.
	}
	for (int i=0; i<1; i++){
		myThreads[i].join();
	}
	cout << "capoooo"<<endl;*/
	return 0;
}

int main(int argc, char** args)
{
	int N = (argc > 1) ? atoi(args[1]) : 13;
	int coresCount = 0;
	int iterationQuantity = 1;
	unsigned long long int processTime = 0;
	unsigned long long int betweenTime = 0;
	bool infiniteProcessTime = false;
	bool infiniteIterations = false;
	string typeOfTest;
	unsigned long long int solutionsToFind = 0;
	cout.imbue(locale(locale::classic(), new MyNumPunct));
	cout << "------------------------------------------------------------" << endl;
	cout << "--------------- N Queens benchmark - ver 1.0 ---------------" << endl;
	cout << "------------------------------------------------------------" << endl << endl;
	cout << "Select the type of test:" << endl << "[a] for multithreaded benchmark || [b] for single threaded benchmark || [Any key] for custom" << endl;
	cin >> typeOfTest;
	if (typeOfTest == "a" || typeOfTest == "A")
	{
		N = 20;
		solutionsToFind = 1300000;
		coresCount = std::thread::hardware_concurrency();
		iterationQuantity = 0;
		processTime = 0;
		betweenTime = 0;
		cout << "N = " << N << endl;
		cout << "Solutions to find = " << solutionsToFind << endl;
		cout << "Threads to use (max CPU concurrency is " << std::thread::hardware_concurrency() << ") = " << coresCount << endl;
		cout << "Iterations = " << iterationQuantity << endl;
		cout << "Time to process each iteration in seconds (aprox) = " << processTime << endl;
		cout << "Time between iterations in seconds = " << betweenTime << endl;

	}
	else if (typeOfTest == "b" || typeOfTest == "B")
	{
		N = 20;
		solutionsToFind = 1300000;
		coresCount = 1;
		iterationQuantity = 0;
		processTime = 0;
		betweenTime = 0;
		cout << "N = " << N << endl;
		cout << "Solutions to find = " << solutionsToFind << endl;
		cout << "Threads to use (max CPU concurrency is " << std::thread::hardware_concurrency() << ") = " << coresCount << endl;
		cout << "Iterations = " << iterationQuantity << endl;
		cout << "Time to process each iteration in seconds (aprox) = " << processTime << endl;
		cout << "Time between iterations in seconds = " << betweenTime << endl;
	}
	else
	{
		cout << "N = ";
		cin >> N;
		cout << "Solutions to find = ";
		cin >> solutionsToFind;
		cout << "Threads to use (max CPU concurrency is " << std::thread::hardware_concurrency() << ") = ";
		cin >> coresCount;
		cout << "Iterations = ";
		cin >> iterationQuantity;
		cout << "Time to process each iteration in seconds (aprox) = ";
		cin >> processTime;
		cout << "Time between iterations in seconds = ";
		cin >> betweenTime;
	}

	if (coresCount == 0)
	{
		coresCount = std::thread::hardware_concurrency();
	}

	//auto startTime = std::chrono::system_clock::now();
	//std::time_t time = std::chrono::system_clock::to_time_t(startTime);
	
	//using clock = std::chrono::high_resolution_clock;
	//using unit = std::chrono::seconds;
	//std::chrono::time_point<clock> startTime = clock::now();

	

	std::thread myThreads[512];
	string outputString;

	problemVariables pm[512];

	for (int qq = 0; qq < coresCount; qq++)
	{
		pm[qq].terminateFlag = false;
		pm[qq].stopFlag = false;
		pm[qq].processEndedFlag = false;
		pm[qq].thNumber = qq;
		pm[qq].size = N;
		pm[qq].halfSize = ((float)N / 2);
		pm[qq].solutionsToFind = solutionsToFind;
		pm[qq].countSolutionsFound = 0;
		pm[qq].timeElapsed = 0;
		pm[qq].queensPositioned = 0;
		pm[qq].countSolutionsFound = 0;
		pm[qq].queensPositioned = 0;
		pm[qq].endedFlag = false;
		pm[qq].lastSolution = "";
		pm[qq].currentX = 0;
		pm[qq].result = "";
	}
	auto startTime = std::chrono::system_clock::now();
	//time_t now_c = chrono::system_clock::to_time_t(startTime);

	//cout << endl << "Started search at " << std::put_time(localtime(&now_c), "%c %Z") << endl;
	bool finished = false;

	if (iterationQuantity == 0) { iterationQuantity++; infiniteIterations = true; }
	if (processTime == 0) { infiniteProcessTime = true; processTime = 1; }
	stopFlag = true;
	for (int rep = 0; rep < iterationQuantity && !finished; rep++)
	{
		if (!infiniteProcessTime && !infiniteIterations)
		{
			cout << "Iteration " << (rep + 1) << " started.";
		}
		if (infiniteIterations)
		{
			cout << "Iteration " << (rep + 1) << " started.";
			iterationQuantity++;
		}
		if (!infiniteProcessTime)
		{
			cout << " Processing for " << processTime << " seconds.";
		}
		cout << endl;
		terminateFlag = false;
		for (int qq = 0; qq < coresCount; qq++) {
			//stopFlag[qq] = false;
			//terminateFlag[qq] = false;
			//pm[qq].terminateFlag=false;
			//pm[qq].stopFlag=false;
			if (!myThreads[qq].joinable())
			{
				myThreads[qq] = std::thread(thQueen, std::ref(pm[qq]));
			}
			stopFlag = false;
		}

		for (int pTime = processTime; pTime > 0 && !finished; pTime--)
		{
			if (!infiniteProcessTime)
			{
				cout << pTime << "                           \r";
			}
			else
			{
				pTime++;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
			for (int qq = 0; qq < coresCount; qq++)
			{
				if (pm[qq].endedFlag == true)
				{
					if (qq > 0)
					{
						if (pm[qq].endedFlag == pm[(qq - 1)].endedFlag)
						{
							finished = true;
						}
					}
					else
					{
						finished = true;
					}
				}
				else
				{
					finished = false;
				}
			}
		}
		if (!finished)
		{
			for (int qq = 0; qq < coresCount; qq++) {
				if ((rep + 1) == iterationQuantity)
				{
					//terminateFlag[qq]=true;
					//pm[qq].terminateFlag=true;
					terminateFlag = true;
					myThreads[qq].join();
				}
				else
				{//myThreads[qq].join();
					//pm[qq].stopFlag=true;
					if (betweenTime > 0)
						stopFlag = true;

					std::this_thread::sleep_for(std::chrono::nanoseconds(1));
					outputString += "------------------------------------ Thread " + std::to_string(pm[qq].thNumber + 1) + " - N: " + std::to_string(pm[qq].size) + "------------------------------------\n";
					outputString += "  Queens positioned: " + std::to_string(pm[qq].queensPositioned) +
						"  ||  Solutions found: " + std::to_string(pm[qq].countSolutionsFound) +
						"\n  Last solution: " + pm[qq].lastSolution +
						"\n  Current X: " + std::to_string(pm[qq].currentX) + "  || Current position: " + pm[qq].currentPosition + "\n";
					//stopFlag[qq] = true;
				}
			}
			system("cls");
			cout << outputString << endl << "Iteration " << (rep + 1) << " ended.";
			outputString = "";
			if ((rep + 1) == iterationQuantity)
			{
				cout << endl;
			}
			else
			{
				if (betweenTime > 0)
				{
					cout << " Waiting " << betweenTime << " seconds." << endl;
					for (int bTime = betweenTime; bTime > 0; bTime--)
					{
						cout << bTime << "                           \r";
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
				}
			}

		}
		else
		{
			for (int qq = 0; qq < coresCount; qq++) {
				myThreads[qq].join();
			}
		}
	}
	cout << "                               " << endl;
	auto finishTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = finishTime - startTime;
	//system("cls");
	//time = std::chrono::system_clock::to_time_t(finishTime);

	//auto finishTime = std::chrono::system_clock::now();
	//time_t now_f = chrono::system_clock::to_time_t(finishTime);

	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "------------------------------------ Finished - Summary ------------------------------------" << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "Took " << elapsed_seconds.count() << " seconds" << endl;
	unsigned long long int solutionsCount = 0;
	unsigned long long int queensPositionedPerSecond = 0;
	unsigned long long int queensPositioned = 0;
	for (int qq = 0; qq < coresCount; qq++) {
		//stopFlag[qq] = false;
		//terminateFlag[qq] = false;
		solutionsCount = solutionsCount + pm[qq].countSolutionsFound;
		queensPositionedPerSecond = queensPositionedPerSecond + (unsigned long long int)(pm[qq].queensPositioned / pm[qq].timeElapsed);
		queensPositioned = queensPositioned + pm[qq].queensPositioned;
	}
	cout << "N: " << N << endl;
	cout << "Sum of queens positioned: " << queensPositioned << endl;
	cout << "Sum of queens positioned per second: " << queensPositionedPerSecond << endl;
	cout << "Sum of solutions found: " << solutionsCount << endl;

	cout << endl << endl << "<press any key to exit>";
	cin.get();
	cin.get();
	return 0;
}

void printResult()
{
	system("cls");
	cout << "--------------------------------------------------------------------------------------------" << endl;
	cout << "------------------------------------------ Summary -----------------------------------------" << endl;
	cout << "--------------------------------------------------------------------------------------------" << endl;

}




//#include <iostream>
//#include <locale>
//#include <cassert>
//#include <stdlib.h>     /* atoi */
//#include<time.h> // for clock
//#include <string>
//
//#include <thread>
//
//using namespace std;
//
//struct timeval inicio, fin;
//class MyNumPunct : public std::numpunct<char>
//{
//protected:
//	virtual char do_thousands_sep() const { return '.'; }
//	virtual std::string do_grouping() const { return "\03"; }
//};
//
//void rec(int &size, int x, bool *lines, bool *diag1, bool *diag2, int *coordXY, unsigned long long int &countSolutionsFound, unsigned long long int &queensPositioned, unsigned long long int solutionsToFind) {
//
//	if (x == size) {
//		// Solution is known here
//		if (((float)size / 2) - 0.5 != (float)coordXY[0])
//		{
//			countSolutionsFound++;
//		}
//		countSolutionsFound++; //+ el mirror
//		/*cout << "Solution found: ";
//		string separator;
//		for(int XX=0; XX<size; XX++)
//		{
//			cout << separator << coordXY[XX]+1;
//			separator = ", ";
//		}
//		cout << endl;*/
//		return;
//	}
//
//	for (int y = 0; y < size; y++) {
//		if (x == 0 && y >= (float)size / 2)
//		{
//			return;
//		}
//		// If position threatened, go to next
//		if (lines[y] || diag1[y + x] || diag2[y - x + size])
//		{
//			continue;
//		}
//		// position queen
//		lines[y] = true;
//		diag1[y + x] = true;
//		diag2[y - x + size] = true;
//		queensPositioned++;
//		coordXY[x] = y;
//		// search for next queen
//		rec(size, x + 1, lines, diag1, diag2, coordXY, countSolutionsFound, queensPositioned, solutionsToFind);
//		if (solutionsToFind > 0)
//		{
//			if (solutionsToFind <= countSolutionsFound)
//			{
//				return;
//			}
//		}
//
//		// deposition queen
//		lines[y] = false;
//		diag1[y + x] = false;
//		diag2[y - x + size] = false;
//	}
//}
//
//void th(int x)
//{
//	//unsigned long int xxx = 10;
//	for (int i = 0; i < x; i++)
//	{
//		cout << i << endl;
//		//Sleep(xxx);
//	}
//}
//
//int main(int argc, char** args)
//{
//	int size = (argc > 1) ? atoi(args[1]) : 13;
//	unsigned long long int solutionsToFind = 0;
//	unsigned long long int queensPositioned = 0;
//	cout.imbue(locale(locale::classic(), new MyNumPunct));
//	string separator;
//	cout << "N = ";
//	cin >> size;
//	cout << "Solutions to find = ";
//	cin >> solutionsToFind;
//	unsigned long long int countSolutionsFound = 0;
//	bool lines[size];
//	bool diag1[size * 2];
//	bool diag2[size * 2];
//
//	int coordXY[size];
//
//	// Initialize to false
//	for (int i = 0; i < size; i++)
//		lines[i] = false;
//	for (int i = 0; i < size * 2; i++)
//		diag1[i] = diag2[i] = false;
//
//	// Search
//	double t;
//
//	std::thread zote(th, 100);
//	std::thread zotezz(th, 50);
//	zote.join();
//	zotezz.join();
//	cout << "Started search" << endl;
//	//gettimeofday(&inicio, NULL);
//	rec(size, 0, lines, diag1, diag2, coordXY, countSolutionsFound, queensPositioned, solutionsToFind);
//	//gettimeofday(&fin, NULL);
//	//t = (double)((fin.tv_sec * 1000000 + fin.tv_usec) - (inicio.tv_sec * 1000000 + inicio.tv_usec)) / 1000000.0;
//	cout << "N: " << size << endl;
//	cout << "Time: " << t << endl;
//	cout << "Queens positioned before resolving: " << queensPositioned << endl;
//	cout << "Solutions found: " << countSolutionsFound << endl;
//	cout << "Last solution: ";
//	for (int x = 0; x < size; x++)
//	{
//		cout << separator << coordXY[x] + 1;
//		separator = ", ";
//	}
//	cin.get();
//	cin.get();
//	return 0;
//}