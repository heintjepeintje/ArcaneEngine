#include "Game.hpp"

Game::Game() {
	mWindow = Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	mWindow.SetMaximized(true);

	mContext = GraphicsContext::Create(mWindow, GraphicsAPI::OpenGL);
	GraphicsContext::SetCurrent(mContext);

	SetCurrentScene(&mScene);

	SceneRenderer::Init();
}

Game::~Game() { }

void Game::Start() {
	Importer importer;
	importer.Import("Game/Assets/Models/dragon_floor.glb", ImportFlag_SwapWindingOrder | ImportFlag_GenerateNormals | ImportFlag_GenerateTangents);

	ImageData colorTexture = LoadImage(Color::White(), ImageFormat::RGB8);
	ImageData aoTexture = LoadImage(Color::White(), ImageFormat::RGB8);
	ImageData roughnessMap = LoadImage(Color::Black(), ImageFormat::RGB8);
	ImageData normalMap = LoadImage(Color::Magenta(), ImageFormat::RGB8);
	ImageData grayImage = LoadImage(Color::Gray(), ImageFormat::RGB8);

	mFloor = Entity();
	mFloor.Add<Tag>("Floor");

	const Node &floorNode = importer.GetNode(1);
	mFloor.Add<Mesh>(Mesh::Create(mContext, floorNode.Mesh));

	Mesh &m = mFloor.Get<Mesh>();
	const Vector3 *positions = m.GetVertexBuffer(0).Map<Vector3>(MapMode::Read);
	const size_t count = m.GetVertexBuffer(0).GetSize() / sizeof(Vector3);

	AR_ENGINE_DEBUG("Floor positions: {}", count);
	for (size_t i = 0; i < count; i++) {
		const Vector3 &pos = positions[i];
		AR_ENGINE_DEBUG("{}, {}, {}", pos.X, pos.Y, pos.Z);
	}

	m.GetVertexBuffer(0).Unmap();
	
	Material &floorMaterial = mFloor.Add<Material>();
	floorMaterial.AlbedoMap = Texture::Create(mContext, colorTexture);
	floorMaterial.NormalMap = Texture::Create(mContext, normalMap);
	floorMaterial.MetallicMap = Texture::Create(mContext, grayImage);
	floorMaterial.RoughnessMap = Texture::Create(mContext, roughnessMap);
	floorMaterial.AmbientOcclusionMap = Texture::Create(mContext, aoTexture);

	Transform &floorTransform = mFloor.Add<Transform>();
	floorTransform.Position = { 0.0f, -1.0f, 0.0f };

	mBox = Entity();
	mBox.Add<Tag>("Box");
	
	mBox.Add<Mesh>(Mesh::Create(mContext, importer.GetNode(0).Mesh));
	
	Material &boxMaterial = mBox.Add<Material>();
	boxMaterial.AlbedoMap = Texture::Create(mContext, colorTexture);
	boxMaterial.NormalMap = Texture::Create(mContext, normalMap);
	boxMaterial.MetallicMap = Texture::Create(mContext, grayImage);
	boxMaterial.RoughnessMap = Texture::Create(mContext, roughnessMap);
	boxMaterial.AmbientOcclusionMap = Texture::Create(mContext, aoTexture);
	
	Transform &boxTransform = mBox.Add<Transform>();
	boxTransform.Position = { 0.0f, 3.0f, 0.0f };

	mPlayer = Entity();
	mPlayer.Add<Tag>("Camera");

	RenderCamera &renderCamera = mPlayer.Add<RenderCamera>(Camera3D(90.0f, mWindow.GetClientSize(), 0.0001f, 10000.0f));
	mPlayer.Add<PointLight>(PointLight(Color::Red(), 10.0f));
	mPlayer.Add<Transform>();

	renderCamera.SetActive(true);
	renderCamera.GetCamera().Front = Vector3(0, 0, 1);
	
	mWindow.SetVisible(true);
	
	mSun = Entity();
	mSun.Add<Tag>("Sun");
	mSun.Add<DirectionalLight>(Color::Gray());
	mSun.Add<Transform>(Vector3::Zero(), Vector3(-45.0f, 0.0f, 0.0f));
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
		// mPlayer.Get<Transform>().Rotation = Vector3(yaw, pitch, 0.0f);

		Vector3 direction = Vector3(0);
		direction.X = Cos(ToRadians(yaw)) * Cos(ToRadians(pitch));
		direction.Y = Sin(ToRadians(pitch));
		direction.Z = Sin(ToRadians(yaw)) * Cos(ToRadians(pitch));
		// cam.Front = mPlayer.Get<Transform>().GetDirection();

		if (IsKeyDown(KeyCode::R)) {
			Renderer::Reload();
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
	const FrameStatistics &frameStats = Renderer::GetRenderer().GetFrameStatistics();
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