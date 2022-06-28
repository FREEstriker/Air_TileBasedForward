#pragma once
#include "Behaviour/Behaviour.h"
#include <string>
#include "Asset/Mesh.h"
#include "Core/Graphic/Shader.h"
#include "Asset/Texture2D.h"
#include "Core/Graphic/Material.h"

namespace AirEngine
{
	namespace Test
	{
		class SelfRotateBehaviour : public AirEngine::Behaviour::Behaviour
		{
		private:
			float _eulerAngularVelocity;
		public:
			SelfRotateBehaviour(float eulerAngularVelocity);
			~SelfRotateBehaviour();
			SelfRotateBehaviour(const SelfRotateBehaviour&) = delete;
			SelfRotateBehaviour& operator=(const SelfRotateBehaviour&) = delete;
			SelfRotateBehaviour(SelfRotateBehaviour&&) = delete;
			SelfRotateBehaviour& operator=(SelfRotateBehaviour&&) = delete;
			void OnAwake()override;
			void OnStart()override;
			void OnUpdate()override;
			void OnDestroy()override;
			RTTR_ENABLE(AirEngine::Behaviour::Behaviour)
		};
	}
}
