#pragma once
#include "Camera/CameraBase.h"

namespace AirEngine
{
	namespace Camera
	{
		class OrthographicCamera final : public Camera::CameraBase
		{
		private:
			void OnSetParameter(glm::vec4& parameter)override;
			void OnSetClipPlanes(glm::vec4* clipPlanes)override;
			void OnSetProjectionMatrix(glm::mat4& matrix)override;

			OrthographicCamera(const OrthographicCamera&) = delete;
			OrthographicCamera& operator=(const OrthographicCamera&) = delete;
			OrthographicCamera(OrthographicCamera&&) = delete;
			OrthographicCamera& operator=(OrthographicCamera&&) = delete;
		public:
			float size;
			OrthographicCamera();
			virtual ~OrthographicCamera();

			RTTR_ENABLE(Camera::CameraBase)
		};
	}
}