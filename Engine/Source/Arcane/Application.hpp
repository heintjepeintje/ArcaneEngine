#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/System/Time.hpp>
#include <Arcane/System/Input.hpp>

namespace Arcane {

	class Application {
	public:
		Application() { }
		virtual ~Application() { }

		virtual void Start() { }
		virtual void Update() { }
		virtual void Render() { }
		virtual void Stop() { }

		virtual bool IsRunning() const = 0;
	};

	extern Application *CreateApplication();
	extern void DestroyApplication(Application *app);
}

int main(int argc, char **argv) {
	Arcane::Application *app = Arcane::CreateApplication();
	app->Start();

	while (app->IsRunning()) {
		AR_PROFILE_FRAME_START();
		{
			AR_PROFILE_SCOPE("Application::Update");
			app->Update();
		}
		
		{
			AR_PROFILE_SCOPE("Application::Render");
			app->Render();
		}

		Arcane::UpdateInput();
		Arcane::UpdateTime();
		
		AR_PROFILE_FRAME_END();
	}

	app->Stop();
	Arcane::DestroyApplication(app);
	return 0;
}