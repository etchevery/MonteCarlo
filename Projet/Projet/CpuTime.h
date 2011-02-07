#ifndef CPUTIME_H
#define CPUTIME_H

// getCpuTimeofday: measured in sec/microsec: wall clock CpuTime
// irrespective of CPU/system/threads, etc.
//fonction de Gordon Erlebacher Tel: (850) 644-0186; FAX: (850) 644-0098; Email: erlebach@csit.fsu.edu

#include <string>
#include <windows.h>
#include <time.h>
#include <WinSock.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <vector>



namespace TIMING {

class CpuTime
{
public:
	static std::vector<CpuTime*> CpuTimeList;

private:
	struct timeval t_start, t_end;
	double elapsed;
	double t;
	clock_t t1;
	clock_t t2;
	double scale;
	std::string name;
	std::string unit;
	int count;
	int nbCalls;
	int offset;

public:
	// nbCalls: how many calls before resetting the clock
	// if nbCalls not -1, print time after nbCalls calls
	// offset: how many calls to ignore
	CpuTime();
	CpuTime(const char* name, int nbCalls=-1, int offset=0);
	CpuTime(const CpuTime&);
	~CpuTime();
	void reset();
	void begin();
	void end();

	void stop() { end(); }
	void start() { begin(); }
	static void printAll();
	static void printAllToFile(FILE *out);
	void print();
	void printToFile(FILE *out);
	void printReset();
};

}


#endif
