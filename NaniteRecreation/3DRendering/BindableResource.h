#pragma once

class BindableResource {
public:
	virtual void Bind() = 0;
	virtual ~BindableResource() = default;
};
