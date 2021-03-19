#pragma once
#include "stdafx.h"

namespace Shader {
	uint buildVertexShader(std::string const& filename);
	uint buildFragShader(std::string const& filename);
	void deleteShader(uint shader);
}

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::string const& vsFn, std::string const& fsFn);
	~ShaderProgram();

	void setVertexShader(uint shader);
	void setVertexShader(std::string const& filename);
	void setFragShader(uint shader);
	void setFragShader(std::string const& filename);

	void link();

	const uint ID;
private:
	std::set<uint> exclusiveShaders;
};

