#pragma once
#include "../Renderable2D.h"
namespace engine {
	namespace graphics {

		class Group : public Renderable2D
		{
		private:
			std::vector <Renderable2D*> m_Renderables;
			maths::mat4 m_TransformationMatrix;
		public:
			Group(const maths::mat4& transform);
			void add(Renderable2D* renderable);
			void submit(Renderer2D* renderer) const override;
			void setMatrix(maths::mat4 mat);
			void setSize(maths::vec2 size);
			std::vector<Renderable2D*> getRenderables();
			maths::vec3 getMatPos();
		};

	}
}