#pragma once
#include "Core/Graphic/Rendering/RenderFeatureBase.h"
#include "Core/Graphic/Rendering/RenderPassBase.h"
#include "Core/Graphic/Instance/Buffer.h"
#include "Core/Graphic/Instance/Image.h"
#include "Core/Graphic/Instance/ImageSampler.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/vector_int2.hpp>
#include <qobject.h>
#include <qwidget.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include "Utils/Log.h"

namespace AirEngine
{
	namespace Asset
	{
		class Mesh;
	}
	namespace Core
	{
		namespace Graphic
		{
			namespace Rendering
			{
				class Shader;
				class Material;
			}
			namespace Instance
			{
				class ImageSampler;
			}
		}
	}
	namespace Rendering
	{
		namespace RenderFeature
		{
			class FftOcean_RenderFeature final : public Core::Graphic::Rendering::RenderFeatureBase
			{
			public:
				class FftOcean_Surface_RenderPass final : public Core::Graphic::Rendering::RenderPassBase
				{
				private:
					void OnPopulateRenderPassSettings(RenderPassSettings& settings)override;
				public:
					CONSTRUCTOR(FftOcean_Surface_RenderPass)
					RTTR_ENABLE(Core::Graphic::Rendering::RenderPassBase)
				};
				class FftOcean_RenderFeatureData;
				class FftOceanDataWindow : public QWidget
				{
				private:
					QPushButton button;
					FftOcean_RenderFeatureData& _fftOceanData;
				public:
					FftOceanDataWindow(FftOcean_RenderFeatureData& fftOceanData)
						: _fftOceanData(fftOceanData)
						, button(this)
					{
						QObject::connect(&button, &QPushButton::clicked, this, [this]()->void {
							AirEngine::Utils::Log::Message(std::to_string(_fftOceanData.L));
						});
					}
					~FftOceanDataWindow() = default;

				};
				class FftOceanDataWindowLauncher : public QObject
				{
					FftOceanDataWindow* w;
					FftOcean_RenderFeatureData& _fftOceanData;
					virtual bool event(QEvent* ev)
					{
						if (ev->type() == QEvent::User)
						{
							w = new FftOceanDataWindow(_fftOceanData);
							w->show();
							return true;
						}
						return QObject::event(ev);
					}
				public:
					FftOceanDataWindowLauncher(FftOcean_RenderFeatureData& fftOceanData)
						: _fftOceanData(fftOceanData)
						, w(nullptr)
					{

					}
					~FftOceanDataWindowLauncher()
					{
						w->close();
						delete w;
					}
				};
				class FftOcean_RenderFeatureData final : public Core::Graphic::Rendering::RenderFeatureDataBase
				{
					friend class FftOcean_RenderFeature;
				public:
					bool isInitialized;
					glm::ivec2 imageSize;
					float L;
					glm::ivec2 NM;
					float windRotationAngle;
					float windSpeed;
					float a;
					float windDependency;
					glm::ivec2 minVertexPosition;
					glm::ivec2 maxVertexPosition;
					glm::vec3 displacementFactor;
					float bubblesLambda;
					float bubblesThreshold;
					float bubblesScale;

					FftOceanDataWindowLauncher* launcher;

					Core::Graphic::Rendering::FrameBuffer* frameBuffer;

					Core::Graphic::Instance::Buffer* gaussianNoiseImageStagingBuffer;
					Core::Graphic::Instance::Image* gaussianNoiseImage;

					Core::Graphic::Instance::Image* imageArray;

					Core::Graphic::Instance::Image* phillipsSpectrumImage;
					Core::Graphic::Rendering::Shader* phillipsSpectrumShader;
					Core::Graphic::Rendering::Material* phillipsSpectrumMaterial;

					Core::Graphic::Rendering::Shader* spectrumShader;
					Core::Graphic::Rendering::Material* spectrumMaterial;

					Core::Graphic::Rendering::Shader* ifftShader;
					Core::Graphic::Rendering::Material* ifftMaterial;

					Core::Graphic::Instance::Image* displacementImage;
					Core::Graphic::Instance::Image* normalImage;
					Core::Graphic::Rendering::Shader* resolveShader;
					Core::Graphic::Rendering::Material* resolveMaterial;
				public:

					CONSTRUCTOR(FftOcean_RenderFeatureData)
					RTTR_ENABLE(Core::Graphic::Rendering::RenderFeatureDataBase)
				};

				CONSTRUCTOR(FftOcean_RenderFeature)

			private:
				Core::Graphic::Rendering::RenderPassBase* _renderPass;
				std::string _renderPassName;
				Core::Graphic::Instance::ImageSampler* _pointSampler;
				Core::Graphic::Instance::ImageSampler* _linearSampler;

				Core::Graphic::Rendering::RenderFeatureDataBase* OnCreateRenderFeatureData(Camera::CameraBase* camera)override;
				void OnResolveRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Camera::CameraBase* camera)override;
				void OnDestroyRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)override;

				void OnPrepare(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)override;
				void OnExcute(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer, Camera::CameraBase* camera, std::vector<AirEngine::Renderer::Renderer*>const* rendererComponents)override;
				void OnSubmit(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)override;
				void OnFinish(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)override;

				RTTR_ENABLE(Core::Graphic::Rendering::RenderFeatureBase)
			};
		}
	}
}