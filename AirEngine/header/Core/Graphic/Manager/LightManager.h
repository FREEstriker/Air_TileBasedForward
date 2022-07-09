#pragma once
#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace AirEngine
{
	namespace Light
	{
		class LightBase;
	}
	namespace Asset
	{
		class TextureCube;
	}
	namespace Core
	{
		namespace Logic
		{
			namespace Object
			{
				class Component;
			}
		}
		namespace Graphic
		{
			namespace CoreObject
			{
				class Instance;
				class Thread;
			}
			namespace Instance
			{
				class Buffer;
			}
			namespace Command
			{
				class CommandPool;
				class CommandBuffer;
			}
			namespace Manager
			{
				class LightManager final
				{
				public:
					#define MAX_ORTHER_LIGHT_COUNT 1024
					#define MAX_FORWARD_ORTHER_LIGHT_COUNT 4
					LightManager();
					~LightManager();
					LightManager(const LightManager&) = delete;
					LightManager& operator=(const LightManager&) = delete;
					LightManager(LightManager&&) = delete;
					LightManager& operator=(LightManager&&) = delete;
					struct LightInfo
					{
						alignas(4) int type;
						alignas(4) float intensity;
						alignas(4) float minRange;
						alignas(4) float maxRange;
						alignas(16) glm::vec4 extraParamter;
						alignas(16) glm::vec3 position;
						alignas(16) glm::vec3 direction;
						alignas(16) glm::vec4 color;
					};
					struct StagingLightInfos
					{
						int importantLightCount;
						int unimportantLightCount;
						LightInfo ambientLightInfo;
						LightInfo mainLightInfo;
						LightInfo ortherLightInfos[MAX_ORTHER_LIGHT_COUNT];
					};
					struct ForwardLightInfos
					{
						LightInfo ambientLightInfo;
						LightInfo mainLightInfo;
						int importantLightCount;
						LightInfo importantLightInfos[MAX_FORWARD_ORTHER_LIGHT_COUNT];
						int unimportantLightCount;
						LightInfo unimportantLightInfos[MAX_FORWARD_ORTHER_LIGHT_COUNT];
					};
					void SetLightInfo(std::vector<Logic::Object::Component*> lights);
					void CopyLightInfo(Command::CommandBuffer* commandBuffer);
					Asset::TextureCube* AmbientTextureCube();
					Instance::Buffer* ForwardLightInfosBuffer();
				private:
					Instance::Buffer* _stagingBuffer;
					Instance::Buffer* _forwardLightInfosBuffer;

					LightInfo _ambientLightInfo;
					Asset::TextureCube* _ambientTextureCube;
					LightInfo _mainLightInfo;
					std::array<LightInfo, MAX_ORTHER_LIGHT_COUNT> _ortherLightInfos;
					int _ortherLightInfosCount;
				};
			}
		}
	}
}
