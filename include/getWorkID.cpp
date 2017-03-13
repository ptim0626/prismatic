//
// Created by AJ Pryor on 3/13/17.
//

#include "getWorkID.h"
#include "params.h"
#include "configure.h"
#include <mutex>
// helper function for dispatching work
bool getWorkID(const PRISM::Parameters<PRISM_FLOAT_PRECISION>& pars, long long& Nstart, long long& Nstop){
	static std::mutex lock; // mutex to synchronize reading/incrementing job ID
	static long long N_current = 0; // number of next job
	static const long long N_total = pars.xp.size() * pars.yp.size(); // total number of jobs
	constexpr long long NUM_JOBS_PER_CALL = 1; // number of pieces of work to assign to each calling thread
	std::lock_guard<std::mutex> supervisor(lock);
	if (N_current >= N_total) return false; // all jobs done, terminate
	Nstart = N_current;
	Nstop = std::min(N_total, Nstart + NUM_JOBS_PER_CALL);
	N_current = Nstop;
	std::cout << " N_current = " << N_current<< std::endl;
	return true;
}