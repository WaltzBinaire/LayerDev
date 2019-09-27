#include "layer_aiCollage.h"

REGISTER_TYPE(Layer_file_aiCollage)

vector<string> Layer_file_aiCollage::allowed_exts{
    "svg"
};

void Layer_file_aiCollage::onSetupParams()
{ 
    Layer_file::onSetupParams();

    params.add(
        m_collage.params
    );
}

void Layer_file_aiCollage::onDraw() const
{
    if (m_collage.isLoaded()) {
        glm::vec2 size = scale * glm::vec2(m_collage.getWidth(), m_collage.getHeight());

        m_collage.draw(position - size * 0.5, size.x, size.y);
    }
}

void Layer_file_aiCollage::onUpdate()
{
    if (m_collage.needRedraw()) redraw();
}

void Layer_file_aiCollage::onReset()
{
    if (m_collage.isLoaded()) m_collage.clear();
}

void Layer_file_aiCollage::handle_file(string path)
{
    m_collage.load(path);
    if (m_collage.isLoaded()) redraw(); 
}
