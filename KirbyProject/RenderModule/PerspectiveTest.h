#pragma once

struct PerspectiveTest
{
	std::function<bool(const Vertex& InVertex)> ClippingTestFunc;
	std::function<Vertex(const Vertex& InStartVertex, const Vertex& InEndVertex)> GetEdgeVertexFunc;
	std::array<bool, 3> TestResult;

	void ClipTriangles(std::vector<Vertex>& InOutVertices)
	{
		size_t triangles = InOutVertices.size() / 3;
		for (size_t ti = 0; ti < triangles; ++ti)
		{
			size_t startIndex = ti * 3;
			size_t nonPassCount = 0;

			for (size_t ix = 0; ix < 3; ++ix)
			{
				TestResult[ix] = ClippingTestFunc(InOutVertices[startIndex + ix]);
				if (TestResult[ix])
				{
					nonPassCount++;
				}
			}

			if (nonPassCount == 0)
			{
				continue;
			}
			else if (nonPassCount == 1)
			{
				DivideIntoTwoTriangles(InOutVertices, startIndex, nonPassCount);
			}
			else if (nonPassCount == 2)
			{
				ClipTriangle(InOutVertices, startIndex, nonPassCount);
			}
			else
			{
				InOutVertices.erase(InOutVertices.begin() + startIndex, InOutVertices.begin() + startIndex + 3);
				triangles--;
				ti--;
			}
		}
	}

private:
	// 점 하나가 평면의 바깥에 있어 삼각형이 2개로 쪼개지는 경우
	void DivideIntoTwoTriangles(std::vector<Vertex>& InOutVertices, size_t StartIndex, size_t NonPassCount)
	{
		// 평면의 바깥에 위치한 점 찾기
		BYTE index = 0;
		if (!TestResult[0])
		{
			index = TestResult[1] ? 1 : 2;
		}

		size_t v1Index = StartIndex + (index + 1) % 3;
		size_t v2Index = StartIndex + (index + 2) % 3;
		Vertex v1 = InOutVertices[v1Index];
		Vertex v2 = InOutVertices[v2Index];
		Vertex clipped1 = GetEdgeVertexFunc(InOutVertices[StartIndex + index], v1);
		Vertex clipped2 = GetEdgeVertexFunc(InOutVertices[StartIndex + index], v2);
		InOutVertices[StartIndex] = clipped1;
		InOutVertices[StartIndex + 1] = v1;
		InOutVertices[StartIndex + 2] = v2;
		InOutVertices.push_back(clipped1);
		InOutVertices.push_back(v2);
		InOutVertices.push_back(clipped2);
	}

	// 점 두 개가 평면의 바깥에 있어 삼각형의 두 점이 변하는 경우
	void ClipTriangle(std::vector<Vertex>& InOutVertices, size_t StartIndex, size_t NonPassCount)
	{
		// 평면의 안쪽에 위치한 점 찾기
		BYTE index = 0;
		if (TestResult[0])
		{
			index = !TestResult[1] ? 1 : 2;
		}

		size_t v1Index = StartIndex + (index + 1) % 3;
		size_t v2Index = StartIndex + (index + 2) % 3;
		Vertex v1 = InOutVertices[v1Index];
		Vertex v2 = InOutVertices[v2Index];
		Vertex clipped1 = GetEdgeVertexFunc(InOutVertices[StartIndex + index], v1);
		Vertex clipped2 = GetEdgeVertexFunc(InOutVertices[StartIndex + index], v2);
		InOutVertices[v1Index] = clipped1;
		InOutVertices[v2Index] = clipped2;
	}
};

static auto TestFuncW0 = [](const Vertex& InVertex) {
	return InVertex.pos.W < 0.f;
};

static auto EdgeFuncW0 = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W;
	float p2 = InEndVertex.pos.W;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncNY = [](const Vertex& InVertex) {
	return InVertex.pos.Y < -InVertex.pos.W;
};

static auto EdgeFuncNY = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W + InStartVertex.pos.Y;
	float p2 = InEndVertex.pos.W + InEndVertex.pos.Y;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncPY = [](const Vertex& InVertex) {
	return InVertex.pos.Y > InVertex.pos.W;
};

static auto EdgeFuncPY = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W - InStartVertex.pos.Y;
	float p2 = InEndVertex.pos.W - InEndVertex.pos.Y;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncNX = [](const Vertex& InVertex) {
	return InVertex.pos.X < -InVertex.pos.W;
};

static auto EdgeFuncNX = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W + InStartVertex.pos.X;
	float p2 = InEndVertex.pos.W + InEndVertex.pos.X;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncPX = [](const Vertex& InVertex) {
	return InVertex.pos.X > InVertex.pos.W;
};

static auto EdgeFuncPX = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W - InStartVertex.pos.X;
	float p2 = InEndVertex.pos.W - InEndVertex.pos.X;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncFar = [](const Vertex& InVertex) {
	return InVertex.pos.Z > InVertex.pos.W;
};

static auto EdgeFuncFar = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W - InStartVertex.pos.Z;
	float p2 = InEndVertex.pos.W - InEndVertex.pos.Z;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};

static auto TestFuncNear = [](const Vertex& InVertex) {
	return InVertex.pos.Z < -InVertex.pos.W;
};

static auto EdgeFuncNear = [](const Vertex& InStartVertex, const Vertex& InEndVertex) {
	float p1 = InStartVertex.pos.W + InStartVertex.pos.Z;
	float p2 = InEndVertex.pos.W + InEndVertex.pos.Z;
	float t = p1 / (p1 - p2);
	return InStartVertex * (1.f - t) + InEndVertex * t;
};


}