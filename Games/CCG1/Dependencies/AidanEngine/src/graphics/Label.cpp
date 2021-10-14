#include "Label.h"
namespace engine {
	namespace graphics {

		Label::Label(std::string text, int x, int y, maths::vec4 color, float scale)
		:	Renderable2D(),	text(text), m_Scale(scale), position(m_Position)
		{
			m_Position = maths::vec3(x,y,0);
			m_Color = color;
		}
		 void Label::submit(Renderer2D* renderer) const
		{
			 renderer->drawString(text,position, m_Color, m_Scale);
		}
	}
}
