#include "pch.h"
#include "RenderQueue.h"

uint16 RenderQueue::PushMaterial(std::shared_ptr<Material> InMaterial)
{
	// 멀쓰 환경에선? static number or 고유 번호 발급해주는 기능 가지고
	// 해당 번호를 index로 하여 material 저장?
	_materialQueue.push_back(InMaterial);
	uint16 indexNum = _materialQueue.size();
	return indexNum;
}

void RenderQueue::DrawIndexedInstance(const::std::vector<Vertex>& InVertexBuffer, const::std::vector<uint32>& InIndexBuffer, const JK::Matrix3x3& InMatrix, uint16 InBufferIndex)
{

}
