#include <ProjectGE.h>

class TestZone : public ProjectGE::Application {
public:
	TestZone() {

	}
	~TestZone() {

	}
};

ProjectGE::Application * ProjectGE::CreateApplication() {
	return new TestZone();
}
