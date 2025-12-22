#pragma once
#include "GameSmithEngine/Core/Core.h"


namespace GameSmith {
	class JobBatchCounter;

	void GE_API WorkerPauseCurrentJob(Ref<JobBatchCounter> marker);
	void GE_API WorkerCompleteCurrentJob();
};