#include "Game.hpp"

Game::Game() {
	mWindow = Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	mWindow.SetMaximized(true);

	mContext = GraphicsContext::Create(mWindow);
	GraphicsContext::SetCurrent(mContext);

	SetCurrentScene(&mScene);

	SceneRenderer::Init();
}

Game::~Game() {

}

void Game::Start() {
	mBox = Entity();
	mBox.Add<Tag>("Box");

	Ref<Importer> imp = Importer::Create("Game/Assets/Models/dragon.glb");
	imp->Import(ImportFlag_SwapWindingOrder | ImportFlag_GenerateTangents);

	mBox.Add<Mesh>(Mesh::Create(mContext, imp->GetNode(1).Mesh));

	ImageData grayImage = LoadImage(Color::Gray(), ImageFormat::RGB8);

	ImageData colorTexture = LoadImage(Color::White(), ImageFormat::RGB8);
	ProcessImage(colorTexture, ImageProcess::FlipVertical);
	ImageData aoTexture = LoadImage(Color::White(), ImageFormat::RGB8);
	ProcessImage(aoTexture, ImageProcess::FlipVertical);
	ImageData roughnessMap = LoadImage(Color::Black(), ImageFormat::RGB8);
	ProcessImage(roughnessMap, ImageProcess::FlipVertical);
	ImageData normalMap = LoadImage(Color(), ImageFormat::RGB8);
	ProcessImage(normalMap, ImageProcess::FlipVertical);
	
	PBRMaterial &boxMaterial = mBox.Add<PBRMaterial>();
	boxMaterial.AlbedoMap = Texture::Create(mContext, colorTexture);
	boxMaterial.NormalMap = Texture::Create(mContext, normalMap);
	boxMaterial.MetallicMap = Texture::Create(mContext, grayImage);
	boxMaterial.RoughnessMap = Texture::Create(mContext, roughnessMap);
	boxMaterial.AmbientOcclusionMap = Texture::Create(mContext, aoTexture);
	
	free(colorTexture.Data);
	free(normalMap.Data);
	free(grayImage.Data);
	free(roughnessMap.Data);
	free(aoTexture.Data);
	
	Transform &boxTransform = mBox.Add<Transform>();
	boxTransform.Position = { 0.0f, 2.0f, 0.0f };

	mPlayer = Entity();
	mPlayer.Add<Tag>(Tag("Camera"));

	RenderCamera &renderCamera = mPlayer.Add<RenderCamera>(Camera3D(90.0f, mWindow.GetClientSize(), 0.0001f, 10000.0f));
	mPlayer.Add<PointLight>(PointLight(Color::Red(), 10.0f));
	mPlayer.Add<Transform>();
	GetCurrentScene()->SetMainEntity(mPlayer.GetID());

	renderCamera.GetCamera().Front = Vector3(0, 0, 1);
	
	mWindow.SetVisible(true);
	
	mSun = Entity();
	mSun.Add<Tag>(Tag("DirLight"));
	mSun.Add<DirectionalLight>(Color::Gray());
	mSun.Add<Transform>(Vector3::Zero(), Vector3(-45.0f, 0.0f, 0.0f));

	float x = 0.0f;
}

void Game::Update() {
	if (mWindow.IsFocused()) {
		SetCursorLocked(true);
		SetCursorVisible(false);

		yaw -= GetMouseDelta().X * sensitivity;
		pitch -= GetMouseDelta().Y * sensitivity;

		if (pitch >= 89.9f) pitch = 89.9f;
		if (pitch <= -89.9f) pitch = -89.9f;

		Camera3D &cam = mPlayer.Get<RenderCamera>().GetCamera();

		Vector3 direction = Vector3(0);
		direction.X = Cos(ToRadians(yaw)) * Cos(ToRadians(pitch));
		direction.Y = Sin(ToRadians(pitch));
		direction.Z = Sin(ToRadians(yaw)) * Cos(ToRadians(pitch));
		cam.Front = Vector3::Normalize(direction);


		if (IsKeyDown(KeyCode::R)) {
			PBRRenderer::Reload();
		}

		if (IsKeyPressed(KeyCode::Space))
			cam.Position += cam.Up * speed * GetDeltaTime();
		if (IsKeyPressed(KeyCode::LeftShift) || IsKeyPressed(KeyCode::RightShift))
			cam.Position -= cam.Up * speed * GetDeltaTime();

		if (IsKeyPressed(KeyCode::W))
			cam.Position += cam.Front * speed * GetDeltaTime();
		if (IsKeyPressed(KeyCode::S))
			cam.Position -= cam.Front * speed * GetDeltaTime();
		if (IsKeyPressed(KeyCode::A))
			cam.Position -= Vector3::Normalize(Vector3::Cross(cam.Up, cam.Front)) * speed * GetDeltaTime();
		if (IsKeyPressed(KeyCode::D))
			cam.Position += Vector3::Normalize(Vector3::Cross(cam.Up, cam.Front)) * speed * GetDeltaTime();
		
		mPlayer.Get<Transform>().Position = cam.Position;
	} else {
		SetCursorLocked(false);
		SetCursorVisible(true);
	}
}

void Game::Render() {
	SceneRenderer::Draw();
	const FrameStatistics &frameStats = PBRRenderer::GetRenderer().GetFrameStatistics();
	mContext.Present();
	mWindow.Update();
}

void Game::Stop() {
	SceneRenderer::Shutdown();
}

Arcane::Application *Arcane::CreateApplication() {
	return new Game();
}

void Arcane::DestroyApplication(Arcane::Application *app) {
	delete app;
}