#include "pch.h"
#include "Engine.h"
#include "RenderQueue.h"
#include "SceneManager.h"

RenderQueue::RenderQueue()
{
	
}

RenderQueue::~RenderQueue()
{
	_RSIPtr.reset();
}

void RenderQueue::Init(const WindowInfo& info)
{
	_RSIPtr = make_unique<WindowsRSI>();
	_RSIPtr->Init(info);
}

void RenderQueue::Render()
{
	GET_SINGLE(SceneManager)->Render();
}

uint16 RenderQueue::PushMaterial(std::shared_ptr<Material> InMaterial)
{
	// 멀쓰 환경에선? static number or 고유 번호 발급해주는 기능 가지고
	// 해당 번호를 index로 하여 material 저장?
	_materialQueue.push_back(InMaterial);
	uint16 indexNum = _materialQueue.size();
	return indexNum;
}

void RenderQueue::DrawIndexedInstance(const ::std::vector<Vertex>& InVertexBuffer, const ::std::vector<uint32>& InIndexBuffer, const Matrix3x3& InMatrix, uint16 InBufferIndex)
{
	size_t indexCount = InIndexBuffer.size();
	size_t triangleCount = indexCount / 3;
	::std::shared_ptr<Material> material = _materialQueue[InBufferIndex];
	shared_ptr<Shader> shader = material->GetShader();
	shared_ptr<Texture> texture = material->GetTexture()[MAIN_TEXTURE_NUM];

	::std::vector<Vertex> worldCoordination = shader->VertexShading(InVertexBuffer, InMatrix);

	// 메시를 삼각형으로 쪼개서 각각 그리기
	for (int ti = 0; ti < triangleCount; ++ti)
	{
		int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
		std::vector<Vertex> tvs = { InVertexBuffer[InIndexBuffer[bi0]] , InVertexBuffer[InIndexBuffer[bi1]] , InVertexBuffer[InIndexBuffer[bi2]] };
		DrawTriangle2D(tvs, shader, texture);
	}
}

void RenderQueue::DrawTriangle2D(const ::std::vector<Vertex>& InTvs, shared_ptr<Shader> InShader, shared_ptr<Texture> InTexture)
{

	// 삼각형 칠하기
	// 삼각형의 영역 설정
	Vector2 minPos(Math::Min3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), Math::Min3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));
	Vector2 maxPos(Math::Max3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), Math::Max3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));

	// 무게중심좌표를 위해 점을 벡터로 변환
	Vector2 u = (InTvs[1].pos - InTvs[0].pos).ToVector2();
	Vector2 v = (InTvs[2].pos - InTvs[0].pos).ToVector2();

	// 공통 분모 값 ( uv * uv - uu * vv )
	float udotv = u.Dot(v);
	float vdotv = v.Dot(v);
	float udotu = u.Dot(u);
	float denominator = udotv * udotv - vdotv * udotu;

	// 퇴화 삼각형이면 그리기 생략
	if (denominator == 0.f)
	{
		return;
	}

	float invDenominator = 1.f / denominator;

	ScreenPoint lowerLeftPoint = ScreenPoint::ToScreenCoordinate(_RSIPtr->GetWindow().ToScreenPoint(), minPos);
	ScreenPoint upperRightPoint = ScreenPoint::ToScreenCoordinate(_RSIPtr->GetWindow().ToScreenPoint(), maxPos);

	// 두 점이 화면 밖을 벗어나는 경우 클리핑 처리
	lowerLeftPoint.X = Math::Max((int32)0, lowerLeftPoint.X);
	lowerLeftPoint.Y = Math::Min(_RSIPtr->GetWindow().height, lowerLeftPoint.Y);
	upperRightPoint.X = Math::Min(_RSIPtr->GetWindow().width, upperRightPoint.X);
	upperRightPoint.Y = Math::Max((int32)0, upperRightPoint.Y);

	// 삼각형 영역 내 모든 점을 점검하고 색칠
	for (int x = lowerLeftPoint.X; x <= upperRightPoint.X; ++x)
	{
		for (int y = upperRightPoint.Y; y <= lowerLeftPoint.Y; ++y)
		{
			ScreenPoint fragment = ScreenPoint(x, y);
			Vector2 pointToTest = fragment.ToCartesianCoordinate(_RSIPtr->GetWindow().ToScreenPoint());
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