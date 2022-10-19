#include "pch.h"
#include "RenderQueue.h"
#include "Material.h"
#include "Engine.h"


uint16 RenderQueue::PushMaterial(std::shared_ptr<JK::Material> InMaterial)
{
	// 멀쓰 환경에선? static number or 고유 번호 발급해주는 기능 가지고
	// 해당 번호를 index로 하여 material 저장?
	_materialQueue.push_back(InMaterial);
	uint16 indexNum = _materialQueue.size();
	return indexNum;
}

void RenderQueue::DrawIndexedInstance(const::std::vector<Vertex>& InVertexBuffer, const::std::vector<uint32>& InIndexBuffer, const JK::Matrix3x3& InMatrix, uint16 InBufferIndex)
{
	size_t indexCount = InIndexBuffer.size();
	size_t triangleCount = indexCount / 3;

	::std::vector<Vertex> worldCoordination = VertexShading(InVertexBuffer, InMatrix);

	// 메시를 삼각형으로 쪼개서 각각 그리기
	for (int ti = 0; ti < triangleCount; ++ti)
	{
		int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
		std::vector<Vertex> tvs = { InVertexBuffer[InIndexBuffer[bi0]] , InVertexBuffer[InIndexBuffer[bi1]] , InVertexBuffer[InIndexBuffer[bi2]] };
		DrawTriangle2D(tvs);
	}
}

void RenderQueue::DrawTriangle2D(const ::std::vector<Vertex>& InTvs, uint16 InBufferIndex)
{

	::std::shared_ptr<JK::Material> material = _materialQueue[InBufferIndex];
	Texture& texture = material->GetTexture();

	// 삼각형 칠하기
	// 삼각형의 영역 설정
	JK::Vector2 minPos(JK::Math::Min3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), JK::Math::Min3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));
	JK::Vector2 maxPos(JK::Math::Max3(InTvs[0].pos.X, InTvs[1].pos.X, InTvs[2].pos.X), JK::Math::Max3(InTvs[0].pos.Y, InTvs[1].pos.Y, InTvs[2].pos.Y));

	// 무게중심좌표를 위해 점을 벡터로 변환
	JK::Vector2 u = InTvs[1].pos - InTvs[0].pos;
	JK::Vector2 v = InTvs[2].pos - InTvs[0].pos;

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

	JK::ScreenPoint lowerLeftPoint = JK::ScreenPoint::ToScreenCoordinate(GEngine->GetWindow().width, minPos);
	JK::ScreenPoint upperRightPoint = JK::ScreenPoint::ToScreenCoordinate(GEngine->GetWindow().height, maxPos);

	// 두 점이 화면 밖을 벗어나는 경우 클리핑 처리
	lowerLeftPoint.X = JK::Math::Max((int32)0, lowerLeftPoint.X);
	lowerLeftPoint.Y = JK::Math::Min(GEngine->GetWindow().height, lowerLeftPoint.Y);
	upperRightPoint.X = JK::Math::Min(GEngine->GetWindow().width, upperRightPoint.X);
	upperRightPoint.Y = JK::Math::Max((int32)0, upperRightPoint.Y);

	// 삼각형 영역 내 모든 점을 점검하고 색칠
	for (int x = lowerLeftPoint.X; x <= upperRightPoint.X; ++x)
	{
		for (int y = upperRightPoint.Y; y <= lowerLeftPoint.Y; ++y)
		{
			JK::ScreenPoint fragment = JK::ScreenPoint(x, y);
			JK::Vector2 pointToTest = fragment.ToCartesianCoordinate(GEngine->GetWindow());
			JK::Vector2 w = pointToTest - InTvs[0].pos;
			float wdotu = w.Dot(u);
			float wdotv = w.Dot(v);

			float s = (wdotv * udotv - wdotu * vdotv) * invDenominator;
			float t = (wdotu * udotv - wdotv * udotu) * invDenominator;
			float oneMinusST = 1.f - s - t;
			if (((s >= 0.f) && (s <= 1.f)) && ((t >= 0.f) && (t <= 1.f)) && ((oneMinusST >= 0.f) && (oneMinusST <= 1.f)))
			{
				JK::Vector2 targetUV = InTvs[0].uv * oneMinusST + InTvs[1].uv * s + InTvs[2].uv * t;
				GEngine->DrawPoint(fragment, PixelShading(texture.GetSample(targetUV), JK::LinearColor::White));
			}
		}
	}
}