#pragma once
#include "GameSmithEngine/Core/Core.h"

#define DEFINE_JOB_FN(FunctionName, Body) void FunctionName(GameSmith::JobStandardParamters jobParams, void* ptr) { \
	JobFnRAII raii; \
	Body \
} \


namespace GameSmith {
	class JobBatchCounter;

	void GE_API WorkerPauseCurrentJob(Ref<JobBatchCounter> marker);
	void GE_API WorkerCompleteCurrentJob();

	// Used to ensure that the current job is marked complete when the job function exits (if using DEFINE_JOB_FN)
	struct JobFnRAII {
		~JobFnRAII() {
			GameSmith::WorkerCompleteCurrentJob();
		}
	};
};