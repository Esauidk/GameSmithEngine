#pragma once

#include "Serializable.h"

namespace GameSmith {
	// A serializeable object that is defined to be an Asset (either single serializable or bundled serializables)
	class GE_API Asset : public Serializeable {
		// No special behavior besides identifier
	};
};
