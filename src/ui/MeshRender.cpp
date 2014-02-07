#include <ui/MeshRender.h>


MeshRender::MeshRender(MeshShPtr mesh)
  : mesh_(mesh)
{
  if (mesh_->isUpdated() || mesh_->isEmpty())
    mesh_->load();
}


MeshRender::~MeshRender()
{
}


void MeshRender::render(float) const
{
  const size_t shapeCount = mesh_->getShapeCount();
  for (size_t i = 0 ; i < shapeCount ; ++i) {
    GlState::bindBuffer(GlState::ELEMENT_ARRAY_BUFFER, mesh_->getIndexBuffer(i));

    // Verticies
    GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_->getVertexBuffer(i));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    const bool hasUVdata = mesh_->hasUVdata(i);
    const bool hasNormalData = mesh_->hasNormalData(i);
    // UV data
    if (hasUVdata) {
      GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_->getUVBuffer(i));
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(1);
    }

    // Normal Data
    if (hasNormalData) {
      GlState::bindBuffer(GlState::ARRAY_BUFFER, mesh_->getNormalBuffer(i));
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      glEnableVertexAttribArray(2);
    }

    glDrawElements(GL_TRIANGLES,
                   mesh_->getIndicesCount(i),
                   GL_UNSIGNED_INT,
                   (GLvoid*)0);

    glDisableVertexAttribArray(0);
    // Indices
    if (hasUVdata) glDisableVertexAttribArray(1);
    if (hasNormalData) glDisableVertexAttribArray(2);
  }
}


void MeshRender::refresh()
{
  if (mesh_->isUpdated())
    mesh_->load();
}
