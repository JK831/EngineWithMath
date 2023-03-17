#include "pch.h"
#include "Engine.h"
#include "RenderQueue.h"
#include "SceneManager.h"

RenderQueue::RenderQueue()
{
	_materialQueue.reserve(100);
}

RenderQueue::~RenderQueue()
{
	_RSIPtr.reset();
}

void RenderQueue::Init(const WindowInfo& info)
{
	if (_RSIPtr == NULL)
		_RSIPtr = make_unique<WindowsRSI>();
	bool tof = _RSIPtr->Init(info);
}

void RenderQueue::Render()
{
	_RSIPtr->FillBuffer(_backGroundColor.ToColor32());
	_RSIPtr->ClearDepthBuffer();
	GET_SINGLE(SceneManager)->Render();
	_RSIPtr->EndFrame();
}

void RenderQueue::ResizeWindow(const WindowInfo& info)
{
	
	_RSIPtr->Init(info);
}

uint16 RenderQueue::PushMaterial(uint16 InIndexNum, Material& InMaterial)
{
	// �־� ȯ�濡��? static number or ���� ��ȣ �߱����ִ� ��� ������
	// �ش� ��ȣ�� index�� �Ͽ� material ����?
	if (InIndexNum == 0)
	{
		uint16 indexNum;
		_materialQueue.push_back(make_shared<Material>(InMaterial));
		indexNum = (uint16)_materialQueue.size() - 1;
		InMaterial.SetIndexNum(indexNum);
		return indexNum;
	}
	else
	{
		assert(_materialQueue.size() > InIndexNum);

		return InIndexNum;
	}
}

void RenderQueue::DrawIndexedInstance(const ::std::vector<Vertex>& InVertexBuffer, const ::std::vector<uint32>& InIndexBuffer, const Matrix4x4& InMatrix, uint16 InBufferIndex)
{
	size_t indexCount = InIndexBuffer.size();
	size_t triangleCount = indexCount / 3;
	::std::shared_ptr<Material> material = _materialQueue[InBufferIndex];
	shared_ptr<Shader> shader = material->GetShader();
	shared_ptr<Texture> texture = material->GetTexture()[MAIN_TEXTURE_NUM];

	::std::vector<Vertex> worldCoordination = shader->VertexShading(InVertexBuffer, InMatrix);

	// �޽ø� �ﰢ������ �ɰ��� ���� �׸���
	for (int ti = 0; ti < triangleCount; ++ti)
	{
		int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
		std::vector<Vertex> tvs = { worldCoordination[InIndexBuffer[bi0]] , worldCoordination[InIndexBuffer[bi1]] , worldCoordination[InIndexBuffer[bi2]] };
		DrawTriangle2D(tvs, shader, texture);
	}
}

void RenderQueue::DrawTriangle2D(const ::std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture)
{
	// �ﰢ�� ĥ�ϱ�
	// �ﰢ���� ���� ����
	Vector2 minPos(Math::Min3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), Math::Min3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));
	Vector2 maxPos(Math::Max3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), Math::Max3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));

	// �����߽���ǥ�� ���� ���� ���ͷ� ��ȯ
	Vector2 u = (InTvs[1].pos - InTvs[0].pos).ToVector2();
	Vector2 v = (InTvs[2].pos - InTvs[0].pos).ToVector2();

	// ���� �и� �� ( uv * uv - uu * vv )
	float udotv = u.Dot(v);
	float vdotv = v.Dot(v);
	float udotu = u.Dot(u);
	float denominator = udotv * udotv - vdotv * udotu;

	// ��ȭ �ﰢ���̸� �׸��� ����
	if (denominator == 0.f)
	{
		return;
	}

	float invDenominator = 1.f / denominator;

	WindowInfo& CurWnd = GetWindow();

	ScreenPoint lowerLeftPoint = ScreenPoint::ToScreenCoordinate(CurWnd.ToScreenPoint(), minPos);
	ScreenPoint upperRightPoint = ScreenPoint::ToScreenCoordinate(CurWnd.ToScreenPoint(), maxPos);

	// �� ���� ȭ�� ���� ����� ��� Ŭ���� ó��
	lowerLeftPoint.X = Math::Max((int32)0, lowerLeftPoint.X);
	lowerLeftPoint.Y = Math::Min(CurWnd.height, lowerLeftPoint.Y);
	upperRightPoint.X = Math::Min(CurWnd.width, upperRightPoint.X);
	upperRightPoint.Y = Math::Max((int32)0, upperRightPoint.Y);

	// �ﰢ�� ���� �� ��� ���� �����ϰ� ��ĥ
	for (int x = lowerLeftPoint.X; x <= upperRightPoint.X; ++x)
	{
		for (int y = upperRightPoint.Y; y <= lowerLeftPoint.Y; ++y)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 pointToTest = fragment.ToCartesianCoordinate(CurWnd.ToScreenPoint());
			Vector2 w = pointToTest - InTvs[0].pos.ToVector2();
			float wdotu = w.Dot(u);
			float wdotv = w.Dot(v);

			float s = (wdotv * udotv - wdotu * vdotv) * invDenominator;
			float t = (wdotu * udotv - wdotv * udotu) * invDenominator;
			float oneMinusST = 1.f - s - t;
			if (((s >= 0.f) && (s <= 1.f)) && ((t >= 0.f) && (t <= 1.f)) && ((oneMinusST >= 0.f) && (oneMinusST <= 1.f)))
			{
				Vector2 targetUV = InTvs[0].uv * oneMinusST + InTvs[1].uv * s + InTvs[2].uv * t;
				_RSIPtr->DrawPoint(fragment, InShader->PixelShading(InTexture->GetSample(targetUV)));
			}
		}
	}
}

void RenderQueue::DrawTriangle3D(const std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture)
{

}