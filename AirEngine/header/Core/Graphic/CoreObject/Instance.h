#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <mutex>
#include <map>
#include <string>
#include <qvulkanwindow.h>

namespace AirEngine
{
	namespace Utils
	{
		class Condition;
	}
	namespace Core
	{
		namespace Graphic
		{
			namespace Manager
			{
				class MemoryManager;
				class RenderPassManager;
				class DescriptorSetManager;
			}
			namespace CoreObject
			{
				class Thread;
				class Instance;
				struct Queue
				{
					friend class Thread;
					friend class Instance;
					uint32_t queueFamilyIndex;
					VkQueue queue;
					std::mutex mutex;
					std::string name;
				private:
					Queue(std::string name, uint32_t queueFamilyIndex, VkQueue queue);
				};
				class Instance final
				{
					friend class Thread;
				private:
					static std::map<std::string, Queue*> _queues;
					static QVulkanInstance* _qVulkanInstance;
					static VkInstance _vkInstance;
					static VkPhysicalDevice _vkPhysicalDevice;
					static QVulkanDeviceFunctions* _qDeviceFunctions;
					static VkDevice _vkDevice;

					static Manager::MemoryManager* _memoryManager;
					static Manager::RenderPassManager* _renderPassManager;
					static Manager::DescriptorSetManager* _descriptorSetManager;

					static Utils::Condition* _startPresentCondition;
					static Utils::Condition* _endPresentCondition;
					static Utils::Condition* _startRenderCondition;
					static Utils::Condition* _endRenderCondition;

					Instance();
					static void Init();
				public:
					static Queue* Queue_(std::string name);
					static QVulkanInstance* QVulkanInstance_();
					static VkInstance VkInstance_();
					static VkPhysicalDevice VkPhysicalDevice_();
					static QVulkanDeviceFunctions* QVulkanDeviceFunctions_();
					static VkDevice VkDevice_();

					static Manager::MemoryManager& MemoryManager();
					static Manager::RenderPassManager& RenderPassManager();
					static Manager::DescriptorSetManager& DescriptorSetManager();

					static Utils::Condition& StartPresentCondition();
					static Utils::Condition& EndPresentCondition();
					static Utils::Condition& StartRenderCondition();
					static Utils::Condition& EndRenderCondition();
				};
			}
		}
	}
}