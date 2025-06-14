#include <Arcane/Arcane.hpp>

using namespace Arcane;

float pitch, yaw;
const float sensitivity = 0.1f;
const float speed = 1.0f;

class Game : public Application {
public:
	Game();
	~Game();

	virtual void Start() override;
	virtual void Update() override;
	virtual void RenderUI() override;
	virtual void Render() override;
	virtual void Stop() override;

	virtual bool IsRunning() const override { return !mWindow.IsClosed(); }

private:
	Window mWindow;
	GraphicsContext mContext;
	Scene mScene;
	ThreadPool mThreads;

	Entity mFloor, mBox, mSun, mPlayer;
};