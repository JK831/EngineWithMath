#include "pch.h"
#include "RenderQueue.h"

uint16 RenderQueue::PushMaterial(std::shared_ptr<Material> InMaterial)
{
	// �־� ȯ�濡��? static number or ���� ��ȣ �߱����ִ� ��� ������
	// �ش� ��ȣ�� index�� �Ͽ� material ����?
	_materialQueue.push_back(InMaterial);
	uint16 indexNum = _materialQueue.size();
	return indexNum;
}

void RenderQueue::DrawIndexedInstance(const::std::vector<Vertex>& InVertexBuffer, const::std::vector<uint32>& InIndexBuffer, const JK::Matrix3x3& InMatrix, uint16 InBufferIndex)
{

}
