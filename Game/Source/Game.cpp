#include <Arcane/Arcane.hpp>

using namespace Arcane;

float pitch, yaw;
const float sensitivity = 0.1f;
const float speed = 1.0f;

Scene scene;

namespace Arcane {
	
	struct Tag {
		Tag() { }
		Tag(const std::string &tag) : Name(tag) { }
	
		std::string Name;
	};

}

int main(int argc, char **argv) {
	Window window = Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetMaximized(true);
	
	GraphicsContext context = GraphicsContext::Create(window);
	GraphicsContext::SetCurrent(context);
	
	SetCurrentScene(&scene);
	
	SceneRenderer::Init();

	Entity floor;
	floor.Add<Tag>("Floor");
	printf("Floor: %u\n", floor.GetID());

	MeshData floorData = LoadCube(Vector3(10.0f, 1.0f, 10.0f), false);
	floor.Add<Mesh>(Mesh::Create(context, floorData));

	ImageData grayImage = LoadImage(Color::Gray(), ImageFormat::RGB8);
	
	Texture blackTexture = Texture::Create(context, LoadImage(Color::Black(), ImageFormat::RGBA8));
	ImageData colorTexture = LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Color.png", ImageFormat::RGB8);
	ProcessImage(colorTexture, ImageProcess::FlipVertical);
	ImageData aoTexture = LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_AmbientOcclusion.png", ImageFormat::RGB8); 
	ProcessImage(aoTexture, ImageProcess::FlipVertical);
	ImageData roughnessMap = LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Roughness.png", ImageFormat::RGB8);
	ProcessImage(roughnessMap, ImageProcess::FlipVertical);
	ImageData normalMap = LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Normal.png", ImageFormat::RGB8);
	ProcessImage(normalMap, ImageProcess::FlipVertical);

	PBRMaterial &floorMaterial = floor.Add<PBRMaterial>();
	floorMaterial.AlbedoMap = Texture::Create(context, colorTexture);
	floorMaterial.NormalMap = Texture::Create(context, normalMap);
	floorMaterial.MetallicMap = Texture::Create(context, grayImage);
	floorMaterial.RoughnessMap = Texture::Create(context, roughnessMap);
	floorMaterial.AmbientOcclusionMap = Texture::Create(context, aoTexture);

	Transform &floorTransform = floor.Add<Transform>();
	floorTransform.Position = Vector3(0, -1.0f, 0.0f);

	Entity box;
	box.Add<Tag>("Box");

	MeshData boxData = LoadCube();
	box.Add<Mesh>(Mesh::Create(context, boxData));
	
	colorTexture = LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Color.png", ImageFormat::RGB8);
	ProcessImage(colorTexture, ImageProcess::FlipVertical);
	aoTexture = LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_AmbientOcclusion.png", ImageFormat::RGB8); 
	ProcessImage(aoTexture, ImageProcess::FlipVertical);
	roughnessMap = LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Roughness.png", ImageFormat::RGB8);
	ProcessImage(roughnessMap, ImageProcess::FlipVertical);
	normalMap = LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Normal.png", ImageFormat::RGB8);
	ProcessImage(normalMap, ImageProcess::FlipVertical);
	
	PBRMaterial &boxMaterial = box.Add<PBRMaterial>();
	boxMaterial.AlbedoMap = Texture::Create(context, colorTexture);
	boxMaterial.NormalMap = Texture::Create(context, normalMap);
	boxMaterial.MetallicMap = Texture::Create(context, grayImage);
	boxMaterial.RoughnessMap = Texture::Create(context, roughnessMap);
	boxMaterial.AmbientOcclusionMap = Texture::Create(context, aoTexture);
	
	free(colorTexture.Data);
	free(normalMap.Data);
	free(grayImage.Data);
	free(roughnessMap.Data);
	free(aoTexture.Data);
	
	Transform &boxTransform = box.Add<Transform>();
	boxTransform.Position = { 0.0f, 3.0f, 0.0f  };
	boxTransform.Scale = { 1.0f, 1.0f, 1.0f };

	Entity camera;
	camera.Add<Tag>(Tag("Camera"));

	RenderCamera &renderCamera = camera.Add<RenderCamera>(Camera3D(90.0f, window.GetClientSize(), 0.0001f, 10000.0f));
	camera.Add<PointLight>(PointLight(Color::Red(), 10.0f));
	camera.Add<Transform>();
	GetCurrentScene()->SetMainEntity(camera.GetID());
	
	window.SetVisible(true);
	
	Entity dirLight;
	dirLight.Add<Tag>(Tag("DirLight"));
	dirLight.Add<DirectionalLight>(Color::Gray());
	dirLight.Add<Transform>(Vector3::Zero(), Vector3(-45.0f, 0.0f, 0.0f));

	while (!window.IsClosed()) {
		AR_PROFILE_FRAME_START();

		if (window.IsFocused()) {
			SetCursorLocked(true);
			SetCursorVisible(false);

			yaw -= GetMouseDelta().X * sensitivity;
			pitch -= GetMouseDelta().Y * sensitivity;

			if (pitch >= 89.9f) pitch = 89.9f;
			if (pitch <= -89.9f) pitch = -89.9f;

			Camera3D &cam = camera.Get<RenderCamera>().GetCamera();

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
			
			camera.Get<Transform>().Position = cam.Position;
		} else {
			SetCursorLocked(false);
			SetCursorVisible(true);
		}

		{
			AR_PROFILE_SCOPE("Rendering");
			SceneRenderer::Draw();
		}
		const FrameStatistics &frameStats = PBRRenderer::GetRenderer().GetFrameStatistics();

		context.Present();
		window.Update();

		UpdateInput();
		UpdateTime();

		AR_PROFILE_FRAME_END();
	}

	SceneRenderer::Shutdown();

}