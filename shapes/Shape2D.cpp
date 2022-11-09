#include "Shape2D.h"
#include <iostream>

Shape2D::Shape2D(GLProgram *prog, vector<vec3> vertices, vector<vec4> colors, GLenum drawMode, bool doDynamicDraw)
	: program(prog), drawMode(drawMode), dynamicDraw(doDynamicDraw), updated(true), position(vec3(0, 0, 0)),
	  anchorPosition(vec3(0, 0, 0)), scale(vec3(1, 1, 1)), anchorScale(vec3(1, 1, 1)), centerAngle(0), anchorAngle(0)
{
	for (int i = 0; i < vertices.size(); i++)
		this->vertices.push_back(vertices[i]);

	for (int i = 0; i < colors.size(); i++)
		this->colors.push_back(colors[i]);

	this->initShape();
}

void Shape2D::initShape()
{
	GLenum draw_type = this->dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(this->VAO);

	// Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &this->VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vec3), this->vertices.data(), draw_type);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	// Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &(this->VBO_colors));
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_colors);
	glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), draw_type);
	// Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);
}

float Shape2D::getCenterAngle()
{
	return centerAngle;
}

float Shape2D::getAnchorAngle()
{
	return anchorAngle;
}

float Shape2D::getTotalAngle()
{
	return centerAngle + ((int)(!lockedRotation) * anchorAngle);
}

vec3 Shape2D::getPosition()
{
	return position;
}

vec3 Shape2D::getAnchorPosition()
{
	return anchorPosition;
}

vec3 Shape2D::getWorldPosition()
{
	calculateModelIfUpdated();
	return vec3(model * vec4(0, 0, 0, 1));
}

void Shape2D::setScale(float scale)
{
	this->setScale(vec3(scale, scale, 1.0f));
}

void Shape2D::setScale(float xs, float ys, float zs)
{
	this->setScale(vec3(xs, ys, zs));
}

void Shape2D::setScale(vec3 scale)
{
	this->scale = scale;
	this->updated = true;
}

void Shape2D::setScaleRelativeToAnchor(float scale)
{
	this->setScaleRelativeToAnchor(vec3(scale, scale, 1.0f));
}

void Shape2D::setScaleRelativeToAnchor(float xs, float ys, float zs)
{
	this->setScaleRelativeToAnchor(vec3(xs, ys, zs));
}

void Shape2D::setScaleRelativeToAnchor(vec3 scale)
{
	this->anchorScale = scale;
	this->updated = true;
}

void Shape2D::setPosition(float x, float y, float z)
{
	this->setPosition(vec3(x, y, z));
}

void Shape2D::setPosition(vec3 pos)
{
	this->position = pos;
	this->updated = true;
}

void Shape2D::setX(float x)
{
	this->position.x = x;
	this->updated = true;
}

void Shape2D::setY(float y)
{
	this->position.y = y;
	this->updated = true;
}

void Shape2D::setAnchorX(float x)
{
	this->anchorPosition.x = x;
	this->updated = true;
}

void Shape2D::setAnchorY(float y)
{
	this->anchorPosition.y = y;
	this->updated = true;
}

void Shape2D::setAnchorPosition(float x, float y, float z)
{
	this->setAnchorPosition(vec3(x, y, z));
}

void Shape2D::setAnchorPosition(vec3 anchorPos)
{
	this->anchorPosition = anchorPos;
	this->updated = true;
}

void Shape2D::setRotation(float radians)
{
	this->centerAngle = radians;
	this->updated = true;
}

void Shape2D::setRotationAroundAnchor(float radians)
{
	this->anchorAngle = radians;
	this->updated = true;
}

void Shape2D::setRotationLock(bool locked)
{
	this->lockedRotation = locked;
}

void Shape2D::changeScale(float scaleOff)
{
	this->changeScale(vec3(scaleOff, scaleOff, 1.0f));
}

void Shape2D::changeScale(float xsOff, float ysOff, float zsOff)
{
	this->changeScale(vec3(xsOff, ysOff, zsOff));
}

void Shape2D::changeScale(vec3 scaleOff)
{
	this->scale += scaleOff;
	this->updated = true;
}

void Shape2D::changeScaleRelativeToAnchor(float scaleOff)
{
	this->changeScaleRelativeToAnchor(vec3(scaleOff, scaleOff, 1.0f));
}

void Shape2D::changeScaleRelativeToAnchor(float xsOff, float ysOff, float zsOff)
{
	this->changeScaleRelativeToAnchor(vec3(xsOff, ysOff, zsOff));
}

void Shape2D::changeScaleRelativeToAnchor(vec3 scaleOff)
{
	this->anchorScale += scaleOff;
	this->updated = true;
}

void Shape2D::shiftX(float xOff)
{
	this->move(vec3(xOff, 0, 0));
}

void Shape2D::shiftY(float yOff)
{
	this->move(vec3(0, yOff, 0));
}

void Shape2D::shiftAnchorX(float xOff)
{
	this->moveAnchor(vec3(xOff, 0, 0));
}

void Shape2D::shiftAnchorY(float yOff)
{
	this->moveAnchor(vec3(0, yOff, 0));
}

void Shape2D::move(float xOff, float yOff, float zOff)
{
	this->move(vec3(xOff, yOff, zOff));
}

void Shape2D::move(vec3 offset)
{
	this->position += offset;
	this->updated = true;
}

void Shape2D::moveAnchor(float xOff, float yOff, float zOff)
{
	this->moveAnchor(vec3(xOff, yOff, zOff));
}

void Shape2D::moveAnchor(vec3 offset)
{
	this->anchorPosition += offset;
	this->updated = true;
}

void Shape2D::rotate(float radians)
{
	this->centerAngle += radians;
	this->updated = true;
}

void Shape2D::rotateAroundAnchor(float radians)
{
	this->anchorAngle += radians;
	this->updated = true;
}

void Shape2D::calculateModelIfUpdated()
{
	if (updated)
	{
		updated = false;
		model = mat4(1.0);
		model = glm::translate(model, anchorPosition);
		model = glm::rotate(model, anchorAngle, vec3(0, 0, 1.0f));
		model = glm::scale(model, anchorScale);
		model = glm::translate(model, position);
		model = glm::rotate(model, centerAngle - ((int)lockedRotation * anchorAngle), vec3(0, 0, 1.0f));
		model = glm::scale(model, scale);
	}
}

void Shape2D::setEnabled(bool enabled)
{
	this->enabledNextFrame = enabled;
}

void Shape2D::draw()
{
	if (enabled)
	{
		calculateModelIfUpdated();
		GLint modelId = program->getUsedProgram()->getUniformLocation((char *)"model");

		glUniformMatrix4fv(modelId, 1, GL_FALSE, value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(drawMode, 0, vertices.size());
		glBindVertexArray(0);
	}
	enabled = enabledNextFrame; // To avoid shapes appearing/disappearing inconsistently on screen immediately after being enabled/disabled.
}

Shape2D *Shape2D::circle(GLProgram *program, int nvertices, vec4 centerColor, vec4 color)
{
	vector<vec3> vertices;
	vector<vec4> colors;

	vertices.push_back(vec3(0.0f, 0.0f, 1.0f));
	colors.push_back(centerColor);
	for (int i = 0; i < nvertices; i++)
	{
		float t = 2 * 3.14159265f * ((float)i / nvertices);
		vertices.push_back(vec3(cos(t), sin(t), 1.0f));
		colors.push_back(color);
	}
	vertices.push_back(vertices[1]); // L'ultimo vertice è uguale al primo (della circonferenza).
	colors.push_back(color);

	return new Shape2D(program, vertices, colors, GL_TRIANGLE_FAN);
}

#define PHI0(t) (2 * pow((t), 3) - 3 * pow((t), 2) + 1)
#define PHI1(t) (pow((t), 3) - 2 * pow((t), 2) + t)
#define PSI0(t) (-2 * pow((t), 3) + 3 * pow((t), 2))
#define PSI1(t) (pow((t), 3) - pow((t), 2))

float mapT(float t, float t1, float t2)
{
	return (t - t1) / (t2 - t1);
}

void curvePiece(vec3 sPos, vec3 ePos, vec2 sDev, vec2 eDev, float sT, float eT, int nVertices, vector<vec3> *vertexArray)
{
	vertexArray->push_back(sPos);
	for (int i = 1; i < nVertices + 1; i++)
	{
		float normT = mapT(sT + (eT - sT) * i / (nVertices + 1), sT, eT);

		float x = sPos.x * PHI0(normT) + sDev.x * PHI1(normT) * (eT - sT) +
				  ePos.x * PSI0(normT) + eDev.x * PSI1(normT) * (eT - sT);
		float y = sPos.y * PHI0(normT) + sDev.y * PHI1(normT) * (eT - sT) +
				  ePos.y * PSI0(normT) + eDev.y * PSI1(normT) * (eT - sT);
		vertexArray->push_back(vec3(x, y, 0));
	}
	vertexArray->push_back(ePos);
}

Shape2D *Shape2D::HermiteCurve(GLProgram *program, int nPieceVertices, vector<vec3> samples, vector<vec2> derivatives,
							   vec4 color, bool fill, vec3 origin, vec4 centerColor)
{
	vector<vec3> vertices;
	vector<vec4> colors;

	if (samples.size() < 2)
	{
		throw invalid_argument("A curve requires at least 2 points.");
	}

	if (samples.size() != derivatives.size())
	{
		throw invalid_argument("Parameter arrays must have the same size.");
	}

	if (fill)
	{
		vertices.push_back(origin); // Center used to create triangles.
		colors.push_back(centerColor);
	}

	for (int i = 0; i < samples.size(); i++)
	{
		curvePiece(samples[i], samples[(i + 1) % (samples.size())], derivatives[i], derivatives[(i + 1) % (samples.size())], i, i + 1, nPieceVertices, &vertices);
	}

	for (size_t i = 1; i < vertices.size(); i++)
	{
		colors.push_back(color);
	}

	return new Shape2D(program, vertices, colors, fill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
}

vector<vec3> Shape2D::getBoxCollider()
{
	calculateModelIfUpdated();

	vector<vec3> box_collider;

	if (vertices.size() == 0)
		throw invalid_argument("Shape has no vertices.");

	vec3 v0 = vec3(model * vec4(vertices[0], 1));
	float minX = v0.x, minY = v0.y, maxX = v0.x, maxY = v0.y;
	for (size_t i = 1; i < vertices.size(); i++)
	{
		vec3 v = vec3(model * vec4(vertices[i], 1));

		if (minX > v.x)
			minX = v.x;
		else if (maxX < v.x)
			maxX = v.x;

		if (minY > v.y)
			minY = v.y;
		else if (maxY < v.y)
			maxY = v.y;
	}
	box_collider.push_back(vec3(minX, minY, 0));
	box_collider.push_back(vec3(maxX, maxY, 0));

	return box_collider;
}

#define BETWEEN(v1, v, v2) ((v1) <= (v)) && ((v) <= (v2))

bool intersect(vector<vec3> bbox1, vector<vec3> bbox2)
{
	return (BETWEEN(bbox1[0].x, bbox2[0].x, bbox1[1].x) || BETWEEN(bbox1[0].x, bbox2[1].x, bbox1[1].x)) && (BETWEEN(bbox1[0].y, bbox2[0].y, bbox1[1].y) || BETWEEN(bbox1[0].y, bbox2[1].y, bbox1[1].y));
}

bool Shape2D::isColliding(Shape2D *other)
{
	auto bc1 = getBoxCollider();
	auto bc2 = other->getBoxCollider();

	return intersect(bc1, bc2) || intersect(bc2, bc1);
}