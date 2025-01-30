#include <Arcane/Arcane.hpp>

float pitch, yaw;
const float sensitivity = 0.1f;
const float speed = 1.0f;

Arcane::Camera3D camera;

int main(int argc, char **argv) {
	Arcane::Window window = Arcane::Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetMaximized(true);

	Arcane::GraphicsContext context = Arcane::GraphicsContext::Create(window);

	Arcane::PBRRenderer::Init(context);

	Arcane::MeshData meshData = Arcane::LoadCube(Arcane::Vector3(10.0f, 1.0f, 10.0f), false);

	Arcane::Mesh mesh = Arcane::Mesh::Create(context, meshData);

	Arcane::Texture blackTexture = Arcane::Texture::Create(context, Arcane::LoadImage(Arcane::Color::Black(), Arcane::ImageFormat::RGBA8));

	Arcane::ImageData colorTexture = Arcane::LoadImage("Game/Assets/Images/PavingStones_Color.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(colorTexture, Arcane::ImageProcess::FlipVertical);

	Arcane::ImageData aoTexture = Arcane::LoadImage("Game/Assets/Images/PavingStones_AmbientOcclusion.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(aoTexture, Arcane::ImageProcess::FlipVertical);

	Arcane::ImageData roughnessMap = Arcane::LoadImage("Game/Assets/Images/PavingStones_Roughness.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(roughnessMap, Arcane::ImageProcess::FlipVertical);

	Arcane::ImageData normalMap = Arcane::LoadImage("Game/Assets/Images/PavingStones_Normal.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(normalMap, Arcane::ImageProcess::FlipVertical);

	Arcane::PBRMaterial material;
	material.AlbedoMap = Arcane::Texture::Create(context, colorTexture);
	material.NormalMap = Arcane::Texture::Create(context, normalMap);
	material.MetallicMap = Arcane::Texture::Create(context, Arcane::LoadImage(Arcane::Color::Gray(), Arcane::ImageFormat::RGB8));
	material.RoughnessMap = Arcane::Texture::Create(context, roughnessMap);
	material.AmbientOcclusionMap = Arcane::Texture::Create(context, aoTexture);

	Arcane::Transform transform;
	transform.Position = Arcane::Vector3(0, -1.0f, 0.0f);

	camera = Arcane::Camera3D(90.0f, window.GetClientSize(), 0.0001f, 10000.0f);

	window.SetVisible(true);
	Arcane::SetCursorLocked(true);
	Arcane::SetCursorVisible(false);

	Arcane::PointLight light = Arcane::PointLight(Arcane::Color::White(), 2.0f);
	Arcane::Transform lightPosition = Arcane::Transform();
	lightPosition.Position = { -1, 1, 2 };

	while (!window.IsClosed()) {
		lightPosition.Position = camera.Position;

		yaw -= Arcane::GetMouseDelta().X * sensitivity;
		pitch -= Arcane::GetMouseDelta().Y * sensitivity;

		if (pitch >= 89.9f) pitch = 89.9f;
		if (pitch <= -89.9f) pitch = -89.9f;

		Arcane::Vector3 direction = Arcane::Vector3(0);
		direction.X = Arcane::Cos(Arcane::ToRadians(yaw)) * Arcane::Cos(Arcane::ToRadians(pitch));
		direction.Y = Arcane::Sin(Arcane::ToRadians(pitch));
		direction.Z = Arcane::Sin(Arcane::ToRadians(yaw)) * Arcane::Cos(Arcane::ToRadians(pitch));
		camera.Front = Arcane::Vector3::Normalize(direction);

		if (Arcane::IsKeyPressed(Arcane::KeyCode::Space))
			camera.Position += camera.Up * speed * Arcane::GetDeltaTime();
		if (Arcane::IsKeyPressed(Arcane::KeyCode::LeftShift) || Arcane::IsKeyPressed(Arcane::KeyCode::RightShift))
			camera.Position -= camera.Up * speed * Arcane::GetDeltaTime();

		if (Arcane::IsKeyPressed(Arcane::KeyCode::W)) 
			camera.Position += camera.Front * speed * Arcane::GetDeltaTime();
		if (Arcane::IsKeyPressed(Arcane::KeyCode::S))
			camera.Position -= camera.Front * speed * Arcane::GetDeltaTime();
		if (Arcane::IsKeyPressed(Arcane::KeyCode::A))
			camera.Position -= Arcane::Vector3::Normalize(Arcane::Vector3::Cross(camera.Up, camera.Front)) * speed * Arcane::GetDeltaTime();
		if (Arcane::IsKeyPressed(Arcane::KeyCode::D))		
			camera.Position += Arcane::Vector3::Normalize(Arcane::Vector3::Cross(camera.Up, camera.Front)) * speed * Arcane::GetDeltaTime();
			
		Arcane::PBRRenderer::Begin(camera);

		Arcane::PBRRenderer::AddLight(lightPosition, light);
		Arcane::PBRRenderer::Submit(transform, mesh, material);

		Arcane::PBRRenderer::End();
		context.Present();
		window.Update();

		Arcane::UpdateInput();
		Arcane::UpdateTime();
	}

	Arcane::PBRRenderer::Shutdown();

}