#include <ProjectGE.h>

class GEEditor : public ProjectGE::Application {
public:
	GEEditor() {}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new GEEditor();
}