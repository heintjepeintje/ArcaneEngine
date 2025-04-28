#include <Arcane/Arcane.hpp>

float pitch, yaw;
const float sensitivity = 0.1f;
const float speed = 1.0f;

Arcane::Camera3D camera;

struct Tag {
	Tag() { }
	Tag(const std::string &tag) : Name(tag) { }

	std::string Name;
};


int main(int argc, char **argv) {
	Arcane::Window window = Arcane::Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetMaximized(true);

	Arcane::Entity firstEntity;
	firstEntity.Add<Arcane::Transform>(Arcane::Vector3(1.0f, 3.0f, 5.0f));

	Arcane::Transform &t = firstEntity.Get<Arcane::Transform>();

	std::printf("Transform: %f, %f, %f\n", t.Position.X, t.Position.Y, t.Position.Z);

	Arcane::GraphicsContext context = Arcane::GraphicsContext::Create(window);

	Arcane::PBRRenderer::Init(context);

	Arcane::MeshData floorData = Arcane::LoadCube(Arcane::Vector3(10.0f, 1.0f, 10.0f), false);
	Arcane::Mesh floor = Arcane::Mesh::Create(context, floorData);
	Arcane::ImageData grayImage = Arcane::LoadImage(Arcane::Color::Gray(), Arcane::ImageFormat::RGB8);
	
	Arcane::Texture blackTexture = Arcane::Texture::Create(context, Arcane::LoadImage(Arcane::Color::Black(), Arcane::ImageFormat::RGBA8));
	Arcane::ImageData colorTexture = Arcane::LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Color.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(colorTexture, Arcane::ImageProcess::FlipVertical);
	Arcane::ImageData aoTexture = Arcane::LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_AmbientOcclusion.png", Arcane::ImageFormat::RGB8); 
	Arcane::ProcessImage(aoTexture, Arcane::ImageProcess::FlipVertical);
	Arcane::ImageData roughnessMap = Arcane::LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Roughness.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(roughnessMap, Arcane::ImageProcess::FlipVertical);
	Arcane::ImageData normalMap = Arcane::LoadImage("Game/Assets/Materials/Paving Stones/PavingStones_Normal.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(normalMap, Arcane::ImageProcess::FlipVertical);

	Arcane::PBRMaterial floorMaterial;
	floorMaterial.AlbedoMap = Arcane::Texture::Create(context, colorTexture);
	floorMaterial.NormalMap = Arcane::Texture::Create(context, normalMap);
	floorMaterial.MetallicMap = Arcane::Texture::Create(context, grayImage);
	floorMaterial.RoughnessMap = Arcane::Texture::Create(context, roughnessMap);
	floorMaterial.AmbientOcclusionMap = Arcane::Texture::Create(context, aoTexture);

	Arcane::Transform floorTransform;
	floorTransform.Position = Arcane::Vector3(0, -1.0f, 0.0f);

	Arcane::MeshData boxData = Arcane::LoadCube();
	Arcane::Mesh box = Arcane::Mesh::Create(context, boxData);
	
	colorTexture = Arcane::LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Color.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(colorTexture, Arcane::ImageProcess::FlipVertical);
	aoTexture = Arcane::LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_AmbientOcclusion.png", Arcane::ImageFormat::RGB8); 
	Arcane::ProcessImage(aoTexture, Arcane::ImageProcess::FlipVertical);
	roughnessMap = Arcane::LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Roughness.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(roughnessMap, Arcane::ImageProcess::FlipVertical);
	normalMap = Arcane::LoadImage("Game/Assets/Materials/Wood Floor/WoodFloor_Normal.png", Arcane::ImageFormat::RGB8);
	Arcane::ProcessImage(normalMap, Arcane::ImageProcess::FlipVertical);
	
	Arcane::PBRMaterial boxMaterial;
	boxMaterial.AlbedoMap = Arcane::Texture::Create(context, colorTexture);
	boxMaterial.NormalMap = Arcane::Texture::Create(context, normalMap);
	boxMaterial.MetallicMap = Arcane::Texture::Create(context, grayImage);
	boxMaterial.RoughnessMap = Arcane::Texture::Create(context, roughnessMap);
	boxMaterial.AmbientOcclusionMap = Arcane::Texture::Create(context, aoTexture);
	
	free(colorTexture.Data);
	free(normalMap.Data);
	free(grayImage.Data);
	free(roughnessMap.Data);
	free(aoTexture.Data);
	
	Arcane::Transform boxTransform;
	boxTransform.Position = { 0.0f, 3.0f, 0.0f  };
	boxTransform.Scale = { 1.0f, 1.0f, 1.0f };
	
	camera = Arcane::Camera3D(90.0f, window.GetClientSize(), 0.0001f, 10000.0f);
	
	window.SetVisible(true);
	
	Arcane::PointLight light = Arcane::PointLight(Arcane::Color::Red(), 10.0f);
	Arcane::Transform lightPosition = Arcane::Transform();
	
	Arcane::DirectionalLight sun = Arcane::DirectionalLight(Arcane::Color::Gray());
	Arcane::Transform sunTransform = Arcane::Transform();
	sunTransform.Rotation.Z = 180.0f;

	char buffer[256];

	while (!window.IsClosed()) {
		AR_PROFILE_FRAME_START();
		lightPosition.Position = camera.Position;

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "Arcane Engine (%.00f fps)", 1.0f / Arcane::GetDeltaTime());
		window.SetTitle(buffer);

		if (window.IsFocused()) {
			Arcane::SetCursorLocked(true);
			Arcane::SetCursorVisible(false);

			yaw -= Arcane::GetMouseDelta().X * sensitivity;
			pitch -= Arcane::GetMouseDelta().Y * sensitivity;

			if (pitch >= 89.9f) pitch = 89.9f;
			if (pitch <= -89.9f) pitch = -89.9f;

			Arcane::Vector3 direction = Arcane::Vector3(0);
			direction.X = Arcane::Cos(Arcane::ToRadians(yaw)) * Arcane::Cos(Arcane::ToRadians(pitch));
			direction.Y = Arcane::Sin(Arcane::ToRadians(pitch));
			direction.Z = Arcane::Sin(Arcane::ToRadians(yaw)) * Arcane::Cos(Arcane::ToRadians(pitch));
			camera.Front = Arcane::Vector3::Normalize(direction);

			if (Arcane::IsKeyDown(Arcane::KeyCode::R)) {
				Arcane::PBRRenderer::Reload();
			}

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
		} else {
			Arcane::SetCursorLocked(false);
			Arcane::SetCursorVisible(true);
		}

		{
			AR_PROFILE_SCOPE("Rendering");
			Arcane::PBRRenderer::SetExposure(1.0);
			Arcane::PBRRenderer::SetGamma(2.2);
			Arcane::PBRRenderer::Begin(camera);

			Arcane::PBRRenderer::AddLight(lightPosition.Position, light);
			Arcane::PBRRenderer::AddLight({ -1, -1, -1 }, sun);
			Arcane::PBRRenderer::Submit(floorTransform, floor, floorMaterial);
			Arcane::PBRRenderer::Submit(boxTransform, box, boxMaterial);

			Arcane::PBRRenderer::End();
		}
		const Arcane::FrameStatistics &frameStats = Arcane::PBRRenderer::GetRenderer().GetFrameStatistics();

		context.Present();
		window.Update();

		Arcane::UpdateInput();
		Arcane::UpdateTime();

		AR_PROFILE_FRAME_END();
	}

	Arcane::PBRRenderer::Shutdown();

}